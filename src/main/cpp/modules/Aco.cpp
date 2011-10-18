/*
 * Aco.cpp
 *
 *  Created on: Aug 24, 2011
 *      Author: ralf
 */

#include <cmath>

#include "../util/LookupTableReader.h"
#include "../core/TiePointInterpolator.h"

#include "Aco.h"

using std::abs;
using std::cos;
using std::exp;

Aco::Aco() :
		BasicModule("ACO") {
}

Aco::~Aco() {
}

void Aco::start(Context& context) {
	addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "OLC_R_atm");
	addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLN_R_atm");
	addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLO_R_atm");
	addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "t");
	addMatrixLookupTable(context, "S3__SY_2_SYRTAX.nc", "rho_atm");
	addScalarLookupTable(context, "S3__SY_2_SYRTAX.nc", "C_O3");

	Segment& collocatedSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const SegmentDescriptor& targetSegmentDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
	for (size_t i = 1; i <= 30; i++) {
		addAccessor(context, collocatedSegment, targetSegmentDescriptor.getVariableDescriptor("SDR_" + lexical_cast<string>(i)));
		addAccessor(context, collocatedSegment, targetSegmentDescriptor.getVariableDescriptor("SDR_" + lexical_cast<string>(i) + "_er"));
	}
}

void Aco::stop(Context& context) {

}

void Aco::process(Context& context) {
	const Accessor& tpVzaOlc = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_VZA");
	const Accessor& tpVaaOlc = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_VAA");
	const Accessor& tpVzaSln = context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_VZA");
	const Accessor& tpVaaSln = context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_VAA");
	const Accessor& tpVzaSlo = context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_VZA");
	const Accessor& tpVaaSlo = context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_VAA");
	const Accessor& tpSzaOlc = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("SZA");
	const Accessor& tpSaaOlc = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("SAA");
	const Accessor& tpLatOlc = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lat");
	const Accessor& tpLonOlc = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lon");
	const Accessor& tpLatSln = context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lat");
	const Accessor& tpLonSln = context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lon");
	const Accessor& tpLatSlo = context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lat");
	const Accessor& tpLonSlo = context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lon");

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

	const TiePointInterpolator<double> tpiOlc = TiePointInterpolator<double>(tpLonsOlc, tpLatsOlc);
	const TiePointInterpolator<double> tpiSln = TiePointInterpolator<double>(tpLonsSln, tpLatsSln);
	const TiePointInterpolator<double> tpiSlo = TiePointInterpolator<double>(tpLonsSlo, tpLatsSlo);

	const Segment& collocatedSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Segment& olcInfoSegment = context.getSegment(Constants::SEGMENT_OLC_INFO);
	const Segment& slnInfoSegment = context.getSegment(Constants::SEGMENT_SLN_INFO);
	const Segment& sloInfoSegment = context.getSegment(Constants::SEGMENT_SLO_INFO);

	vector<Accessor*> ltoaAccessors;
	for (size_t i = 1; i <= 30; i++) {
		ltoaAccessors.push_back(&collocatedSegment.getAccessor("L_" + lexical_cast<string>(i)));
	}
	const Accessor& latAccessor = collocatedSegment.getAccessor("latitude");
	const Accessor& lonAccessor = collocatedSegment.getAccessor("longitude");
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

	vector<Accessor*> sdrAccessors;
	for (size_t i = 1; i <= 30; i++) {
		sdrAccessors.push_back(&collocatedSegment.getAccessor("SDR_" + lexical_cast<string>(i)));
	}
	vector<Accessor*> errAccessors;
	for (size_t i = 1; i <= 30; i++) {
		errAccessors.push_back(&collocatedSegment.getAccessor("SDR_" + lexical_cast<string>(i) + "_er"));
	}

	const MatrixLookupTable<double>& lutOlcRatm = (MatrixLookupTable<double>&) context.getObject("OLC_R_atm");
	const MatrixLookupTable<double>& lutSlnRatm = (MatrixLookupTable<double>&) context.getObject("SLN_R_atm");
	const MatrixLookupTable<double>& lutSloRatm = (MatrixLookupTable<double>&) context.getObject("SLO_R_atm");
	const MatrixLookupTable<double>& lutT = (MatrixLookupTable<double>&) context.getObject("t");
	const MatrixLookupTable<double>& lutRhoAtm = (MatrixLookupTable<double>&) context.getObject("rho_atm");
	const ScalarLookupTable<double>& lutCO3 = (ScalarLookupTable<double>&) context.getObject("C_O3");

	context.getLogging().progress("Processing segment '" + collocatedSegment.toString() + "'", getId());

	const long firstL = context.getFirstComputableL(collocatedSegment, *this);
	context.getLogging().debug("Segment [" + collocatedSegment.toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
	const long lastL = context.getLastComputableL(collocatedSegment, *this);
	context.getLogging().debug("Segment [" + collocatedSegment.toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

#pragma omp parallel for
	for (long l = firstL; l <= lastL; l++) {
		valarray<double> coordinates(10);
		valarray<double> tpiWeights(1);
		valarray<size_t> tpiIndexes(1);

		matrix<double> matRatmOlc(40, 18);
		matrix<double> matRatmSln(40, 6);
		matrix<double> matRatmSlo(40, 6);
		matrix<double> matTs(40, 30);
		matrix<double> matTv(40, 30);
		matrix<double> matRho(40, 30);

		valarray<double> f(lutOlcRatm.getDimensionCount());
		valarray<double> w(lutOlcRatm.getWorkspaceSize());

		valarray<double> ts(30);
		valarray<double> tv(30);
		valarray<double> tO3(30);

		valarray<double> rboa(30);
		valarray<double> rtoa(30);

		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l) + " ...", getId());

		for (long k = collocatedGrid.getFirstK(); k < collocatedGrid.getFirstK() + collocatedGrid.getSizeK(); k++) {
			for (long m = collocatedGrid.getFirstM(); m < collocatedGrid.getFirstM() + collocatedGrid.getSizeM(); m++) {
				const size_t i = collocatedGrid.getIndex(k, l, m);

				// TODO: consider flags

				// TODO - get from ECMWF tie points
				const double nO3 = 0.0;
				const double wv = 2.0;
				const double p = 1000;

				// TODO - get from segment data
				const double tau550 = 0.1;
				const uint8_t amin = 1;

				/*
				 * Surface reflectance for OLC channels
				 */
				tpiOlc.prepare(lonAccessor.getDouble(i), latAccessor.getDouble(i), tpiWeights, tpiIndexes);

				const double szaOlc = tpiOlc.interpolate(tpSzasOlc, tpiWeights, tpiIndexes);
				const double saaOlc = tpiOlc.interpolate(tpSaasOlc, tpiWeights, tpiIndexes);
				const double vzaOlc = tpiOlc.interpolate(tpVzasOlc, tpiWeights, tpiIndexes);
				const double vaaOlc = tpiOlc.interpolate(tpVaasOlc, tpiWeights, tpiIndexes);

				coordinates[0] = abs(saaOlc - vaaOlc); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaOlc; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
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
					// Eq. 2-1
					const double ltoa = ltoaAccessors[b]->getDouble(i);
					const double f0 = solarIrrOlcAccessor.getDouble(olcInfoGrid.getIndex(k, b, m));
					rtoa[b] = toaReflectance(ltoa, f0, szaOlc);

					// Eq. 2-2
					tO3[b] = ozoneTransmission(lutCO3, szaOlc, vzaOlc, nO3, b + 1.0);

					// Eq. 2-3
					const double ratm = matRatmOlc(amin - 1, b);
					ts[b] = matTs(amin - 1, b);
					tv[b] = matTv(amin - 1, b);
					const double rho = matRho(amin - 1, b);
					rboa[b] = surfaceReflectance(rtoa[b], ratm, ts[b], tv[b], rho, tO3[b]);

					if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
						sdrAccessors[b]->setDouble(i, rboa[b]);
					} else {
						sdrAccessors[b]->setFillValue(i);
					}
				}

				/*
				 * Surface reflectance for SLN channels
				 */
				tpiSln.prepare(lonAccessor.getDouble(i), latAccessor.getDouble(i), tpiWeights, tpiIndexes);

				const double vzaSln = tpiSln.interpolate(tpVzasSln, tpiWeights, tpiIndexes);
				const double vaaSln = tpiSln.interpolate(tpVaasSln, tpiWeights, tpiIndexes);

				coordinates[0] = abs(saaOlc - vaaSln); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaSln; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = tau550; // aerosol

				lutSlnRatm.getValues(&coordinates[0], matRatmSln, f, w);
				lutT.getValues(&coordinates[2], matTv, f, w);

				for (size_t b = 18; b < 24; b++) {
					// Eq. 2-1
					const double ltoa = ltoaAccessors[b]->getDouble(i);
					const double f0 = solarIrrSlnAccessors[b - 18]->getDouble(slnInfoGrid.getIndex(0, 0, l % 4));
					rtoa[b] = toaReflectance(ltoa, f0, szaOlc);

					// Eq. 2-2
					tO3[b] = ozoneTransmission(lutCO3, szaOlc, vzaSln, nO3, b + 1.0);

					// Eq. 2-3
					const double ratm = matRatmSln(amin - 1, b - 18);
					ts[b] = matTs(amin - 1, b);
					tv[b] = matTv(amin - 1, b);
					const double rho = matRho(amin - 1, b);
					rboa[b] = surfaceReflectance(rtoa[b], ratm, ts[b], tv[b], rho, tO3[b]);

					if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
						sdrAccessors[b]->setDouble(i, rboa[b]);
					} else {
						sdrAccessors[b]->setFillValue(i);
					}
				}

				/*
				 * Surface reflectance for SLO channels
				 */
				context.getLogging().info("preparing tpiSlo", getId());
				tpiSlo.prepare(lonAccessor.getDouble(i), latAccessor.getDouble(i), tpiWeights, tpiIndexes);

				context.getLogging().info("interpolating tpVzasSlo", getId());
				const double vzaSlo = tpiSlo.interpolate(tpVzasSlo, tpiWeights, tpiIndexes);
				context.getLogging().info("interpolating tpVaasSlo", getId());
				const double vaaSlo = tpiSlo.interpolate(tpVaasSlo, tpiWeights, tpiIndexes);

				coordinates[0] = abs(saaOlc - vaaSlo); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaSlo; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = tau550; // aerosol

				context.getLogging().info("interpolating lutSloRatm", getId());
				lutSloRatm.getValues(&coordinates[0], matRatmSlo, f, w);
				context.getLogging().info("interpolating lutT", getId());
				lutT.getValues(&coordinates[2], matTv, f, w);

				for (size_t b = 24; b < 30; b++) {
					context.getLogging().info("b = " + lexical_cast<string>(b), getId());
					// Eq. 2-1
					const double ltoa = ltoaAccessors[b]->getDouble(i);
					const double f0 = solarIrrSloAccessors[b - 24]->getDouble(sloInfoGrid.getIndex(0, 0, l & 4));
					rtoa[b] = toaReflectance(ltoa, f0, szaOlc);

					// Eq. 2-2
					tO3[b] = ozoneTransmission(lutCO3, szaOlc, vzaSlo, nO3, b + 1.0);

					// Eq. 2-3
					const double ratm = matRatmSlo(amin - 1, b - 24);
					ts[b] = matTs(amin - 1, b);
					tv[b] = matTv(amin - 1, b);
					const double rho = matRho(amin - 1, b);
					rboa[b] = surfaceReflectance(rtoa[b], ratm, ts[b], tv[b], rho, tO3[b]);

					if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
						sdrAccessors[b]->setDouble(i, rboa[b]);
					} else {
						sdrAccessors[b]->setFillValue(i);
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

				coordinates[6] = coordinates[1]; // SZA
				coordinates[7] = coordinates[3]; // air pressure
				coordinates[8] = coordinates[4]; // water vapour
				coordinates[9] = coordinates[5]; // aerosol

				lutOlcRatm.getValues(&coordinates[0], matRatmOlc, f, w);
				lutT.getValues(&coordinates[6], matTs, f, w);
				lutT.getValues(&coordinates[2], matTv, f, w);
				lutRhoAtm.getValues(&coordinates[3], matRho, f, w);

				for (size_t b = 0; b < 18; b++) {
					if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
						// TODO: errors for OLCI channels

						if (rtoa[b] >= 0.0 && rtoa[b] <= 1.0) {
							errAccessors[b]->setDouble(i, rtoa[b]);
						} else {
							errAccessors[b]->setFillValue(i);
						}
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

				lutSlnRatm.getValues(&coordinates[0], matRatmSln, f, w);
				lutT.getValues(&coordinates[2], matTv, f, w);

				for (size_t b = 18; b < 24; b++) {
					if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
						// TODO: errors for SLN channels

						if (rtoa[b] >= 0.0 && rtoa[b] <= 1.0) {
							errAccessors[b]->setDouble(i, rtoa[b]);
						} else {
							errAccessors[b]->setFillValue(i);
						}
					}
				}

				/*
				 * Errors for SLO channels
				 */
				/* TODO: fix segmentation fault occurring below
				coordinates[0] = abs(saaOlc - vaaSlo); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaSlo; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = 0.8 * tau550; // aerosol

				lutSloRatm.getValues(&coordinates[0], matRatmSlo, f, w);
				lutT.getValues(&coordinates[2], matTv, f, w);

				for (size_t b = 24; b < 30; b++) {
					if (rboa[b] >= 0.0 && rboa[b] <= 1.0) {
						// TODO: errors for SLO channels

						if (rtoa[b] >= 0.0 && rtoa[b] <= 1.0) {
							errAccessors[b]->setDouble(i, rtoa[b]);
						} else {
							errAccessors[b]->setFillValue(i);
						}
					}
				}
				*/
			}
		}
	}
	context.setLastComputedL(collocatedSegment, *this, lastL);
}

void Aco::addAccessor(Context& context, Segment& s, const VariableDescriptor& varDescriptor) const {
	context.getLogging().info("Adding accessor for " + varDescriptor.toString() + "to segment " + s.toString(), getId());
	s.addVariable(varDescriptor);
}

void Aco::addMatrixLookupTable(Context& context, const string& fileName, const string& varName) const {
	if (!context.hasObject(varName)) {
		const LookupTableReader reader(getAuxdataPath() + fileName);
		context.getLogging().info("Reading LUT '" + varName + "'", getId());
		context.addObject(reader.readMatrixLookupTable<double>(varName));
	}
}

void Aco::addScalarLookupTable(Context& context, const string& fileName, const string& varName) const {
	if (!context.hasObject(varName)) {
		const LookupTableReader reader(getAuxdataPath() + fileName);
		context.getLogging().info("Reading LUT '" + varName + "'", getId());
		context.addObject(reader.readScalarLookupTable<double>(varName));
	}
}

double Aco::ozoneTransmission(const ScalarLookupTable<double>& lut, double sza, double vza, double nO3, double channel) {
	// Eq. 2-2
	const double m = 0.5 * (1.0 / cos(sza * D2R) + 1.0 / cos(vza * D2R));
	const double cO3 = lut.getValue(&channel);
	const double tO3 = exp(-m * nO3 * cO3);

	return tO3;
}

double Aco::surfaceReflectance(double rtoa, double ratm, double ts, double tv, double rho, double tO3) {
	// Eq. 2-3
	const double f = (rtoa - tO3 * ratm) / (tO3 * ts * tv);
	const double rboa = f / (1.0 + rho * f);

	return rboa;
}

double Aco::toaReflectance(double ltoa, double f0, double sza) {
	return (PI * ltoa) / (f0 * cos(sza * D2R));
}
