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

#include <algorithm>
#include <cmath>
#include <limits>

#include "../core/TiePointInterpolator.h"
#include "../util/ErrorMetric.h"
#include "../util/LookupTableReader.h"
#include "../util/AuxdataProvider.h"

#include "Aer.h"

using std::cos;
using std::max;
using std::min;
using std::numeric_limits;

using boost::lexical_cast;

class PixelProvider {

public:
	PixelProvider(const Context& context);
	~PixelProvider();
	Pixel& getPixel(size_t index, Pixel& p, bool aerosolOnly = false) const;
	void putPixel(size_t index, const Pixel& p);

private:
	template<class T>
	static void copy(const std::valarray<T>& s, std::valarray<T>& t) {
		t.resize(s.size());
		std::copy(&s[0], &s[s.size()], &t[0]);
	}

	const Context& context;

	const Segment& averagedSegment;
	const Segment& olcInfoSegment;
	const Segment& slnInfoSegment;
	const Segment& sloInfoSegment;

	const Grid& averagedGrid;
	const Grid& olcInfoGrid;
	const Grid& slnInfoGrid;
	const Grid& sloInfoGrid;

	Accessor& tau550;
	Accessor& tau550Error;
	Accessor& alpha550;
	Accessor& synFlags;
	Accessor& amin;

	const Accessor& lat;
	const Accessor& lon;

	const TiePointInterpolator<double> tiePointInterpolatorOlc;
	const TiePointInterpolator<double> tiePointInterpolatorSln;
	const TiePointInterpolator<double> tiePointInterpolatorSlo;

	vector<Accessor*> radianceAccessors;
	vector<Accessor*> solarIrradianceOlcAccessors;
	vector<Accessor*> solarIrradianceSlnAccessors;
	vector<Accessor*> solarIrradianceSloAccessors;

	valarray<double> szaTiePointsOlc;
	valarray<double> saaTiePointsOlc;
	valarray<double> vzaTiePointsOlc;
	valarray<double> vaaTiePointsOlc;
	valarray<double> vzaTiePointsSln;
	valarray<double> vaaTiePointsSln;
	valarray<double> vzaTiePointsSlo;
	valarray<double> vaaTiePointsSlo;

	valarray<double> ozoneTiePoints;
	valarray<double> airPressureTiePoints;
	valarray<double> waterVapourTiePoints;
};

PixelProvider::PixelProvider(const Context& context) :
		context(context),
		averagedSegment(context.getSegment(Constants::SEGMENT_SYN_AVERAGED)),
		olcInfoSegment(context.getSegment(Constants::SEGMENT_OLC_INFO)),
		slnInfoSegment(context.getSegment(Constants::SEGMENT_SLN_INFO)),
		sloInfoSegment(context.getSegment(Constants::SEGMENT_SLO_INFO)),
		averagedGrid(averagedSegment.getGrid()),
		olcInfoGrid(olcInfoSegment.getGrid()),
		slnInfoGrid(slnInfoSegment.getGrid()),
		sloInfoGrid(sloInfoSegment.getGrid()),
		tau550(averagedSegment.getAccessor("T550")),
		tau550Error(averagedSegment.getAccessor("T550_er")),
		alpha550(averagedSegment.getAccessor("A550")),
		synFlags(averagedSegment.getAccessor("SYN_flags")),
		amin(averagedSegment.getAccessor("AMIN")),
		lat(averagedSegment.getAccessor("latitude")),
		lon(averagedSegment.getAccessor("longitude")),
		tiePointInterpolatorOlc(context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lat").getDoubles()),
		tiePointInterpolatorSln(context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lat").getDoubles()),
		tiePointInterpolatorSlo(context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lat").getDoubles()) {

	for (size_t b = 0; b < 30; b++) {
		radianceAccessors.push_back(&averagedSegment.getAccessor("L_" + lexical_cast<string>(b + 1)));
	}
	for (size_t b = 0; b < 18; b++) {
		solarIrradianceOlcAccessors.push_back(&averagedSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
	}
	for (size_t b = 18; b < 24; b++) {
		solarIrradianceSlnAccessors.push_back(&averagedSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
	}
	for (size_t b = 24; b < 30; b++) {
		solarIrradianceSloAccessors.push_back(&averagedSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
	}

	const Segment& olciTiepointSegment = context.getSegment(Constants::SEGMENT_OLC_TP);
	const Segment& slnTiepointSegment = context.getSegment(Constants::SEGMENT_SLN_TP);
	const Segment& sloTiepointSegment = context.getSegment(Constants::SEGMENT_SLO_TP);

	const Accessor& szaOlc = olciTiepointSegment.getAccessor("SZA");
	const Accessor& saaOlc = olciTiepointSegment.getAccessor("SAA");
	const Accessor& vzaOlc = olciTiepointSegment.getAccessor("OLC_VZA");
	const Accessor& vaaOlc = olciTiepointSegment.getAccessor("OLC_VAA");
	const Accessor& vzaSln = slnTiepointSegment.getAccessor("SLN_VZA");
	const Accessor& vaaSln = slnTiepointSegment.getAccessor("SLN_VAA");
	const Accessor& vzaSlo = sloTiepointSegment.getAccessor("SLO_VZA");
	const Accessor& vaaSlo = sloTiepointSegment.getAccessor("SLO_VAA");

	copy(szaOlc.getDoubles(), szaTiePointsOlc);
	copy(saaOlc.getDoubles(), saaTiePointsOlc);
	copy(vzaOlc.getDoubles(), vzaTiePointsOlc);
	copy(vaaOlc.getDoubles(), vaaTiePointsOlc);
	copy(vzaSln.getDoubles(), vzaTiePointsSln);
	copy(vzaSlo.getDoubles(), vzaTiePointsSlo);
	copy(vaaSln.getDoubles(), vaaTiePointsSln);
	copy(vaaSlo.getDoubles(), vaaTiePointsSlo);

	copy(olciTiepointSegment.getAccessor("ozone").getDoubles(), ozoneTiePoints);
	copy(olciTiepointSegment.getAccessor("air_pressure").getDoubles(), airPressureTiePoints);

	if (olciTiepointSegment.hasVariable("water_vapour")) {
		copy(olciTiepointSegment.getAccessor("water_vapour").getDoubles(), waterVapourTiePoints);
	}
}

PixelProvider::~PixelProvider() {
}

Pixel& PixelProvider::getPixel(size_t index, Pixel& p, bool aerosolOnly) const {
	if (!aerosolOnly) {
		/*
		 * Radiances
		 */
		for (size_t b = 0; b < 30; b++) {
			const Accessor& radiance = *radianceAccessors[b];
			if (!radiance.isFillValue(index)) {
				p.radiances[b] = radiance.getDouble(index);
			} else {
				p.radiances[b] = Constants::FILL_VALUE_DOUBLE;
			}
		}
		/*
		 * Solar irradiances
		 */
		for (size_t i = 0; i < 18; i++) {
			const Accessor& accessor = *solarIrradianceOlcAccessors[i];
			if (!accessor.isFillValue(index)) {
				p.solarIrradiances[i] = accessor.getDouble(index);
			} else {
				p.solarIrradiances[i] = Constants::FILL_VALUE_DOUBLE;
			}
		}
		for (size_t i = 0; i < 6; i++) {
			const Accessor& accessor = *solarIrradianceSlnAccessors[i];
			if (!accessor.isFillValue(index)) {
				p.solarIrradiances[i + 18] = accessor.getDouble(index);
			} else {
				p.solarIrradiances[i + 18] = Constants::FILL_VALUE_DOUBLE;
			}
		}
		for (size_t i = 0; i < 6; i++) {
			const Accessor& accessor = *solarIrradianceSloAccessors[i];
			if (!accessor.isFillValue(index)) {
				p.solarIrradiances[i + 24] = accessor.getDouble(index);
			} else {
				p.solarIrradiances[i + 24] = Constants::FILL_VALUE_DOUBLE;
			}
		}
		/*
		 * Geo-location
		 */
		p.lat = lat.getDouble(index);
		p.lon = lon.getDouble(index);

		/*
		 * Tie Point data
		 */
		valarray<double> tpiWeights(1);
		valarray<size_t> tpiIndexes(1);
		tiePointInterpolatorOlc.prepare(p.lon, p.lat, tpiWeights, tpiIndexes);

		p.sza = tiePointInterpolatorOlc.interpolate(szaTiePointsOlc, tpiWeights, tpiIndexes);
		p.saa = tiePointInterpolatorOlc.interpolate(saaTiePointsOlc, tpiWeights, tpiIndexes);
		p.vzaOlc = tiePointInterpolatorOlc.interpolate(vzaTiePointsOlc, tpiWeights, tpiIndexes);
		p.vaaOlc = tiePointInterpolatorOlc.interpolate(vaaTiePointsOlc, tpiWeights, tpiIndexes);

		p.airPressure = tiePointInterpolatorOlc.interpolate(airPressureTiePoints, tpiWeights, tpiIndexes);
		p.ozone = tiePointInterpolatorOlc.interpolate(ozoneTiePoints, tpiWeights, tpiIndexes);
		if (waterVapourTiePoints.size() != 0) {
			p.waterVapour = tiePointInterpolatorOlc.interpolate(waterVapourTiePoints, tpiWeights, tpiIndexes);
		} else {
			p.waterVapour = 0.2;
		}

		tiePointInterpolatorSln.prepare(p.lon, p.lat, tpiWeights, tpiIndexes);
		p.vaaSln = tiePointInterpolatorSln.interpolate(vaaTiePointsSln, tpiWeights, tpiIndexes);
		p.vzaSln = tiePointInterpolatorSln.interpolate(vzaTiePointsSln, tpiWeights, tpiIndexes);

		tiePointInterpolatorSlo.prepare(p.lon, p.lat, tpiWeights, tpiIndexes);
		p.vaaSlo = tiePointInterpolatorSlo.interpolate(vaaTiePointsSlo, tpiWeights, tpiIndexes);
		p.vzaSlo = tiePointInterpolatorSlo.interpolate(vzaTiePointsSlo, tpiWeights, tpiIndexes);

		p.errorMetric = numeric_limits<double>::max();
	}

	/*
	 * Aerosol properties
	 */
	p.aot = tau550.isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : tau550.getDouble(index);
	p.aotError = tau550Error.isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : tau550Error.getDouble(index);
	p.angstromExponent = alpha550.isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : alpha550.getDouble(index);
	p.flags = synFlags.getUShort(index);
	p.aerosolModel = amin.getUByte(index);

	return p;
}

void PixelProvider::putPixel(size_t index, const Pixel& p) {
	amin.setUByte(index, p.aerosolModel);
	synFlags.setUShort(index, p.flags);
	if (p.aot == Constants::FILL_VALUE_DOUBLE) {
		tau550.setFillValue(index);
	} else {
		tau550.setDouble(index, p.aot);
	}
	if (p.aotError == Constants::FILL_VALUE_DOUBLE) {
		tau550Error.setFillValue(index);
	} else {
		tau550Error.setDouble(index, p.aotError);
	}
	if (p.angstromExponent == Constants::FILL_VALUE_DOUBLE) {
		alpha550.setFillValue(index);
	} else {
		alpha550.setDouble(index, p.angstromExponent);
	}
}

Aer::Aer() :
		BasicModule("AER") {
}

Aer::~Aer() {
}

void Aer::start(Context& context) {
	readAuxiliaryData(context);
	averagedSegment = &context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
	SegmentDescriptor& collocatedSegmentDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550"));
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550_er"));
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("A550"));
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("AMIN"));

	averagedGrid = &averagedSegment->getGrid();

	getAuxdataProvider(context, Constants::AUX_ID_SYCP).getDouble("ave_square", averagingFactor);
}

void Aer::process(Context& context) {
	const long firstL = context.getFirstComputableL(*averagedSegment, *this);
	context.getLogging().debug("Segment [" + averagedSegment->toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
	long lastL = context.getLastComputableL(*averagedSegment, *this);
	context.getLogging().debug("Segment [" + averagedSegment->toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

	ErrorMetric em(context);
	PixelProvider pixelProvider(context);

	for (long l = firstL; l <= lastL; l++) {
		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l), getId());

		Pixel p;
		Pixel q;

		for (long k = averagedGrid->getMinK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getMinM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t pixelIndex = averagedGrid->getIndex(k, l, m);
				pixelProvider.getPixel(pixelIndex, p);

				if (isSet(p.flags, Constants::SY2_AEROSOL_SUCCESS_FLAG) || isSet(p.flags, Constants::SY2_AEROSOL_TOO_LOW_FLAG) || isSet(p.flags, Constants::SY2_AEROSOL_HIGH_ERROR_FLAG) || isSet(p.flags, Constants::SY2_AEROSOL_FILLED_FLAG)) {
					continue;
				}
				if (isSet(p.flags, Constants::SY2_PARTLY_CLOUDY_FLAG) || isSet(p.flags, Constants::SY2_PARTLY_WATER_FLAG)) {
					p.aot = Constants::FILL_VALUE_DOUBLE;
					p.aotError = Constants::FILL_VALUE_DOUBLE;
					p.angstromExponent = Constants::FILL_VALUE_DOUBLE;
				} else {
					retrieveAerosolProperties(p, q, em);
				}
				pixelProvider.putPixel(pixelIndex, p);
			}
		}
	}

	const long n = 120;
		// shall be at least 240 km, but cannot be larger than half the height of the segment
	long lastFillableL;
	if (lastL < averagedGrid->getMaxL()) {
		lastFillableL = lastL - n;
	} else {
		lastFillableL = lastL;
	}

#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) > 40100
#pragma omp parallel for
#endif
	for (long targetL = firstL; targetL <= lastFillableL; targetL++) {
		context.getLogging().info("Filling line l = " + lexical_cast<string>(targetL), getId());

		Pixel p;
		Pixel q;

		for (long targetK = averagedGrid->getMinK(); targetK <= averagedGrid->getMaxK(); targetK++) {
			for (long targetM = averagedGrid->getMinM(); targetM <= averagedGrid->getMaxM(); targetM++) {
                const long targetN = targetM + averagedGrid->getSizeM() * targetK;
				const size_t targetPixelIndex = averagedGrid->getIndex(targetK, targetL, targetM);
				pixelProvider.getPixel(targetPixelIndex, p, true);

				if (!(isSet(p.flags, Constants::SY2_AEROSOL_SUCCESS_FLAG)
				        || isSet(p.flags, Constants::SY2_AEROSOL_HIGH_ERROR_FLAG)
				        || isSet(p.flags, Constants::SY2_AEROSOL_TOO_LOW_FLAG))) {
					double ws = 0.00000625;
					double aot = aerosolOpticalThickness(p.lat);
					double aotError = 0.0;
					double angstromExponent = 1.25;
					double minPixelDistance = numeric_limits<double>::max();

					for (long sourceL = targetL - n; sourceL <= targetL + n; sourceL++) {
						for (long sourceN = targetN - n; sourceN <= targetN + n; sourceN++) {
                            const long sourceK = sourceN / averagedGrid->getSizeM();
                            const long sourceM = sourceN - sourceK * averagedGrid->getSizeM();
							if (averagedGrid->isValidPosition(sourceK, sourceL, sourceM)) {
								const size_t sourcePixelIndex = averagedGrid->getIndex(sourceK, sourceL, sourceM);
								pixelProvider.getPixel(sourcePixelIndex, q, true);
								if (isSet(q.flags, Constants::SY2_AEROSOL_SUCCESS_FLAG)) {
									const double d = (sourceL - targetL) * (sourceL - targetL) + (sourceN - targetN) * (sourceN - targetN);
									if (d < minPixelDistance) {
										minPixelDistance = d;
										p.aerosolModel = q.aerosolModel;
									}
									const double w = 1.0 / (d * d);
									const double t = w + ws;
									aot += (q.aot - aot) * w / t;
									aotError += (q.aotError - aotError) * w / t;
									angstromExponent += (q.angstromExponent - angstromExponent) * w / t;
									ws = t;
								}
							}
						}
					}
					p.aot = aot;
					p.aotError = aotError;
					p.angstromExponent = angstromExponent;
					p.flags |= Constants::SY2_AEROSOL_FILLED_FLAG;
					pixelProvider.putPixel(targetPixelIndex, p);
				}
			}
		}
	}

	context.setLastComputedL(*averagedSegment, *this, lastFillableL);
}

void Aer::retrieveAerosolProperties(Pixel& p, Pixel& q, ErrorMetric& em) {
	using std::sqrt;

	for (size_t i = 0; i < amins.size(); i++) {
		const int16_t amin = amins[i];
		q.assign(p);
		q.aerosolModel = amin;
		bool success = em.findMinimum(q);
		if (success && q.errorMetric < p.errorMetric) {
			p.assign(q);
			p.angstromExponent = aerosolAngstromExponents[amin];
			p.aerosolModel = amin;
		}
	}
	if (p.aerosolModel > 0) {
		if (p.aot > 0.0001) {
			double a = em.computeErrorSurfaceCurvature(p);
			if (a > 0.0) {
				p.aotError = kappa * sqrt(p.errorMetric / a);
				if (p.aot > 0.1 && p.aotError > 5.0 * p.aot) {
					p.flags |= Constants::SY2_AEROSOL_HIGH_ERROR_FLAG;
				} else {
					p.flags |= Constants::SY2_AEROSOL_SUCCESS_FLAG;
				}
			} else {
				p.aotError = Constants::FILL_VALUE_DOUBLE;
				p.flags |= Constants::SY2_AEROSOL_NEGATIVE_CURVATURE_FLAG;
			}
		} else {
			p.aotError = Constants::FILL_VALUE_DOUBLE;
			p.flags |= Constants::SY2_AEROSOL_TOO_LOW_FLAG;
		}
	} else {
		p.aot = Constants::FILL_VALUE_DOUBLE;
		p.aotError = Constants::FILL_VALUE_DOUBLE;
		p.angstromExponent = Constants::FILL_VALUE_DOUBLE;
	}
}

void Aer::readAuxiliaryData(Context& context) {
	getLookupTable(context, Constants::AUX_ID_SYRT, "OLC_R_atm");
	getLookupTable(context, Constants::AUX_ID_SYRT, "SLN_R_atm");
	getLookupTable(context, Constants::AUX_ID_SYRT, "SLO_R_atm");
	getLookupTable(context, Constants::AUX_ID_SYRT, "t");
	getLookupTable(context, Constants::AUX_ID_SYRT, "rho_atm");
	getLookupTable(context, Constants::AUX_ID_SYRT, "D");
	getLookupTable(context, Constants::AUX_ID_SYCP, "weight_ang_tot");

	AuxdataProvider& cpAuxdataProvider = getAuxdataProvider(context, Constants::AUX_ID_SYCP);
	AuxdataProvider& rtAuxdataProvider = getAuxdataProvider(context, Constants::AUX_ID_SYRT);

	cpAuxdataProvider.getVectorShort("AMIN", amins);
	cpAuxdataProvider.getDouble("kappa", kappa);
	rtAuxdataProvider.getVectorDouble("A550", aerosolAngstromExponents);
}

double Aer::aerosolOpticalThickness(double lat) {
	const double c = cos(lat * D2R);
	return 0.2 * (c - 0.25) * (c * c * c) + 0.05;
}

