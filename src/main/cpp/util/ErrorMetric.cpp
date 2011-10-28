/*
 * ErrorMetric.cpp
 *
 *  Created on: 13.10.2011
 *      Author: thomasstorm
 */

#include <limits>

#include "../core/LookupTable.h"

#include "ErrorMetric.h"

using std::numeric_limits;

ErrorMetric::ErrorMetric(const Context& context) :
		context(context),
		lutOlcRatm((MatrixLookupTable<double>&) context.getObject("OLC_R_atm")),
		lutSlnRatm((MatrixLookupTable<double>&) context.getObject("SLN_R_atm")),
		lutSloRatm((MatrixLookupTable<double>&) context.getObject("SLO_R_atm")),
		lutT((MatrixLookupTable<double>&) context.getObject("t")),
		lutRhoAtm((MatrixLookupTable<double>&) context.getObject("rho_atm")),
		lutTotalAngularWeights((ScalarLookupTable<double>&) context.getObject("weight_ang_tot")),
		lutD((VectorLookupTable<double>&) context.getObject("D")),
		configurationAuxdata((AuxdataProvider&) context.getObject(Constants::AUXDATA_CONFIGURATION_ID)),
		gamma(configurationAuxdata.getDouble("gamma")),
		ndviIndices(configurationAuxdata.getVectorShort("NDV_channel")),
		spectralWeights(configurationAuxdata.getVectorDouble("weight_spec")),
		vegetationSpectrum(configurationAuxdata.getVectorDouble("R_veg")),
		soilReflectance(configurationAuxdata.getVectorDouble("R_soil")),
		angularWeights(configurationAuxdata.getMatrixDouble("weight_ang")),
		sdrs(30),
		rSpec(30),
		rAng(12),
		coordinates(10),
		matRatmOlc(40, 18),
		matRatmSln(40, 6),
		matRatmSlo(40, 6),
		matTs(40, 30),
		matTv(40, 30),
		matRho(40, 30),
		diffuseFraction(6),
		f(lutOlcRatm.getDimensionCount()),
		w(lutOlcRatm.getWorkspaceSize()),
		pn(10),
		p0(10),
		pe(10),
		u(valarray<double>(10), 10),
		lineMinimizer(*this, pn, u)
{
}

void ErrorMetric::setPixel(const Pixel& p) {
	sum2 = 0.0;
	sum4 = 0.0;
	for (size_t i = 0; i < 30; i++) {
		if (p.radiances[i] != Constants::FILL_VALUE_DOUBLE) {
			sum2 += spectralWeights[i];
		}
	}
	for (size_t i = 0; i < 12; i++) {
		if (p.radiances[i + 18] != Constants::FILL_VALUE_DOUBLE) {
			const size_t xIndex = i < 6 ? 0 : 1;
			const size_t yIndex = i % 6;
			sum4 += angularWeights(xIndex, yIndex);
		}
	}
	const double ndvi = computeNdvi(p);
	totalAngularWeight = lutTotalAngularWeights.getValue(&ndvi);

	this->pixel = &p;
}

double ErrorMetric::getValue(double x) {
	computeAtmosphericCorrection(x);
	pn[0] = pixel->c1;
	pn[1] = pixel->c2;
	pn[2] = pixel->nu[0];
	pn[3] = pixel->nu[1];
	for (size_t i = 0; i < 6; i++) {
		pn[i + 4] = pixel->omega[i];
	}
	for (size_t i = 0; i < 10; i++) {
		u[i][i] = 1.0;
	}
	MultiMin::powell(*this, lineMinimizer, pn, p0, pe, u, 1.0e-4, 100);

	return getValue(pn);
}

double ErrorMetric::getValue(valarray<double>& x) {
	for (size_t i = 0; i < 30; i++) {
		rSpec[i] = specModelSurf(x[0], x[1], i);
	}
	for (size_t i = 0; i < 12; i++) {
		rAng[i] = angModelSurf(i, x);
	}
	return errorMetric();
}

static double ozoneTransmission(double cO3, double sza, double vza, double nO3) {
	static const double D2R = 3.14159265358979323846 / 180.0;
	// Eq. 2-2
	const double m = 0.5 * (1.0 / std::cos(sza * D2R) + 1.0 / std::cos(vza * D2R));
	const double tO3 = std::exp(-m * nO3 * cO3);

	return tO3;
}

static double toaReflectance(double ltoa, double f0, double sza) {
	static const double D2R = 3.14159265358979323846 / 180.0;
	static const double PI = 3.14159265358979323846;
	return (PI * ltoa) / (f0 * std::cos(sza * D2R));
}

static double surfaceReflectance(double rtoa, double ratm, double ts, double tv, double rho, double tO3) {
	// Eq. 2-3
	const double f = (rtoa - tO3 * ratm) / (tO3 * ts * tv);
	const double rboa = f / (1.0 + rho * f);

	return rboa;
}

void ErrorMetric::computeAtmosphericCorrection(double tau550) {
	using std::abs;

	const uint8_t amin = pixel->amin;

	coordinates[0] = pixel->sza;
	coordinates[1] = pixel->airPressure;
	coordinates[2] = tau550;
	coordinates[3] = amin;
	lutD.getValues(&coordinates[0], diffuseFraction);

	coordinates[0] = abs(pixel->saa - pixel->vaaOlc); // ADA
	coordinates[1] = pixel->sza; // SZA
	coordinates[2] = pixel->vzaOlc; // VZA
	coordinates[3] = pixel->airPressure; // air pressure
	coordinates[4] = pixel->waterVapour; // water vapour
	coordinates[5] = tau550; // aerosol

	coordinates[6] = coordinates[1]; // SZA
	coordinates[7] = coordinates[3]; // air pressure
	coordinates[8] = coordinates[4]; // water vapour
	coordinates[9] = coordinates[5]; // aerosol

	lutOlcRatm.getValues(&coordinates[0], matRatmOlc, f, w);
	lutT.getValues(&coordinates[6], matTs, f, w);
	lutT.getValues(&coordinates[2], matTv, f, w);
	lutRhoAtm.getValues(&coordinates[3], matRho, f, w);

	for (size_t b = 0; b < 18; b++) {
		if (pixel->radiances[b] != Constants::FILL_VALUE_DOUBLE) {
			// Eq. 2-1
			const double rtoa = toaReflectance(pixel->radiances[b], pixel->solarIrradiances[b], pixel->sza);

			// Eq. 2-2
			const double tO3 = ozoneTransmission(pixel->cO3[b], pixel->sza, pixel->vzaOlc, pixel->ozone);

			// Eq. 2-3
			const double ratm = matRatmOlc(amin - 1, b);
			const double ts = matTs(amin - 1, b);
			const double tv = matTv(amin - 1, b);
			const double rho = matRho(amin - 1, b);
			const double sdr = surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);

			sdrs[b] = sdr;
		}
	}

	coordinates[0] = abs(pixel->saa - pixel->vaaSln); // ADA
	coordinates[1] = pixel->sza; // SZA
	coordinates[2] = pixel->vzaSln; // VZA
	coordinates[3] = pixel->airPressure; // air pressure
	coordinates[4] = pixel->waterVapour; // water vapour
	coordinates[5] = tau550; // aerosol

	lutSlnRatm.getValues(&coordinates[0], matRatmSln, f, w);
	lutT.getValues(&coordinates[2], matTv, f, w);

	for (size_t b = 18; b < 24; b++) {
		if (pixel->radiances[b] != Constants::FILL_VALUE_DOUBLE) {
			// Eq. 2-1
			const double rtoa = toaReflectance(pixel->radiances[b], pixel->solarIrradiances[b], pixel->sza);

			// Eq. 2-2
			const double tO3 = ozoneTransmission(pixel->cO3[b], pixel->sza, pixel->vzaOlc, pixel->ozone);

			// Eq. 2-3
			const double ratm = matRatmSln(amin - 1, b - 18);
			const double ts = matTs(amin - 1, b);
			const double tv = matTv(amin - 1, b);
			const double rho = matRho(amin - 1, b);
			const double sdr = surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);

			sdrs[b] = sdr;
		}
	}

	coordinates[0] = abs(pixel->saa - pixel->vaaSlo); // ADA
	coordinates[1] = pixel->sza; // SZA
	coordinates[2] = pixel->vzaSlo; // VZA
	coordinates[3] = pixel->airPressure; // air pressure
	coordinates[4] = pixel->waterVapour; // water vapour
	coordinates[5] = tau550; // aerosol

	lutSloRatm.getValues(&coordinates[0], matRatmSlo, f, w);
	lutT.getValues(&coordinates[2], matTv, f, w);

	for (size_t b = 24; b < 30; b++) {
		if (pixel->radiances[b] != Constants::FILL_VALUE_DOUBLE) {
			// Eq. 2-1
			const double rtoa = toaReflectance(pixel->radiances[b], pixel->solarIrradiances[b], pixel->sza);

			// Eq. 2-2
			const double tO3 = ozoneTransmission(pixel->cO3[b], pixel->sza, pixel->vzaOlc, pixel->ozone);

			// Eq. 2-3
			const double ratm = matRatmSlo(amin - 1, b - 24);
			const double ts = matTs(amin - 1, b);
			const double tv = matTv(amin - 1, b);
			const double rho = matRho(amin - 1, b);
			const double sdr = surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);

			sdrs[b] = sdr;
		}
	}
}

double ErrorMetric::angModelSurf(size_t index, valarray<double>& x) {
	const size_t j = index % 6;
	const size_t o = index / 6;
	const double d = diffuseFraction[j];
	const double omega = x[j + 4];
	const double nu = x[o + 2];
	const double g = (1 - gamma) * omega;
	return (1 - d) * nu * omega + (gamma * omega) / (1 - g) * (d + g * (1 - d));
}

double ErrorMetric::specModelSurf(double c_1, double c_2, size_t index) {
	return c_1 * vegetationSpectrum[index] + c_2 * soilReflectance[index];
}

double ErrorMetric::errorMetric() {
	double sum1 = 0.0;
	double sum3 = 0.0;
	for (size_t i = 0; i < 30; i++) {
		if (pixel->radiances[i] != Constants::FILL_VALUE_DOUBLE) {
			sum1 += spectralWeights[i] * (sdrs[i] - rSpec[i]) * (sdrs[i] - rSpec[i]);
		}
	}
	for (size_t i = 0; i < 12; i++) {
		if (pixel->radiances[i + 18] != Constants::FILL_VALUE_DOUBLE) {
			size_t xIndex = i < 6 ? 0 : 1;
			size_t yIndex = i % 6;
			sum3 += angularWeights(xIndex, yIndex) * (sdrs[i] - rAng[i]) * (sdrs[i] - rAng[i]);
		}
	}
	return (1 - totalAngularWeight) * sum1 / sum2 + totalAngularWeight * sum3 / sum4;
}

double ErrorMetric::computeNdvi(const Pixel& p) {
	double l1 = p.radiances[ndviIndices[0] - 1];
	double l2 = p.radiances[ndviIndices[1] - 1];
	double f1 = p.solarIrradiances[ndviIndices[0] - 1];
	double f2 = p.solarIrradiances[ndviIndices[1] - 1];

	if (l1 == Constants::FILL_VALUE_DOUBLE || l2 == Constants::FILL_VALUE_DOUBLE || f1 == Constants::FILL_VALUE_DOUBLE || f2 == Constants::FILL_VALUE_DOUBLE) {
		return 0.5;
	}

	return ((l2 / f2) - (l1 / f1)) / ((l2 / f2) + (l1 / f1));
}
