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
		context(context),
		lutOlcRatm((MatrixLookupTable<double>&) context.getObject("OLC_R_atm")),
		lutSlnRatm((MatrixLookupTable<double>&) context.getObject("SLN_R_atm")),
		lutSloRatm((MatrixLookupTable<double>&) context.getObject("SLO_R_atm")),
		lutT((MatrixLookupTable<double>&) context.getObject("t")),
		lutRhoAtm((MatrixLookupTable<double>&) context.getObject("rho_atm")),
		lutTotalAngularWeights((ScalarLookupTable<double>&) context.getObject("weight_ang_tot")),
		lutD((VectorLookupTable<double>&) context.getObject("D")),
		configurationAuxdata((AuxdataProvider&) context.getObject(Constants::AUX_ID_SYCPAX)),
		gamma(configurationAuxdata.getDouble("gamma")),
		ndviIndices(configurationAuxdata.getVectorShort("NDV_channel")),
		vegetationModel(configurationAuxdata.getVectorDouble("R_veg")),
		soilModel(configurationAuxdata.getVectorDouble("R_soil")),
		spectralWeights(configurationAuxdata.getVectorDouble("weight_spec")),
		angularWeights(configurationAuxdata.getMatrixDouble("weight_ang")),
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
		lutWorkspace(lutOlcRatm.getWorkspaceSize()),
		pn(10),
		p0(10),
		pe(10),
		u(valarray<double>(10), 10),
		lineMinimizer2(this, &ErrorMetric::computeRss2, pn, u),
		lineMinimizer8(this, &ErrorMetric::computeRss8, pn, u) {
}

ErrorMetric::~ErrorMetric() {
}

bool ErrorMetric::findMinimum(Pixel& p) {
	setPixel(p);

	if (doOLC || doSLS) {
		Bracket bracket;
		bracket.lowerX = 0.0;
		bracket.minimumX = p.tau550;
		bracket.upperX = 2.0;
		bracket.lowerF = getValue(0.0);
		bracket.minimumF = getValue(0.1);
		bracket.upperF = getValue(2.0);

		const bool success = Min::brent(*this, bracket, 1.0e-4);

		if (doOLC) {
			p.c1 = pn[0];
			p.c2 = pn[1];
		}
		if (doSLS) {
			p.nu[0] = pn[2];
			p.nu[1] = pn[3];
			for (size_t i = 0; i < 6; i++) {
				p.omega[i] = pn[i + 4];
			}
		}
		p.tau550 = bracket.minimumX;
		p.E2 = bracket.minimumF;

		return success;
	}

	return false;
}

double ErrorMetric::computeErrorSurfaceCurvature(const Pixel& p) {
	setPixel(p);

	const double a = getValue(0.8 * p.tau550);
	const double b = getValue(0.6 * p.tau550);

	return 25.0 * (p.E2 - 2.0 * a + b) / (2.0 * p.E2 * p.E2);
}

double ErrorMetric::getValue(double x) {
	setAerosolOpticalThickness(x);

	pn[0] = pixel->c1;
	pn[1] = pixel->c2;
	pn[2] = pixel->nu[0];
	pn[3] = pixel->nu[1];
	for (size_t i = 0; i < 6; i++) {
		pn[i + 4] = pixel->omega[i];
	}

	if (doOLC) {
		if (true) {
			MultiMin::chol2D(pn, p0, u, pixel->sdrs, 0, 18, validMask, spectralWeights, vegetationModel, soilModel);
		} else {
			for (size_t i = 0; i < 2; i++) {
				u[i][i] = 1.0;
				for (size_t j = 0; j < i; j++) {
					u[i][j] = u[j][i] = 0.0;
				}
			}
			MultiMin::powell(this, &ErrorMetric::computeRss2, lineMinimizer2, 0, 2, pn, p0, pe, u, 1.0e-4, 100);
		}
	}
	if (doSLS) {
		for (size_t i = 2; i < 10; i++) {
			u[i][i] = 1.0;
			for (size_t j = 2; j < i; j++) {
				u[i][j] = u[j][i] = 0.0;
			}
		}
		MultiMin::powell(this, &ErrorMetric::computeRss2, lineMinimizer8, 2, 10, pn, p0, pe, u, 1.0e-4, 100);
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
	unsigned slsCount = 0;

#pragma omp parallel for reduction(+ : sum2, olcCount)
	for (size_t i = 0; i < 18; i++) {
		validMask[i] = p.radiances[i] != Constants::FILL_VALUE_DOUBLE;
		if (validMask[i]) {
			sum2 += spectralWeights[i];
			olcCount++;
		}
	}
#pragma omp parallel for reduction(+ : sum8, slsCount)
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
	// TODO - revert
	this->doSLS = false; //slsCount >= 8;

	const double ndvi = computeNdvi(p);
	totalAngularWeight = lutTotalAngularWeights.getValue(&ndvi);

	pixel = &p;
}

double ErrorMetric::computeRss2(valarray<double>& x) {
	double sum = 0.0;
	if (doOLC) {
#pragma omp parallel for reduction(+ : sum)
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
#pragma omp parallel for reduction(+ : sum)
		for (size_t o = 0; o < 2; o++) {
			for (size_t j = 0; j < 6; j++) {
				const int i = 18 + 6 * o + j;
				if (validMask[i]) {
					const double d = diffuseFractions[j];
					const double nu = x[o + 2];
					const double omega = x[j + 4];
					const double g = (1.0 - gamma) * omega;
					// TODO - check
					const double rAng = (1.0 - d) * nu * omega + (gamma * omega) / (1.0 - g) * (d + g * (1.0 - d));

					sum += angularWeights(o, j) * square(sdrs[i] - rAng);
				}
			}
		}
	}
	return sum;
}

static double ozoneTransmission(double cO3, double sza, double vza,
		double nO3) {
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

void ErrorMetric::setAerosolOpticalThickness(double tau550) {
	using std::abs;

	const size_t amin = pixel->amin;

	coordinates[0] = abs(pixel->saa - pixel->vaaOlc);
	coordinates[1] = pixel->sza;
	coordinates[2] = pixel->vzaOlc;
	coordinates[3] = pixel->airPressure;
	coordinates[4] = pixel->waterVapour;
	coordinates[5] = tau550;

	lutRhoAtm.getValues(&coordinates[3], matRho, lutWeights, lutWorkspace);

	coordinates[6] = coordinates[1]; // SZA
	coordinates[7] = coordinates[3]; // air pressure
	coordinates[8] = coordinates[4]; // water vapour
	coordinates[9] = coordinates[5]; // aerosol

	lutT.getValues(&coordinates[6], matTs, lutWeights, lutWorkspace);

	if (doOLC) {
		lutOlcRatm.getValues(&coordinates[0], matRatmOlc, lutWeights, lutWorkspace);
		lutT.getValues(&coordinates[2], matTv, lutWeights, lutWorkspace);

#pragma omp parallel for
		for (size_t b = 0; b < 18; b++) {
			if (validMask[b]) {
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
	}
	if (doSLS) {
		coordinates[0] = abs(pixel->saa - pixel->vaaSln);
		coordinates[2] = pixel->vzaSln;

		lutSlnRatm.getValues(&coordinates[0], matRatmSln, lutWeights, lutWorkspace);
		lutT.getValues(&coordinates[2], matTv, lutWeights, lutWorkspace);

#pragma omp parallel for
		for (size_t b = 18; b < 24; b++) {
			if (validMask[b]) {
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

		coordinates[0] = abs(pixel->saa - pixel->vaaSlo);
		coordinates[2] = pixel->vzaSlo;

		lutSloRatm.getValues(&coordinates[0], matRatmSlo, lutWeights, lutWorkspace);
		lutT.getValues(&coordinates[2], matTv, lutWeights, lutWorkspace);

#pragma omp parallel for
		for (size_t b = 24; b < 30; b++) {
			if (validMask[b]) {
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

		coordinates[0] = pixel->sza;
		coordinates[1] = pixel->airPressure;
		coordinates[2] = tau550;
		coordinates[3] = amin;

		lutD.getValues(&coordinates[0], diffuseFractions);
	}
}
