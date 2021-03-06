/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#include <cmath>

#include "../core/LookupTable.h"
#include "../core/TiePointInterpolator.h"

#include "Aco.h"

using std::abs;
using std::cos;
using std::exp;
using std::sqrt;

using boost::lexical_cast;

Aco::Aco() :
		BasicModule("ACO") {
}

Aco::~Aco() {
}

void Aco::start(Context& context) {
	getLookupTable(context, Constants::AUX_ID_SYRT, "OLC_R_atm");
	getLookupTable(context, Constants::AUX_ID_SYRT, "SLN_R_atm");
	getLookupTable(context, Constants::AUX_ID_SYRT, "SLO_R_atm");
	getLookupTable(context, Constants::AUX_ID_SYRT, "t");
	getLookupTable(context, Constants::AUX_ID_SYRT, "rho_atm");
	getAuxdataProvider(context, Constants::AUX_ID_SYRT);

	Segment& collocatedSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const SegmentDescriptor& targetSegmentDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
	for (size_t i = 1; i <= 30; i++) {
		addAccessor(context, collocatedSegment, targetSegmentDescriptor.getVariableDescriptor("SDR_" + lexical_cast<string>(i)));
		addAccessor(context, collocatedSegment, targetSegmentDescriptor.getVariableDescriptor("SDR_" + lexical_cast<string>(i) + "_er"));
	}
	if (true) { // TODO - use breakpoint settings from job order file
		collocatedSegment.addVariable("SAA", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("SZA", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("VAA", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("VZA", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("SLN_VAA", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("SLN_VZA", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("SLO_VAA", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("SLO_VZA", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("O3", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("WV", Constants::TYPE_DOUBLE, 1.0, 0.0);
		collocatedSegment.addVariable("AP", Constants::TYPE_DOUBLE, 1.0, 0.0);
	}
}

void Aco::process(Context& context) {
    const Segment& tpSegmentOlc = context.getSegment(Constants::SEGMENT_OLC_TP);
    const Segment& tpSegmentSln = context.getSegment(Constants::SEGMENT_SLN_TP);
    const Segment& tpSegmentSlo = context.getSegment(Constants::SEGMENT_SLO_TP);

	const Accessor& tpVzaOlc = tpSegmentOlc.getAccessor("OLC_VZA");
	const Accessor& tpVaaOlc = tpSegmentOlc.getAccessor("OLC_VAA");
	const Accessor& tpVzaSln = tpSegmentSln.getAccessor("SLN_VZA");
	const Accessor& tpVaaSln = tpSegmentSln.getAccessor("SLN_VAA");
	const Accessor& tpVzaSlo = tpSegmentSlo.getAccessor("SLO_VZA");
	const Accessor& tpVaaSlo = tpSegmentSlo.getAccessor("SLO_VAA");
	const Accessor& tpSzaOlc = tpSegmentOlc.getAccessor("SZA");
	const Accessor& tpSaaOlc = tpSegmentOlc.getAccessor("SAA");
	const Accessor& tpLatOlc = tpSegmentOlc.getAccessor("OLC_TP_lat");
	const Accessor& tpLonOlc = tpSegmentOlc.getAccessor("OLC_TP_lon");
	const Accessor& tpLatSln = tpSegmentSln.getAccessor("SLN_TP_lat");
	const Accessor& tpLonSln = tpSegmentSln.getAccessor("SLN_TP_lon");
	const Accessor& tpLatSlo = tpSegmentSlo.getAccessor("SLO_TP_lat");
	const Accessor& tpLonSlo = tpSegmentSlo.getAccessor("SLO_TP_lon");
	const Accessor& tpOzone = tpSegmentOlc.getAccessor("ozone");
	const Accessor& tpAirPressure = tpSegmentOlc.getAccessor("air_pressure");

	const valarray<double> tpLonsOlc = tpLonOlc.getDoubles();
	const valarray<double> tpLatsOlc = tpLatOlc.getDoubles();
	const valarray<double> tpLonsSln = tpLonSln.getDoubles();
	const valarray<double> tpLatsSln = tpLatSln.getDoubles();
	const valarray<double> tpLonsSlo = tpLonSlo.getDoubles();
	const valarray<double> tpLatsSlo = tpLatSlo.getDoubles();
	const valarray<double> tpSzasOlc = tpSzaOlc.getDoubles();
	const valarray<double> tpSaasOlc = tpSaaOlc.getDoubles();
	const valarray<double> tpVzasOlc = tpVzaOlc.getDoubles();
	const valarray<double> tpVaasOlc = tpVaaOlc.getDoubles();
	const valarray<double> tpVzasSln = tpVzaSln.getDoubles();
	const valarray<double> tpVaasSln = tpVaaSln.getDoubles();
	const valarray<double> tpVzasSlo = tpVzaSlo.getDoubles();
	const valarray<double> tpVaasSlo = tpVaaSlo.getDoubles();
	const valarray<double> tpOzones = tpOzone.getDoubles();
	const valarray<double> tpAirPressures = tpAirPressure.getDoubles();
	valarray<double> tpWaterVapours;
	if (tpSegmentOlc.hasVariable("water_vapour")) {
		copy(tpSegmentOlc.getAccessor("water_vapour").getDoubles(), tpWaterVapours);
	}

	const TiePointInterpolator<double> tpiOlc = TiePointInterpolator<double>(tpLonsOlc, tpLatsOlc);
	const TiePointInterpolator<double> tpiSln = TiePointInterpolator<double>(tpLonsSln, tpLatsSln);
	const TiePointInterpolator<double> tpiSlo = TiePointInterpolator<double>(tpLonsSlo, tpLatsSlo);

	const Segment& geoSegment = context.getSegment(Constants::SEGMENT_GEO);
	const Segment& collocatedSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Segment& olcInfoSegment = context.getSegment(Constants::SEGMENT_OLC_INFO);
	const Segment& slnInfoSegment = context.getSegment(Constants::SEGMENT_SLN_INFO);
	const Segment& sloInfoSegment = context.getSegment(Constants::SEGMENT_SLO_INFO);

	vector<Accessor*> ltoaAccessors;
	for (size_t i = 1; i <= 30; i++) {
		ltoaAccessors.push_back(&collocatedSegment.getAccessor("L_" + lexical_cast<string>(i)));
	}
	vector<Accessor*> ltoaErrAccessors;
	for (size_t i = 1; i <= 18; i++) {
		ltoaErrAccessors.push_back(&collocatedSegment.getAccessor("L_" + lexical_cast<string>(i) + "_er"));
	}
    const Accessor& tau550Accessor = collocatedSegment.getAccessor("T550");
    const Accessor& tau550ErrAccessor = collocatedSegment.getAccessor("T550_er");
    const Accessor& aminAccessor = collocatedSegment.getAccessor("AMIN");
    const Accessor& flagsAccessor = collocatedSegment.getAccessor("SYN_flags");
	const Accessor& latAccessor = geoSegment.getAccessor("latitude");
	const Accessor& lonAccessor = geoSegment.getAccessor("longitude");
	const Accessor& solarIrrOlcAccessor = olcInfoSegment.getAccessor("solar_irradiance");

	vector<Accessor*> solarIrrSlnAccessors;
	for (size_t i = 1; i <= 6; i++) {
		solarIrrSlnAccessors.push_back(&slnInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(i)));
	}
	vector<Accessor*> solarIrrSloAccessors;
	for (size_t i = 1; i <= 6; i++) {
		solarIrrSloAccessors.push_back(&sloInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(i)));
	}

	const Grid& collocatedGrid = collocatedSegment.getGrid();
	const Grid& olcInfoGrid = olcInfoSegment.getGrid();
	const Grid& slnInfoGrid = slnInfoSegment.getGrid();
	const Grid& sloInfoGrid = sloInfoSegment.getGrid();
	const Grid& geoGrid = geoSegment.getGrid();

	vector<Accessor*> sdrAccessors;
	for (size_t i = 1; i <= 30; i++) {
		sdrAccessors.push_back(&collocatedSegment.getAccessor("SDR_" + lexical_cast<string>(i)));
	}
	vector<Accessor*> errAccessors;
	for (size_t i = 1; i <= 30; i++) {
		errAccessors.push_back(&collocatedSegment.getAccessor("SDR_" + lexical_cast<string>(i) + "_er"));
	}

	const LookupTable<double>& lutOlcRatm = getLookupTable(context, Constants::AUX_ID_SYRT, "OLC_R_atm");
	const LookupTable<double>& lutSlnRatm = getLookupTable(context, Constants::AUX_ID_SYRT, "SLN_R_atm");
	const LookupTable<double>& lutSloRatm = getLookupTable(context, Constants::AUX_ID_SYRT, "SLO_R_atm");
	const LookupTable<double>& lutT = getLookupTable(context, Constants::AUX_ID_SYRT, "t");
	const LookupTable<double>& lutRhoAtm = getLookupTable(context, Constants::AUX_ID_SYRT, "rho_atm");
	const AuxdataProvider& auxdataProvider = getAuxdataProvider(context, Constants::AUX_ID_SYRT);

	valarray<double> cO3;
	auxdataProvider.getVectorDouble("C_O3", cO3);
	double delta3;
	auxdataProvider.getDouble("delta_rt", delta3);

	context.getLogging().progress("Processing segment '" + collocatedSegment.toString() + "'", getId());
	const long firstL = context.getFirstComputableL(collocatedSegment, *this);
	context.getLogging().debug("Segment [" + collocatedSegment.toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
	const long lastL = context.getLastComputableL(collocatedSegment, *this);
	context.getLogging().debug("Segment [" + collocatedSegment.toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) > 40100
#pragma omp parallel for
#endif
	for (long l = firstL; l <= lastL; l++) {
		valarray<double> coordinates(7);
		valarray<double> tpiWeights(1);
		valarray<size_t> tpiIndexes(1);

		valarray<double> matRatmOlc(18);
		valarray<double> matRatmSln(6);
		valarray<double> matRatmSlo(6);
		valarray<double> matTv(30);
		valarray<double> matRho(30);

		valarray<double> f(lutOlcRatm.getDimensionCount());
		valarray<double> w;

		valarray<double> tv(30);
		valarray<double> tO3(30);

		valarray<double> rboa(30);
		valarray<double> rtoa(30);

		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l) + " ...", getId());

		for (long k = collocatedGrid.getMinK(); k <= collocatedGrid.getMaxK(); k++) {
			for (long m = collocatedGrid.getMinM(); m <= collocatedGrid.getMaxM(); m++) {
				const size_t geoIndex = geoGrid.getIndex(k, l, m);
				const size_t i = collocatedGrid.getIndex(k, l, m);

			    for (size_t b = 0; b < sdrAccessors.size(); b++) {
			        sdrAccessors[b]->setFillValue(i);
			        errAccessors[b]->setFillValue(i);
			    }

			    const uint16_t synFlags = flagsAccessor.getUShort(i);
			    if (!isSet(synFlags, Constants::SY2_LAND_FLAG) || isSet(synFlags, Constants::SY2_CLOUD_FLAG)) {
			    	continue;
			    }

				const double tau550 = tau550Accessor.getDouble(i);
				const double tau550Err = tau550ErrAccessor.getDouble(i);
				const uint8_t amin = aminAccessor.getUByte(i);
				if (amin == 0) {
					continue;
				}

				tpiOlc.prepare(lonAccessor.getDouble(geoIndex), latAccessor.getDouble(geoIndex), tpiWeights, tpiIndexes);
				const double nO3 = siToDu(tpiOlc.interpolate(tpOzones, tpiWeights, tpiIndexes));
				const double p = tpiOlc.interpolate(tpAirPressures, tpiWeights, tpiIndexes);
				const double wv = tpWaterVapours.size() != 0 ? tpiOlc.interpolate(tpWaterVapours, tpiWeights, tpiIndexes) : 2.0;

				/*
				 * Surface reflectance for OLC channels
				 */
				const double szaOlc = tpiOlc.interpolate(tpSzasOlc, tpiWeights, tpiIndexes);
				const double saaOlc = tpiOlc.interpolate(tpSaasOlc, tpiWeights, tpiIndexes);
				const double vzaOlc = tpiOlc.interpolate(tpVzasOlc, tpiWeights, tpiIndexes);
				const double vaaOlc = tpiOlc.interpolate(tpVaasOlc, tpiWeights, tpiIndexes);

				if (true) { // TODO - use breakpoint settings from job order file
					collocatedSegment.getAccessor("SAA").setDouble(i, saaOlc);
					collocatedSegment.getAccessor("SZA").setDouble(i, szaOlc);
					collocatedSegment.getAccessor("VAA").setDouble(i, vaaOlc);
					collocatedSegment.getAccessor("VZA").setDouble(i, vzaOlc);
					collocatedSegment.getAccessor("O3").setDouble(i, nO3);
					collocatedSegment.getAccessor("WV").setDouble(i, wv);
					collocatedSegment.getAccessor("AP").setDouble(i, p);
				}

				coordinates[0] = abs(saaOlc - vaaOlc); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaOlc; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = tau550; // aerosol
				coordinates[6] = amin; // aerosol model

				lutOlcRatm.getVector(&coordinates[0], matRatmOlc, f, w);
				lutT.getVector(&coordinates[1], matTv, f, w);
				lutRhoAtm.getVector(&coordinates[3], matRho, f, w);

				for (size_t b = 0; b < 18; b++) {
					if (!ltoaAccessors[b]->isFillValue(i)) {
						// Eq. 2-1
						const double ltoa = ltoaAccessors[b]->getDouble(i);
						const double f0 = solarIrrOlcAccessor.getDouble(olcInfoGrid.getIndex(k, b, m));
						rtoa[b] = toaReflectance(ltoa, f0, szaOlc);

						// Eq. 2-2
						tO3[b] = ozoneTransmission(cO3[b], szaOlc, vzaOlc, nO3);

						// Eq. 2-3
						const double ratm = matRatmOlc[b];
						tv[b] = matTv[b];
						const double rho = matRho[b];
						rboa[b] = surfaceReflectance(rtoa[b], ratm, tv[b], rho, tO3[b]);

						if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
							sdrAccessors[b]->setDouble(i, rboa[b]);
						}
					}
				}

				/*
				 * Surface reflectance for SLN channels
				 */
				tpiSln.prepare(lonAccessor.getDouble(geoIndex), latAccessor.getDouble(geoIndex), tpiWeights, tpiIndexes);

				const double vzaSln = tpiSln.interpolate(tpVzasSln, tpiWeights, tpiIndexes);
				const double vaaSln = tpiSln.interpolate(tpVaasSln, tpiWeights, tpiIndexes);

				if (true) { // TODO - use breakpoint settings from job order file
					collocatedSegment.getAccessor("SLN_VAA").setDouble(i, vaaSln);
					collocatedSegment.getAccessor("SLN_VZA").setDouble(i, vzaSln);
				}

				coordinates[0] = abs(saaOlc - vaaSln); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaSln; // VZA

				lutSlnRatm.getVector(&coordinates[0], matRatmSln, f, w);
				lutT.getVector(&coordinates[1], matTv, f, w);

				for (size_t b = 18; b < 24; b++) {
					if (!ltoaAccessors[b]->isFillValue(i)) {
						// Eq. 2-1
						const double ltoa = ltoaAccessors[b]->getDouble(i);
						const double f0 = solarIrrSlnAccessors[b - 18]->getDouble(slnInfoGrid.getIndex(0, 0, l % 4));
						rtoa[b] = toaReflectance(ltoa, f0, szaOlc);

						// Eq. 2-2
						tO3[b] = ozoneTransmission(cO3[b], szaOlc, vzaSln, nO3);

						// Eq. 2-3
						const double ratm = matRatmSln[b - 18];
						tv[b] = matTv[b];
						const double rho = matRho[b];
						rboa[b] = surfaceReflectance(rtoa[b], ratm, tv[b], rho, tO3[b]);

						if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
							sdrAccessors[b]->setDouble(i, rboa[b]);
						}
					}
				}

				/*
				 * Surface reflectance for SLO channels
				 */
				tpiSlo.prepare(lonAccessor.getDouble(geoIndex), latAccessor.getDouble(geoIndex), tpiWeights, tpiIndexes);

				const double vzaSlo = tpiSlo.interpolate(tpVzasSlo, tpiWeights, tpiIndexes);
				const double vaaSlo = tpiSlo.interpolate(tpVaasSlo, tpiWeights, tpiIndexes);

				if (true) { // TODO - use breakpoint settings from job order file
					collocatedSegment.getAccessor("SLO_VAA").setDouble(i, vaaSlo);
					collocatedSegment.getAccessor("SLO_VZA").setDouble(i, vzaSlo);
				}

				coordinates[0] = abs(saaOlc - vaaSlo); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaSlo; // VZA

				lutSloRatm.getVector(&coordinates[0], matRatmSlo, f, w);
				lutT.getVector(&coordinates[1], matTv, f, w);

				for (size_t b = 24; b < 30; b++) {
					if (!ltoaAccessors[b]->isFillValue(i)) {
						// Eq. 2-1
						const double ltoa = ltoaAccessors[b]->getDouble(i);
						const double f0 = solarIrrSloAccessors[b - 24]->getDouble(sloInfoGrid.getIndex(0, 0, l % 4));
						rtoa[b] = toaReflectance(ltoa, f0, szaOlc);

						// Eq. 2-2
						tO3[b] = ozoneTransmission(cO3[b], szaOlc, vzaSlo, nO3);

						// Eq. 2-3
						const double ratm = matRatmSlo[b - 24];
						tv[b] = matTv[b];
						const double rho = matRho[b];
						rboa[b] = surfaceReflectance(rtoa[b], ratm, tv[b], rho, tO3[b]);

						if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
							sdrAccessors[b]->setDouble(i, rboa[b]);
						}
					}
				}

				/*
				 * Errors for OLC channels
				 */
				coordinates[0] = abs(saaOlc - vaaOlc); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaOlc; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = 0.8 * tau550; // aerosol

				lutOlcRatm.getVector(&coordinates[0], matRatmOlc, f, w);
				lutT.getVector(&coordinates[1], matTv, f, w);
				lutRhoAtm.getVector(&coordinates[3], matRho, f, w);

				for (size_t b = 0; b < 18; b++) {
					if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
						const double ratm = matRatmOlc[b];
						const double tv1 = matTv[b];
						const double rho = matRho[b];
						const double deltaR = rboa[b] - surfaceReflectance(rtoa[b], ratm, tv1, rho, tO3[b]);
						const double deltaTau = 0.2 * tau550;
						const double delta1 = (deltaR / deltaTau) * tau550Err;
						const double ltoaErr = ltoaErrAccessors[b]->getDouble(i);
						const double f0 = solarIrrOlcAccessor.getDouble(olcInfoGrid.getIndex(k, b, m));
						const double delta2 = toaReflectance(ltoaErr, f0, szaOlc) / (tv[b] * tO3[b]);
						const double err = sqrt(delta1 * delta1 + delta2 * delta2 + delta3 * delta3);
						errAccessors[b]->setDouble(i, err);
					}
				}

				/*
				 * Errors for SLN channels
				 */
				coordinates[0] = abs(saaOlc - vaaSln); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaSln; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = 0.8 * tau550; // aerosol

				lutSlnRatm.getVector(&coordinates[0], matRatmSln, f, w);
				lutT.getVector(&coordinates[1], matTv, f, w);

				for (size_t b = 18; b < 24; b++) {
					if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
						const double ratm = matRatmSln[b - 18];
						const double tv1 = matTv[b];
						const double rho = matRho[b];
						const double deltaR = rboa[b] - surfaceReflectance(rtoa[b], ratm, tv1, rho, tO3[b]);
						const double deltaTau = 0.2 * tau550;
						const double delta1 = (deltaR / deltaTau) * tau550Err;
						const double err = sqrt(delta1 * delta1 + delta3 * delta3);
						errAccessors[b]->setDouble(i, err);
					}
				}

				/*
				 * Errors for SLO channels
				 */
				coordinates[0] = abs(saaOlc - vaaSlo); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaSlo; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = 0.8 * tau550; // aerosol

				lutSloRatm.getVector(&coordinates[0], matRatmSlo, f, w);
				lutT.getVector(&coordinates[1], matTv, f, w);

				for (size_t b = 24; b < 30; b++) {
					if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
						if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
							const double ratm = matRatmSlo[b - 24];
							const double tv1 = matTv[b];
							const double rho = matRho[b];
							const double deltaR = rboa[b] - surfaceReflectance(rtoa[b], ratm, tv1, rho, tO3[b]);
							const double deltaTau = 0.2 * tau550;
							const double delta1 = (deltaR / deltaTau) * tau550Err;
							const double err = sqrt(delta1 * delta1 + delta3 * delta3);
							errAccessors[b]->setDouble(i, err);
						}
					}
				}
			}
		}
	}
	context.setLastComputedL(collocatedSegment, *this, lastL);
}

double Aco::siToDu(double nO3) {
	return nO3 * 46698.0;
}

double Aco::ozoneTransmission(double cO3, double sza, double vza, double nO3) {
	// Eq. 2-2
	const double m = 0.5 * (1.0 / cos(sza * D2R) + 1.0 / cos(vza * D2R));
	const double tO3 = exp(-m * (nO3 - 350.0) * cO3);

	return tO3;
}

double Aco::surfaceReflectance(double rtoa, double ratm, double tvs, double rho, double tO3) {
	// Eq. 2-3
	const double f = (rtoa - tO3 * ratm) / (tO3 * tvs);
	const double rboa = f / (1.0 + rho * f);

	return rboa;
}

double Aco::toaReflectance(double ltoa, double f0, double sza) {
	return (PI * ltoa) / (f0 * cos(sza * D2R));
}

