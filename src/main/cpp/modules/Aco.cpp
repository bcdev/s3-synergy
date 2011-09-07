/*
 * Aco.cpp
 *
 *  Created on: Aug 24, 2011
 *      Author: ralf
 */

#include <cmath>

#include "../util/LookupTableReader.h"

#include "Aco.h"

Aco::Aco() :
		BasicModule("ACO") {
}

Aco::~Aco() {
}

void Aco::start(Context& context) {
	/*
	const LookupTableReader reader(
			getInstallationPath() + "/auxdata/v1/S3__SY_2_SYRTAX.nc");
	olcRatm = reader.readLookupTable<double>("OLC_R_atm");
	t = reader.readLookupTable<double>("t");
	rhoAtm = reader.readLookupTable<double>("rho_atm");
	co3 = reader.readLookupTable<double>("C_O3");

	context.addObject(*olcRatm);
	context.addObject(*t);
	context.addObject(*rhoAtm);
	context.addObject(*co3);
	*/
}

void Aco::stop(Context& context) {

}

void Aco::process(Context& context) {
	/*
	const Accessor& vza =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor(
					"OLC_VZA");
	const Accessor& vaa =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor(
					"OLC_VAA");
	const Accessor& sza =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("SZA");
	const Accessor& saa =
			context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("SAA");

	const Segment& olcInfo = context.getSegment(Constants::SEGMENT_OLC_INFO);
	const Accessor& f = olcInfo.getAccessor("solar_irradiance");

	const Segment& olc = context.getSegment(Constants::SEGMENT_OLC);
	const Accessor& ltoa12 = olc.getAccessor("L_12");

	const double tau550 = 0.1;
	const double no3 = 0.0;
	const double wv = 2.0;
	const double p = 1000;

	double coordinates[14];
	coordinates[0] = 0.0; // ADA
	coordinates[1] = 0.0; // SZA
	coordinates[2] = 0.0; // VZA
	coordinates[3] = 1000.0; // air pressure
	coordinates[4] = 2.0; // water vapour
	coordinates[5] = 0.1; // aerosol
	coordinates[6] = 0.0; // aerosol model index
	coordinates[7] = 1.0; // SYN channel

	coordinates[8] = coordinates[1]; // SZA
	coordinates[9] = coordinates[3]; // air pressure
	coordinates[10] = coordinates[4]; // water vapour
	coordinates[11] = coordinates[5]; // aerosol
	coordinates[12] = coordinates[6]; // aerosol model index
	coordinates[13] = coordinates[7]; // SYN channel

	const double ratm = olcRatm->operator()(&coordinates[0]);
	const double tsza = t->operator()(&coordinates[8]);
	const double tvza = tsza;
	const double rho = rhoAtm->operator()(&coordinates[9]);
	const double coeffo3 = co3->operator()(&coordinates[7]);

	const Grid& olcInfoGrid = olcInfo.getGrid();
	const Grid& olcGrid = olc.getGrid();

	for (size_t k = olcGrid.getFirstK();
			k < olcGrid.getFirstK() + olcGrid.getSizeK(); k++) {
		for (size_t l = olcGrid.getFirstL();
				l < olcGrid.getFirstL() + olcGrid.getSizeL(); l++) {
			for (size_t m = olcGrid.getFirstK();
					m < olcGrid.getFirstM() + olcGrid.getSizeM(); m++) {
				const double ltoa = ltoa12.getDouble(olcGrid.getIndex(k, l, m));
				const double f0 = f.getDouble(olcInfoGrid.getIndex(k, 12, m));

				const double rtoa = (3.141592654 * ltoa) / (f0 * std::cos(0.0)); // Eq. 2-1
			}
		}
	}
	*/
}
