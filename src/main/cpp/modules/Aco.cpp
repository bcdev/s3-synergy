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
	const LookupTableReader reader(getInstallationPath() + "/auxdata/v1/S3__SY_2_SYRTAX.nc");
	lutOlcRatm = reader.readMatrixLookupTable<double>("OLC_R_atm");
	lutT = reader.readMatrixLookupTable<double>("t");
	lutRhoAtm = reader.readMatrixLookupTable<double>("rho_atm");
	lutCO3 = reader.readScalarLookupTable<double>("C_O3");

	context.addObject(lutOlcRatm);
	context.addObject(lutT);
	context.addObject(lutRhoAtm);
	context.addObject(lutCO3);

	Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	for (size_t i = 1; i <= 30; i++) {
		// TODO - get attributes from dictionary
		t.addVariable("SDR_" + lexical_cast<string>(i), Constants::TYPE_SHORT, 0.0001);
		t.addVariable("SDR_" + lexical_cast<string>(i) + "_er", Constants::TYPE_SHORT, 0.0001);
	}
}

void Aco::stop(Context& context) {

}

void Aco::process(Context& context) {
	const Accessor& tpVza = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_VZA");
	const Accessor& tpVaa = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_VAA");
	const Accessor& tpSza = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("SZA");
	const Accessor& tpSaa = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("SAA");
	const Accessor& tpLat = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lat");
	const Accessor& tpLon = context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lon");

	const valarray<double> tpLons = tpLon.getDoubles();
	const valarray<double> tpLats = tpLat.getDoubles();
	const valarray<double> tpSzas = tpSza.getDoubles();
	const valarray<double> tpSaas = tpSaa.getDoubles();
	const valarray<double> tpVzas = tpVza.getDoubles();
	const valarray<double> tpVaas = tpVaa.getDoubles();

	const TiePointInterpolator<double> tpi = TiePointInterpolator<double>(tpLons, tpLats);

	const Segment& olc = context.getSegment(Constants::SEGMENT_OLC);
	const Segment& olcInfo = context.getSegment(Constants::SEGMENT_OLC_INFO);

	vector<Accessor*> L;
	for (size_t i = 1; i <= 18; i++) {
		L.push_back(&olc.getAccessor("L_" + lexical_cast<string>(i)));
	}
	const Accessor& lat = olc.getAccessor("latitude");
	const Accessor& lon = olc.getAccessor("longitude");
	const Accessor& solarIrradiance = olcInfo.getAccessor("solar_irradiance");

	const Grid& olcInfoGrid = olcInfo.getGrid();
	const Grid& olcGrid = olc.getGrid();

	const Segment& col = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Grid& colGrid = col.getGrid();
	vector<Accessor*> sdr;
	for (size_t i = 1; i <= 18; i++) {
		sdr.push_back(&col.getAccessor("SDR_" + lexical_cast<string>(i)));
	}
	vector<Accessor*> err;
	for (size_t i = 1; i <= 18; i++) {
		err.push_back(&col.getAccessor("SDR_" + lexical_cast<string>(i) + "_er"));
	}

	context.getLogging()->progress("Processing segment '" + col.toString() + "'", getId());

	// TODO - get from ECMWF tie points
	const double no3 = 0.0;
	const double wv = 2.0;
	const double p = 1000;

	// TODO - get from segment data
	const double tau550 = 0.1;

	for (size_t k = olcGrid.getFirstK(); k < olcGrid.getFirstK() + olcGrid.getSizeK(); k++) {
		#pragma omp parallel for
		for (size_t l = olcGrid.getFirstL(); l < olcGrid.getFirstL() + olcGrid.getSizeL(); l++) {
			valarray<double> coordinates(20);
			valarray<double> tpiWeights(1);
			valarray<size_t> tpiIndexes(1);

			matrix<double> matRatm(40, 18);
			matrix<double> matTs(40, 30);
			matrix<double> matTv(40, 30);
			matrix<double> matRho(40, 30);

			for (size_t m = olcGrid.getFirstM(); m < olcGrid.getFirstM() + olcGrid.getSizeM(); m++) {
				const size_t i = olcGrid.getIndex(k, l, m);
				const size_t j = colGrid.getIndex(k, l, m);

				tpi.prepare(lon.getDouble(i), lat.getDouble(i), tpiWeights, tpiIndexes);

				const double sza = tpi.interpolate(tpSzas, tpiWeights, tpiIndexes);
				const double saa = tpi.interpolate(tpSaas, tpiWeights, tpiIndexes);
				const double vza = tpi.interpolate(tpVzas, tpiWeights, tpiIndexes);
				const double vaa = tpi.interpolate(tpVaas, tpiWeights, tpiIndexes);

				coordinates[0] = abs(saa - vaa); // ADA
				coordinates[1] = sza; // SZA
				coordinates[2] = vza; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = tau550; // aerosol
//				coordinates[6] = 1; // aerosol model index
//				coordinates[7] = b + 1; // SYN channel

				coordinates[8] = coordinates[1]; // SZA
				coordinates[9] = coordinates[3]; // air pressure
				coordinates[10] = coordinates[4]; // water vapour
				coordinates[11] = coordinates[5]; // aerosol
//				coordinates[12] = coordinates[6]; // aerosol model index
//				coordinates[13] = coordinates[7]; // SYN channel

				coordinates[14] = coordinates[2]; // VZA
				coordinates[15] = coordinates[3]; // air pressure
				coordinates[16] = coordinates[4]; // water vapour
				coordinates[17] = coordinates[5]; // aerosol
//				coordinates[18] = coordinates[6]; // aerosol model index
//				coordinates[19] = coordinates[7]; // SYN channel

				lutOlcRatm->getValues(&coordinates[0], matRatm);
				lutT->getValues(&coordinates[8], matTs);
				lutT->getValues(&coordinates[14], matTv);
				lutRhoAtm->getValues(&coordinates[9], matRho);

				for (size_t b = 0; b < 18; b++) {
					const double channel = b + 1.0;
					const double ratm = matRatm(0, b);
					const double ts = matTs(0, b);
					const double tv = matTv(0, b);
					const double rho = matRho(0, b);
					const double co3 = lutCO3->getValue(&channel);

					const double ltoa = L[b]->getDouble(i);
					const double f0 = solarIrradiance.getDouble(olcInfoGrid.getIndex(k, b, m));

					// Eq. 2-1
					const double rtoa = (PI * ltoa) / (f0 * cos(sza * D2R));

					// Eq. 2-2
					const double m = 0.5 * (1.0 / cos(sza * D2R) + 1.0 / cos(vza * D2R));
					const double to3 = exp(-m * no3 * co3);

					// Eq. 2-3
					const double f = (rtoa - to3 * ratm) / (to3 * ts * tv);
					const double rsurf = f / (1.0 + rho * f);

					sdr[b]->setDouble(j, rsurf);
					err[b]->setDouble(j, rtoa);
				}
			}
		}
	}
}
