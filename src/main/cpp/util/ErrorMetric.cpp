/*
 * ErrorMetric.cpp
 *
 *  Created on: 13.10.2011
 *      Author: thomasstorm
 */

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
		configurationAuxdata((AuxdataProvider&) context.getObject(Constants::AUXDATA_CONFIGURATION_ID)),
		gamma(configurationAuxdata.getDouble("gamma")),
		ndviIndices(configurationAuxdata.getVectorShort("NDV_channel")),
		spectralWeights(configurationAuxdata.getVectorDouble("weight_spec")),
		vegetationSpectrum(configurationAuxdata.getVectorDouble("R_veg")),
		soilReflectance(configurationAuxdata.getVectorDouble("R_soil")),
		angularWeights(configurationAuxdata.getMatrixDouble("weight_ang")),
		sdrs(30),
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
		lineMinimizer2(this, &ErrorMetric::computeRss2, pn, u),
		lineMinimizer8(this, &ErrorMetric::computeRss8, pn, u)
{
}

double ErrorMetric::getValue(double x) {
	applyAtmosphericCorrection(x);
	for (size_t i = 0; i < 10; i++) {
        u[i][i] = 1.0;
        for (size_t j = 0; j < i; j++) {
            u[i][j] = u[j][i] = 0.0;
        }
	}
    if (doOLC) {
        pn[0] = pixel->c1;
        pn[1] = pixel->c2;
        MultiMin::powell(this, &ErrorMetric::computeRss2, lineMinimizer2, 0, 2, pn, p0, pe, u, 1.0e-4, 100);
    }
    if (doSLS) {
        pn[2] = pixel->nu[0];
        pn[3] = pixel->nu[1];
        for (size_t i = 0; i < 6; i++) {
            pn[i + 4] = pixel->omega[i];
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
    
	if (l1 == Constants::FILL_VALUE_DOUBLE || 
        l2 == Constants::FILL_VALUE_DOUBLE || 
        f1 == Constants::FILL_VALUE_DOUBLE || 
        f2 == Constants::FILL_VALUE_DOUBLE) {
		return 0.5;
	}
    
	return ((l2 / f2) - (l1 / f1)) / ((l2 / f2) + (l1 / f1));
}

void ErrorMetric::setPixel(const Pixel& p) {
	sum2 = 0.0;
	sum8 = 0.0;

    unsigned olcCount = 0;
    unsigned slsCount = 0;
	for (size_t i = 0; i < 30; i++) {
		if (p.radiances[i] != Constants::FILL_VALUE_DOUBLE) {
			sum2 += spectralWeights[i];
            if (i < 18) {
                olcCount++;
            } else {
                slsCount++;
            }
		}
	}
	for (size_t o = 0, i = 18; o < 2; o++) {
        for (size_t j = 0; j < 6; j++, i++) {
            if (p.radiances[i] != Constants::FILL_VALUE_DOUBLE) {
                sum8 += angularWeights(o, j);
            }
        }
    }
    doOLC = olcCount >= 12;
    doSLS = slsCount >= 8;
    
	const double ndvi = computeNdvi(p);
	totalAngularWeight = lutTotalAngularWeights.getValue(&ndvi);

    pixel = &p;
}


double ErrorMetric::computeRss2(valarray<double>& x) {
	double sum = 0.0;
    if (doOLC) {
#pragma omp parallel for reduction(+ : sum)
        for (size_t i = 0; i < 30; i++) {
            if (pixel->radiances[i] != Constants::FILL_VALUE_DOUBLE) {
                const double rSpec = x[0] * vegetationSpectrum[i] + x[1] * soilReflectance[i];
                sum += spectralWeights[i] * (sdrs[i] - rSpec) * (sdrs[i] - rSpec);
            }
        }
    }
	return sum;
}

double ErrorMetric::computeRss8(valarray<double>& x) {
	double sum = 0.0;
    if (doSLS) {
        for (size_t o = 0, i = 18; i < 2; o++) {
            for (size_t j = 0; j < 6; j++, i++) {
                if (pixel->radiances[i] != Constants::FILL_VALUE_DOUBLE) {            
                    const double d = diffuseFraction[j];
                    const double nu = x[o + 2];
                    const double omega = x[j + 4];
                    const double g = (1 - gamma) * omega;
                    const double rAng = (1 - d) * nu * omega + (gamma * omega) / (1 - g) * (d + g * (1 - d));
                    
                    sum += angularWeights(o, j) * (sdrs[i] - rAng) * (sdrs[i] - rAng);
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

static double surfaceReflectance(double rtoa, double ratm, double ts, double tv, double rho, double tO3) {
	// Eq. 2-3
	const double f = (rtoa - tO3 * ratm) / (tO3 * ts * tv);
	const double rboa = f / (1.0 + rho * f);

	return rboa;
}

void ErrorMetric::applyAtmosphericCorrection(double tau550) {
	using std::abs;

	const size_t amin = pixel->amin;

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

#pragma omp parallel for
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

#pragma omp parallel for
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

#pragma omp parallel for
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
