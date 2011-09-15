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
	const LookupTableReader reader(
			getInstallationPath() + "/auxdata/v1/S3__SY_2_SYRTAX.nc");
	lutOlcRatm = reader.readLookupTable<double>("OLC_R_atm");
	lutT = reader.readLookupTable<double>("t");
	lutRhoAtm = reader.readLookupTable<double>("rho_atm");
	lutCO3 = reader.readLookupTable<double>("C_O3");

	context.addObject(lutOlcRatm);
	context.addObject(lutT);
	context.addObject(lutRhoAtm);
	context.addObject(lutCO3);

	Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	t.addVariable("SDR_1", Constants::TYPE_SHORT, 0.0001);
	t.addVariable("SDR_2", Constants::TYPE_SHORT, 0.0001);
	t.addVariable("SDR_3", Constants::TYPE_SHORT, 0.0001);
	t.addVariable("SDR_4", Constants::TYPE_SHORT, 0.0001);
	t.addVariable("SDR_5", Constants::TYPE_SHORT, 0.0001);
	t.addVariable("SDR_6", Constants::TYPE_SHORT, 0.0001);
	t.addVariable("SDR_7", Constants::TYPE_SHORT, 0.0001);
	t.addVariable("SDR_8", Constants::TYPE_SHORT, 0.0001);
	t.addVariable("SDR_9", Constants::TYPE_SHORT, 0.0001);
	t.addVariable("SDR_10", Constants::TYPE_SHORT, 0.0001);
}

void Aco::stop(Context& context) {

}

void Aco::process(Context& context) {
	const Accessor& tpVza =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor(
					"OLC_VZA");
	const Accessor& tpVaa =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor(
					"OLC_VAA");
	const Accessor& tpSza =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("SZA");
	const Accessor& tpSaa =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("SAA");
	const Accessor& tpLat =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor(
					"OLC_TP_lat");
	const Accessor& tpLon =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor(
					"OLC_TP_lon");

	const valarray<double> tpLons = tpLon.getDoubles();
	const valarray<double> tpLats = tpLat.getDoubles();
	const valarray<double> tpSzas = tpSza.getDoubles();
	const valarray<double> tpSaas = tpSaa.getDoubles();
	const valarray<double> tpVzas = tpVza.getDoubles();
	const valarray<double> tpVaas = tpVaa.getDoubles();

	const TiePointInterpolator<double> tpi = TiePointInterpolator<double>(tpLons, tpLats);

	const Segment& olc = context.getSegment(Constants::SEGMENT_OLC);
	const Segment& olcInfo = context.getSegment(Constants::SEGMENT_OLC_INFO);

	const Accessor& l1 = olc.getAccessor("L_1");
	const Accessor& lat = olc.getAccessor("latitude");
	const Accessor& lon = olc.getAccessor("longitude");
	const Accessor& solarIrradiance = olcInfo.getAccessor("solar_irradiance");

	const Grid& olcInfoGrid = olcInfo.getGrid();
	const Grid& olcGrid = olc.getGrid();

	const Segment& syc = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	Accessor& sdr1 = syc.getAccessor("SDR_1");
	Accessor& sdr2 = syc.getAccessor("SDR_2");
	Accessor& sdr3 = syc.getAccessor("SDR_3");
	Accessor& sdr4 = syc.getAccessor("SDR_4");
	Accessor& sdr5 = syc.getAccessor("SDR_5");
	Accessor& sdr6 = syc.getAccessor("SDR_6");
	Accessor& sdr7 = syc.getAccessor("SDR_7");
	Accessor& sdr8 = syc.getAccessor("SDR_8");
	Accessor& sdr9 = syc.getAccessor("SDR_9");
	Accessor& sdr10 = syc.getAccessor("SDR_10");

	// TODO - get from ECMWF tie points
	const double no3 = 0.0;
	const double wv = 2.0;
	const double p = 1000;

	// TODO - get from segment data
	const double tau550 = 0.1;

	// #pragma omp parallel for
	for (size_t k = olcGrid.getFirstK();
			k < olcGrid.getFirstK() + olcGrid.getSizeK(); k++) {
		valarray<double> coordinates(20);
		valarray<double> tpiWeights(2);
		valarray<size_t> tpiIndexes(2);
		for (size_t l = olcGrid.getFirstL();
				l < olcGrid.getFirstL() + olcGrid.getSizeL(); l++) {
			for (size_t m = olcGrid.getFirstK();
					m < olcGrid.getFirstM() + olcGrid.getSizeM(); m++) {
				const size_t i = olcGrid.getIndex(k, l, m);

				tpi.prepare(lon.getDouble(i), lat.getDouble(i), tpiWeights, tpiIndexes);

				const double sza = tpi.interpolate(tpSzas, tpiWeights, tpiIndexes);
				const double saa = tpi.interpolate(tpSaas, tpiWeights, tpiIndexes);
				const double vza = tpi.interpolate(tpVzas, tpiWeights, tpiIndexes);
				const double vaa = tpi.interpolate(tpVaas, tpiWeights, tpiIndexes);

				coordinates[0] = abs(saa - vaa); // ADA
				coordinates[1] = 0.0; // SZA
				coordinates[2] = 0.0; // VZA
				coordinates[3] = p; // air pressure
				coordinates[4] = wv; // water vapour
				coordinates[5] = tau550; // aerosol
				coordinates[6] = 1; // aerosol model index
				coordinates[7] = 1; // SYN channel

				coordinates[8] = coordinates[1]; // SZA
				coordinates[9] = coordinates[3]; // air pressure
				coordinates[10] = coordinates[4]; // water vapour
				coordinates[11] = coordinates[5]; // aerosol
				coordinates[12] = coordinates[6]; // aerosol model index
				coordinates[13] = coordinates[7]; // SYN channel

				coordinates[14] = coordinates[2]; // VZA
				coordinates[15] = coordinates[3]; // air pressure
				coordinates[16] = coordinates[4]; // water vapour
				coordinates[17] = coordinates[5]; // aerosol
				coordinates[18] = coordinates[6]; // aerosol model index
				coordinates[19] = coordinates[7]; // SYN channel

				const double ratm = lutOlcRatm->operator()(&coordinates[0]);
				const double ts = lutT->operator()(&coordinates[8]);
				const double tv = lutT->operator()(&coordinates[14]);
				const double rho = lutRhoAtm->operator()(&coordinates[9]);
				const double co3 = lutCO3->operator()(&coordinates[7]);
				const double ltoa = l1.getDouble(olcGrid.getIndex(k, l, m));
				const double f0 = solarIrradiance.getDouble(olcInfoGrid.getIndex(k, 1, m));

				// Eq. 2-1
				const double rtoa = (PI * ltoa) / (f0 * cos(sza * D2R));

				// Eq. 2-2
				const double m = 0.5 * (1.0 / cos(sza * D2R) + 1.0 / cos(vza * D2R));
				const double to3 = exp(-m * no3 * co3);

				// Eq. 2-3
				const double f = (rtoa - to3 * ratm) / (to3 * ts * tv);
				const double rsurf = f / (1.0 + rho * f);

				sdr1.setDouble(i, rtoa);
				sdr2.setDouble(i, rsurf);
				sdr3.setDouble(i, sza / 90.0);
				sdr4.setDouble(i, saa / 180.0);
				sdr5.setDouble(i, vza / 90.0);
				sdr6.setDouble(i, vaa / 180.0);
				sdr7.setDouble(i, ratm);
				sdr8.setDouble(i, ts);
				sdr9.setDouble(i, tv);
				sdr10.setDouble(i, rho);
			}
		}
	}
}
