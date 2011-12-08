/*
 * ErrorMetric.cpp
 *
 *  Created on: 13.10.2011
 *      Author: Thomas Storm, Ralf Quast
 */

#include <algorithm>
#include <cmath>

#include "ErrorMetric.h"

static const double PI = 3.14159265358979323846;
static const double RADIAN = PI / 180.0;

ErrorMetric::ErrorMetric(const Context& context) :
		lutOlcRatm((LookupTable<double>&) context.getObject("OLC_R_atm")),
		lutSlnRatm((LookupTable<double>&) context.getObject("SLN_R_atm")),
		lutSloRatm((LookupTable<double>&) context.getObject("SLO_R_atm")),
		lutT((LookupTable<double>&) context.getObject("t")),
		lutRhoAtm((LookupTable<double>&) context.getObject("rho_atm")),
		lutTotalAngularWeights((LookupTable<double>&) context.getObject("weight_ang_tot")),
		lutD((LookupTable<double>&) context.getObject("D")),
		validMask(30),
		sdrs(30),
		coordinates(10),
		matRatmOlc(40, 18),
		matRatmSln(40, 6),
		matRatmSlo(40, 6),
		matTs(40, 30),
		matTv(40, 30),
		matRho(40, 30),
		diffuseFractions(6),
		lutWeights(lutOlcRatm.getDimensionCount()),
		lutWorkspace(lutOlcRatm.getMatrixWorkspaceSize()),
		pn(10),
		p0(10),
		pe(10),
		u(valarray<double>(10), 10),
		lineMinimizer(this, &ErrorMetric::computeRss8, pn, u) {
	const AuxdataProvider& cpAuxdataProvider = (AuxdataProvider&) context.getObject(Constants::AUX_ID_SYCPAX);
	const AuxdataProvider& rtAuxdataProvider = (AuxdataProvider&) context.getObject(Constants::AUX_ID_SYRTAX);

	rtAuxdataProvider.getVectorDouble("C_O3", cO3);
	cpAuxdataProvider.getDouble("T550_ini", initialAot);
	cpAuxdataProvider.getVectorDouble("v_ini", initialNus);
	cpAuxdataProvider.getVectorDouble("w_ini", initialOmegas);
	cpAuxdataProvider.getDouble("gamma", gamma);
	cpAuxdataProvider.getVectorShort("NDV_channel", ndviIndices);
	cpAuxdataProvider.getVectorDouble("R_veg", vegetationModel);
	cpAuxdataProvider.getVectorDouble("R_soil", soilModel);
	cpAuxdataProvider.getVectorDouble("weight_spec", spectralWeights);
	cpAuxdataProvider.getMatrixDouble("weight_ang", angularWeights);
}

ErrorMetric::~ErrorMetric() {
}

bool ErrorMetric::findMinimum(Pixel& p) {
	setPixel(p);

	if (doOLC || doSLS) {
		Bracket bracket;
		bracket.lowerX = 0.0;
		bracket.minimumX = initialAot;
		bracket.upperX = 3.0;
		bracket.lowerF = getValue(0.0);
		bracket.minimumF = getValue(initialAot);
		bracket.upperF = getValue(3.0);

		const bool success = Min::brent(*this, bracket, ACCURACY_GOAL) && bracket.minimumX >= 0.0 && bracket.minimumX <= 3.0;

		if (doOLC) {
			p.c1 = pn[0];
			p.c2 = pn[1];
		}
		if (doSLS) {
			p.nus[0] = pn[2];
			p.nus[1] = pn[3];
			for (size_t i = 0; i < 6; i++) {
				p.omegas[i] = pn[i + 4];
			}
		}
		p.aot = bracket.minimumX;
		p.errorMetric = bracket.minimumF;

		return success;
	}

	return false;
}

double ErrorMetric::computeErrorSurfaceCurvature(const Pixel& p) {
	setPixel(p);

	const double x0 = p.aot;
	const double y0 = p.errorMetric;
	const double y1 = getValue(0.9 * x0);
	const double y2 = getValue(1.1 * x0);

	const double a = (0.1 * y1 - 0.2 * y0 + 0.1 * y2) / (0.002 * x0 * x0);

	return a;
}

double ErrorMetric::getValue(double x) {
	setAerosolOpticalThickness(x);

	pn[0] = 0.5;
	pn[1] = 0.5;
	pn[2] = initialNus[0];
	pn[3] = initialNus[1];
	for (size_t i = 0; i < 6; i++) {
		pn[i + 4] = initialOmegas[i];
	}

	if (doOLC) {
		MultiMin::chol2D(pn, p0, u, sdrs, 0, 18, validMask, spectralWeights, vegetationModel, soilModel);
	}
	if (doSLS) {
		for (size_t i = 2; i < 10; i++) {
			u[i][i] = 1.0;
			for (size_t j = 2; j < i; j++) {
				u[i][j] = u[j][i] = 0.0;
			}
		}
		MultiMin::powell(this, &ErrorMetric::computeRss8, lineMinimizer, 2, 10, pn, p0, pe, u, ACCURACY_GOAL, 100);
	}

	return computeRss10(pn);
}

double ErrorMetric::computeNdvi(const Pixel& p) const {
	double l1 = p.radiances[ndviIndices[0] - 1];
	double l2 = p.radiances[ndviIndices[1] - 1];
	double f1 = p.solarIrradiances[ndviIndices[0] - 1];
	double f2 = p.solarIrradiances[ndviIndices[1] - 1];

	if (l1 == Constants::FILL_VALUE_DOUBLE || l2 == Constants::FILL_VALUE_DOUBLE
			|| f1 == Constants::FILL_VALUE_DOUBLE
			|| f2 == Constants::FILL_VALUE_DOUBLE) {
		return 0.5;
	}

	return ((l2 / f2) - (l1 / f1)) / ((l2 / f2) + (l1 / f1));
}

void ErrorMetric::setPixel(const Pixel& p) {
	double sum2 = 0.0;
	double sum8 = 0.0;
	unsigned olcCount = 0;
	unsigned slnCount = 0;
	unsigned slsCount = 0;

	for (size_t i = 0; i < 18; i++) {
		validMask[i] = p.radiances[i] != Constants::FILL_VALUE_DOUBLE;
		if (validMask[i]) {
			sum2 += spectralWeights[i];
			if (i < 18) {
				olcCount++;
			} else {
				slnCount++;
			}
		}
	}
	for (size_t o = 0; o < 2; o++) {
		for (size_t j = 0; j < 6; j++) {
			const int i = 18 + 6 * o + j;
			validMask[i] = p.radiances[i] != Constants::FILL_VALUE_DOUBLE;
			if (validMask[i]) {
				sum8 += angularWeights(o, j);
				slsCount++;
			}
		}
	}
	this->sum2 = sum2;
	this->sum8 = sum8;
	this->doOLC = olcCount >= 12;
	this->doSLN = slnCount >= 1;
	this->doSLS = slsCount >= 11;

	const double ndvi = computeNdvi(p);
	totalAngularWeight = lutTotalAngularWeights.getScalar(&ndvi, lutWeights, lutWorkspace);

	pixel = &p;
}

double ErrorMetric::computeRss2(valarray<double>& x) {
	double sum = 0.0;
	if (doOLC) {
		for (size_t i = 0; i < 18; i++) {
			if (validMask[i]) {
				const double rSpec = x[0] * vegetationModel[i] + x[1] * soilModel[i];
				sum += spectralWeights[i] * square(sdrs[i] - rSpec);
			}
		}
	}
	return sum;
}

double ErrorMetric::computeRss8(valarray<double>& x) {
	double sum = 0.0;
	if (doSLS) {
		for (size_t o = 0; o < 2; o++) {
			for (size_t j = 0; j < 6; j++) {
				const int i = 18 + 6 * o + j;
				if (validMask[i]) {
					const double d = diffuseFractions[j];
					const double nu = x[o + 2];
					const double omega = x[j + 4];
					const double g = (1.0 - gamma) * omega;
					const double rAng = (1.0 - d) * nu * omega + (gamma * omega) / (1.0 - g) * (d + g * (1.0 - d));

					sum += angularWeights(o, j) * square(sdrs[i] - rAng);
				}
			}
		}
	}
	return sum;
}

static double ozoneTransmission(double cO3, double sza, double vza, double nO3) {
	using std::cos;
	using std::exp;

	// Eq. 2-2
	const double m = 0.5 * (1.0 / cos(sza * RADIAN) + 1.0 / cos(vza * RADIAN));
	const double tO3 = exp(-m * nO3 * cO3);

	return tO3;
}

static double toaReflectance(double ltoa, double f0, double sza) {
	using std::cos;

	// Eq. 2-1
	return (PI * ltoa) / (f0 * cos(sza * RADIAN));
}

static double surfaceReflectance(double rtoa, double ratm, double ts, double tv,
		double rho, double tO3) {
	// Eq. 2-3
	const double f = (rtoa - tO3 * ratm) / (tO3 * ts * tv);
	const double rboa = f / (1.0 + rho * f);

	return rboa;
}

void ErrorMetric::setAerosolOpticalThickness(double aot) {
	using std::abs;

	const size_t amin = pixel->aerosolModel;

	coordinates[0] = abs(pixel->saa - pixel->vaaOlc);
	coordinates[1] = pixel->sza;
	coordinates[2] = pixel->vzaOlc;
	coordinates[3] = pixel->airPressure;
	coordinates[4] = pixel->waterVapour;
	coordinates[5] = aot;

	lutRhoAtm.getMatrix(&coordinates[3], matRho, lutWeights, lutWorkspace);

	coordinates[6] = coordinates[1]; // SZA
	coordinates[7] = coordinates[3]; // air pressure
	coordinates[8] = coordinates[4]; // water vapour
	coordinates[9] = coordinates[5]; // aerosol

	lutT.getMatrix(&coordinates[6], matTs, lutWeights, lutWorkspace);

	if (doOLC) {
		lutOlcRatm.getMatrix(&coordinates[0], matRatmOlc, lutWeights, lutWorkspace);
		lutT.getMatrix(&coordinates[2], matTv, lutWeights, lutWorkspace);

		for (size_t b = 0; b < 18; b++) {
			if (validMask[b]) {
				// Eq. 2-1
				const double rtoa = toaReflectance(pixel->radiances[b], pixel->solarIrradiances[b], pixel->sza);
				// Eq. 2-2
				const double tO3 = ozoneTransmission(cO3[b], pixel->sza, pixel->vzaOlc, pixel->ozone);
				// Eq. 2-3
				const double ratm = matRatmOlc(amin - 1, b);

				const double ts = matTs(amin - 1, b);
				const double tv = matTv(amin - 1, b);
				const double rho = matRho(amin - 1, b);
				const double sdr = surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);

				sdrs[b] = sdr;
			}
		}
	}

	if (doSLN || doSLS) {
		coordinates[0] = abs(pixel->saa - pixel->vaaSln);
		coordinates[2] = pixel->vzaSln;

		lutSlnRatm.getMatrix(&coordinates[0], matRatmSln, lutWeights, lutWorkspace);
		lutT.getMatrix(&coordinates[2], matTv, lutWeights, lutWorkspace);

		for (size_t b = 18; b < 24; b++) {
			if (validMask[b]) {
				// Eq. 2-1
				const double rtoa = toaReflectance(pixel->radiances[b], pixel->solarIrradiances[b], pixel->sza);
				// Eq. 2-2
				const double tO3 = ozoneTransmission(cO3[b], pixel->sza, pixel->vzaOlc, pixel->ozone);
				// Eq. 2-3
				const double ratm = matRatmSln(amin - 1, b - 18);

				const double ts = matTs(amin - 1, b);
				const double tv = matTv(amin - 1, b);
				const double rho = matRho(amin - 1, b);
				const double sdr = surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);

				sdrs[b] = sdr;
			}
		}
	}

	if (doSLS) {
		coordinates[0] = abs(pixel->saa - pixel->vaaSlo);
		coordinates[2] = pixel->vzaSlo;

		lutSloRatm.getMatrix(&coordinates[0], matRatmSlo, lutWeights, lutWorkspace);
		lutT.getMatrix(&coordinates[2], matTv, lutWeights, lutWorkspace);

		for (size_t b = 24; b < 30; b++) {
			if (validMask[b]) {
				// Eq. 2-1
				const double rtoa = toaReflectance(pixel->radiances[b], pixel->solarIrradiances[b], pixel->sza);
				// Eq. 2-2
				const double tO3 = ozoneTransmission(cO3[b], pixel->sza, pixel->vzaOlc, pixel->ozone);
				// Eq. 2-3
				const double ratm = matRatmSlo(amin - 1, b - 24);

				const double ts = matTs(amin - 1, b);
				const double tv = matTv(amin - 1, b);
				const double rho = matRho(amin - 1, b);
				const double sdr = surfaceReflectance(rtoa, ratm, ts, tv, rho, tO3);

				sdrs[b] = sdr;
			}
		}

		coordinates[0] = pixel->sza;
		coordinates[1] = pixel->airPressure;
		coordinates[2] = aot;
		coordinates[3] = amin;

		lutD.getVector(&coordinates[0], diffuseFractions, lutWeights, lutWorkspace);
	}
}
