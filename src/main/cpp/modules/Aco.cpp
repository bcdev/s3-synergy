/*
 * Aco.cpp
 *
 *  Created on: Aug 24, 2011
 *      Author: ralf
 */

#include <cmath>

#include "../core/TiePointInterpolator.h"
#include "../util/LookupTableReader.h"

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
	context.getLogging()->info("Reading LUTs for atmospheric correction module ...", getId());

	// TODO - only read LUTs when there are not already known to the context
	const LookupTableReader reader(getInstallationPath() + "/auxdata/v" + Constants::PROCESSOR_VERSION + "/S3__SY_2_SYRTAX.nc");
	lutOlcRatm = reader.readMatrixLookupTable<double>("OLC_R_atm");
	lutSlnRatm = reader.readMatrixLookupTable<double>("SLN_R_atm");
	lutSloRatm = reader.readMatrixLookupTable<double>("SLO_R_atm");
	lutT = reader.readMatrixLookupTable<double>("t");
	lutRhoAtm = reader.readMatrixLookupTable<double>("rho_atm");
	lutCO3 = reader.readScalarLookupTable<double>("C_O3");

	context.addObject(lutOlcRatm);
	context.addObject(lutSlnRatm);
	context.addObject(lutSloRatm);
	context.addObject(lutT);
	context.addObject(lutRhoAtm);
	context.addObject(lutCO3);

	Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const SegmentDescriptor& d = context.getDictionary()->getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
	for (size_t i = 1; i <= 30; i++) {
		const string sdrBandName = "SDR_" + lexical_cast<string>(i);
		const VariableDescriptor& sdrDescriptor = d.getVariableDescriptor(sdrBandName);
		context.getLogging()->info("Adding accessor for " + sdrDescriptor.toString() + "to segment " + t.toString(), getId());
		t.addVariable(sdrDescriptor);
		const string errBandName = sdrBandName + "_er";
		const VariableDescriptor& errDescriptor = d.getVariableDescriptor(errBandName);
		context.getLogging()->info("Adding accessor for " + errDescriptor.toString() + "to segment " + t.toString(), getId());
		t.addVariable(errDescriptor);
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

	const Segment& col = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Segment& olcInfo = context.getSegment(Constants::SEGMENT_OLC_INFO);
	const Segment& slnInfo = context.getSegment(Constants::SEGMENT_SLN_INFO);

	vector<Accessor*> lToas;
	for (size_t i = 1; i <= 30; i++) {
		lToas.push_back(&col.getAccessor("L_" + lexical_cast<string>(i)));
	}
	const Accessor& lat = col.getAccessor("latitude");
	const Accessor& lon = col.getAccessor("longitude");
	const Accessor& solarIrradianceOlc = olcInfo.getAccessor("solar_irradiance");

	vector<Accessor*> solarIrradianceSln;
	for (size_t i = 1; i <= 6; i++) {
		solarIrradianceSln.push_back(&slnInfo.getAccessor("solar_irradiance_" + lexical_cast<string>(i)));
	}

	const Grid& olcInfoGrid = olcInfo.getGrid();
	const Grid& slnInfoGrid = slnInfo.getGrid();
	const Grid& colGrid = col.getGrid();

	vector<Accessor*> sdr;
	for (size_t i = 1; i <= 30; i++) {
		sdr.push_back(&col.getAccessor("SDR_" + lexical_cast<string>(i)));
	}
	vector<Accessor*> err;
	for (size_t i = 1; i <= 30; i++) {
		err.push_back(&col.getAccessor("SDR_" + lexical_cast<string>(i) + "_er"));
	}

	context.getLogging()->progress("Processing segment '" + col.toString() + "'", getId());

	// TODO - get from ECMWF tie points
	const double no3 = 0.0;
	const double wv = 2.0;
	const double p = 1000;

	// TODO - get from segment data
	const double tau550 = 0.1;

	const long firstL = context.getFirstComputableL(col, *this);
	context.getLogging()->debug("Segment [" + col.toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
	const long lastL = context.getLastComputableL(col, *this);
	context.getLogging()->debug("Segment [" + col.toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

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

		valarray<double> f(lutOlcRatm->getDimensionCount());
		valarray<double> w(lutOlcRatm->getWorkspaceSize());

		context.getLogging()->progress("Processing line l = " + lexical_cast<string>(l) + " ...", getId());

		for (long k = colGrid.getFirstK(); k < colGrid.getFirstK() + colGrid.getSizeK(); k++) {
			for (long m = colGrid.getFirstM(); m < colGrid.getFirstM() + colGrid.getSizeM(); m++) {
				const size_t i = colGrid.getIndex(k, l, m);

				tpiOlc.prepare(lon.getDouble(i), lat.getDouble(i), tpiWeights, tpiIndexes);

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

				lutOlcRatm->getValues(&coordinates[0], matRatmOlc, f, w);
				lutT->getValues(&coordinates[6], matTs, f, w);
				lutT->getValues(&coordinates[2], matTv, f, w);
				lutRhoAtm->getValues(&coordinates[3], matRho, f, w);

				for (size_t b = 0; b < 18; b++) {
					const double channel = b + 1.0;
					const double ratm = matRatmOlc(0, b);
					const double ts = matTs(0, b);
					const double tv = matTv(0, b);
					const double rho = matRho(0, b);
					const double co3 = lutCO3->getValue(&channel);

					const double ltoa = lToas[b]->getDouble(i);
					const double f0 = solarIrradianceOlc.getDouble(olcInfoGrid.getIndex(k, b, m));

					// Eq. 2-1
					const double rtoa = (PI * ltoa) / (f0 * cos(szaOlc * D2R));

					// Eq. 2-2
					const double m = 0.5 * (1.0 / cos(szaOlc * D2R) + 1.0 / cos(vzaOlc * D2R));
					const double to3 = exp(-m * no3 * co3);

					// Eq. 2-3
					const double f = (rtoa - to3 * ratm) / (to3 * ts * tv);
					const double rsurf = f / (1.0 + rho * f);

					sdr[b]->setDouble(i, rsurf);
					err[b]->setDouble(i, rtoa);
				}

				tpiSln.prepare(lon.getDouble(i), lat.getDouble(i), tpiWeights, tpiIndexes);

				const double vzaSln = tpiSln.interpolate(tpVzasSln, tpiWeights, tpiIndexes);
				const double vaaSln = tpiSln.interpolate(tpVaasSln, tpiWeights, tpiIndexes);

				coordinates[0] = abs(saaOlc - vaaSln); // ADA
				coordinates[1] = szaOlc; // SZA
				coordinates[2] = vzaSln; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = tau550; // aerosol

				lutSlnRatm->getValues(&coordinates[0], matRatmSln, f, w);
				lutT->getValues(&coordinates[2], matTv, f, w);

				for (size_t b = 18; b < 24; b++) {
					const double channel = b + 1.0;
					const double ratm = matRatmSln(0, b - 18);
					const double ts = matTs(0, b);
					const double tv = matTv(0, b);
					const double rho = matRho(0, b);
					const double co3 = lutCO3->getValue(&channel);

					const double ltoa = lToas[b]->getDouble(i);
					const double f0 = solarIrradianceSln[b - 18]->getDouble(slnInfoGrid.getIndex(0, 0, 1));

					// Eq. 2-1
					const double rtoa = (PI * ltoa) / (f0 * cos(szaOlc * D2R));

					// Eq. 2-2
					const double m = 0.5 * (1.0 / cos(szaOlc * D2R) + 1.0 / cos(vzaSln * D2R));
					const double to3 = exp(-m * no3 * co3);

					// Eq. 2-3
					const double f = (rtoa - to3 * ratm) / (to3 * ts * tv);
					const double rsurf = f / (1.0 + rho * f);

					if (rsurf >= 0.0 && rsurf <= 1.0) {
						sdr[b]->setDouble(i, rsurf);
					} else {
						sdr[b]->setFillValue(i);
					}
					if (rtoa >= 0.0 && rtoa <= 1.0) {
						err[b]->setDouble(i, rtoa);
					} else {
						err[b]->setFillValue(i);
					}
				}

			}
		}
	}
	context.setLastComputedL(col, *this, lastL);
}
