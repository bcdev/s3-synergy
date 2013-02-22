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

#include "Aco.h"
#include "Vac.h"

using boost::lexical_cast;

Vac::Vac() :
		BasicModule("VAC") {
}

Vac::~Vac() {
}

void Vac::start(Context& context) {
	getLookupTable(context, Constants::AUX_ID_VSRTAX, "rho_atm");
	getLookupTable(context, Constants::AUX_ID_VSRTAX, "VGT_R_atm");
	getLookupTable(context, Constants::AUX_ID_VSRTAX, "t");

	context.getSegment(Constants::SEGMENT_SYN_COLLOCATED).addVariable("NDVI", Constants::TYPE_DOUBLE);
}

void Vac::process(Context& context) {
	using std::abs;
	using std::exp;

	const Segment& segment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Grid& grid = segment.getGrid();

	const Accessor& saaAccessor = segment.getAccessor("SAA");
	const Accessor& szaAccessor = segment.getAccessor("SZA");
	const Accessor& vaaAccessor = segment.getAccessor("VAA");
	const Accessor& vzaAccessor = segment.getAccessor("VZA");

	const Accessor& vgtFlagsAccessor = segment.getAccessor("SM");
	const Accessor& aotAccessor = segment.getAccessor("AG");
	const Accessor& airPressureAccessor = segment.getAccessor("PG");
	const Accessor& ozoneAccessor = segment.getAccessor("OG");
	const Accessor& waterVapourAccessor = segment.getAccessor("WVG");

	valarray<double> cO3;
	int16_t aerosolModelIndex;

	getAuxdataProvider(context, Constants::AUX_ID_VSRTAX).getVectorDouble("C_O3", cO3);
	getAuxdataProvider(context, Constants::AUX_ID_VSCPAX).getShort("AMIN", aerosolModelIndex);

	const LookupTable<double>& lutRhoAtm = getLookupTable(context, Constants::AUX_ID_VSRTAX, "rho_atm");
	const LookupTable<double>& lutRatm = getLookupTable(context, Constants::AUX_ID_VSRTAX, "VGT_R_atm");
	const LookupTable<double>& lutT = getLookupTable(context, Constants::AUX_ID_VSRTAX, "t");

	const long minK = grid.getMinK();
	const long maxK = grid.getMaxK();
	const long minL = context.getFirstComputableL(segment, *this);
	const long maxL = context.getLastComputableL(segment, *this);
	const long minM = grid.getMinM();
	const long maxM = grid.getMaxM();

	valarray<Accessor*> reflectanceAccessors(4);
	reflectanceAccessors[0] = &segment.getAccessor("B0");
	reflectanceAccessors[1] = &segment.getAccessor("B2");
	reflectanceAccessors[2] = &segment.getAccessor("B3");
	reflectanceAccessors[3] = &segment.getAccessor("MIR");
	Accessor& ndviAccessor = segment.getAccessor("NDVI");

#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) > 40100
#pragma omp parallel for
#endif
	for (long l = minL; l <= maxL; l++) {
		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l) + " ...", getId());

		valarray<double> coordinates(lutRatm.getDimensionCount());
		valarray<double> w(lutRatm.getScalarWorkspaceSize());
		valarray<double> f(lutRatm.getDimensionCount());

		for (long k = minK; k <= maxK; k++) {
			for (long m = minM; m <= maxM; m++) {
				const size_t index = grid.getIndex(k, l, m);

				const uint8_t vgtFlags = vgtFlagsAccessor.getUByte(index);
				const double sza = szaAccessor.getDouble(index);
				const double saa = saaAccessor.getDouble(index);
				const double vza = vzaAccessor.getDouble(index);
				const double vaa = vaaAccessor.getDouble(index);
				const double airPressure = airPressureAccessor.getDouble(index);
				const double ozone = atmCmToDU(ozoneAccessor.getDouble(index));
				const double waterVapour = waterVapourAccessor.getDouble(index);
				const double aot = aotAccessor.getDouble(index);
				const double am = airMass(sza, vza);

				for (size_t b = 0; b < reflectanceAccessors.size(); b++) {
					if (isSet(vgtFlags, Constants::VGT_LAND_FLAG) && !isSet(vgtFlags, Constants::VGT_CLOUD_VALUE)) {
						// Land and no cloud
						const double reflectance = reflectanceAccessors[b]->getDouble(index);

						if (reflectance != Constants::FILL_VALUE_DOUBLE) {
							coordinates[0] = airPressure;
							coordinates[1] = waterVapour;
							coordinates[2] = aot;
							coordinates[3] = aerosolModelIndex;
							coordinates[4] = b;

							const double rhoAtm = lutRhoAtm.getScalar(&coordinates[0], f, w);

							coordinates[0] = abs(vaa - saa);
							coordinates[1] = sza;
							coordinates[2] = vza;
							coordinates[3] = airPressure;
							coordinates[4] = waterVapour;
							coordinates[5] = aot;
							coordinates[6] = aerosolModelIndex;
							coordinates[7] = b;

							const double rAtm = lutRatm.getScalar(&coordinates[0], f, w);
							const double tv = lutT.getScalar(&coordinates[1], f, w);
                            
							const double tO3 = exp(-am * (ozone - 350.0) * cO3[b]);
							const double surfaceReflectance = Aco::surfaceReflectance(reflectance, rAtm, tv, rhoAtm, tO3);

							reflectanceAccessors[b]->setDouble(index, surfaceReflectance);
						}
					} else {
						// Water or cloud
						reflectanceAccessors[b]->setFillValue(index);
					}
				}
				if (isSet(vgtFlags, Constants::VGT_LAND_FLAG)) {
					// Land
					const double b2 = reflectanceAccessors[1]->getDouble(index);
					const double b3 = reflectanceAccessors[2]->getDouble(index);

					if (b2 != Constants::FILL_VALUE_DOUBLE && b3 != Constants::FILL_VALUE_DOUBLE) {
						ndviAccessor.setDouble(index, ndvi(b2, b3));
					} else {
						ndviAccessor.setFillValue(index);
					}
				} else {
					// Water
					ndviAccessor.setFillValue(index);
				}
			}
		}
	}

	context.setLastComputedL(segment, *this, maxL);
}
