/*
 * Aer.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
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
using std::min;
using std::numeric_limits;

class PixelInitializer {

public:
	PixelInitializer(const Context& context);
	~PixelInitializer();
	Pixel& getPixel(size_t index, Pixel& p) const;

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

	const Accessor& tau550;
	const Accessor& tau550Error;
	const Accessor& alpha550;
	const Accessor& synFlags;
	const Accessor& amin;
	const Accessor& lat;
	const Accessor& lon;

	const TiePointInterpolator<double> tiePointInterpolatorOlc;
	const TiePointInterpolator<double> tiePointInterpolatorSln;
	const TiePointInterpolator<double> tiePointInterpolatorSlo;

	vector<Accessor*> radianceAccessors;
	vector<Accessor*> solarIrradianceOlcAccessors;
	vector<Accessor*> solarIrradianceSlnAccessors;
	vector<Accessor*> solarIrradianceSloAccessors;

	valarray<double> cO3;

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

PixelInitializer::PixelInitializer(const Context& context) :
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
	const AuxdataProvider& rtAuxdataProvider = (AuxdataProvider&) context.getObject(Constants::AUX_ID_SYRTAX);
	rtAuxdataProvider.getVectorDouble("C_O3", cO3);

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

PixelInitializer::~PixelInitializer() {
}

Pixel& PixelInitializer::getPixel(size_t index, Pixel& p) const {
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
	 * Aerosol properties
	 */
	p.aot = tau550.isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : tau550.getDouble(index);
	p.aotError = tau550Error.isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : tau550Error.getDouble(index);
	p.angstromExponent = alpha550.isFillValue(index) ? Constants::FILL_VALUE_DOUBLE : alpha550.getDouble(index);
	p.flags = synFlags.getUShort(index);
	p.aerosolModel = amin.getUByte(index);

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

	/*
	 * Anything else
	 */
	p.errorMetric = numeric_limits<double>::max();

	return p;
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
}

void Aer::process(Context& context) {
	const long firstL = context.getFirstComputableL(*averagedSegment, *this);
	context.getLogging().debug("Segment [" + averagedSegment->toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
	long lastL = context.getLastComputableL(*averagedSegment, *this);
	context.getLogging().debug("Segment [" + averagedSegment->toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

	valarray<Pixel> pixels(averagedGrid->getSize());

	context.getLogging().info("Getting lines ...", getId());
	getPixels(context, pixels);

	ErrorMetric em(context);
	Pixel q;

	for (long l = firstL; l <= lastL; l++) {
		context.getLogging().progress("Retrieving aerosol properties for line l = " + lexical_cast<string>(l), getId());
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t pixelIndex = averagedGrid->getIndex(k, l, m);
				Pixel& p = pixels[pixelIndex];

				if (isSet(p.flags, Constants::SY2_AEROSOL_SUCCESS_FLAG) || isSet(p.flags, Constants::SY2_AEROSOL_TOO_LOW_FLAG) || isSet(p.flags, Constants::SY2_AEROSOL_HIGH_ERROR_FLAG) || isSet(p.flags, Constants::SY2_AEROSOL_FILLED_FLAG)) {
					continue;
				}
				if (isSet(p.flags, Constants::SY2_PARTLY_CLOUDY_FLAG) || isSet(p.flags, Constants::SY2_PARTLY_WATER_FLAG)) {
					p.aot = Constants::FILL_VALUE_DOUBLE;
					p.aotError = Constants::FILL_VALUE_DOUBLE;
					p.angstromExponent = Constants::FILL_VALUE_DOUBLE;
					continue;
				}
				retrieveAerosolProperties(p, q, em);
			}
		}
	}

	const long n = 30;
	long lastFillableL;
	if (lastL < averagedGrid->getMaxL()) {
		lastFillableL = lastL - n;
	} else {
		lastFillableL = lastL;
	}

	for (long targetL = firstL; targetL <= lastFillableL; targetL++) {
		context.getLogging().info("Filling line l = " + lexical_cast<string>(targetL), getId());
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long targetM = averagedGrid->getFirstM(); targetM <= averagedGrid->getMaxM(); targetM++) {
				const size_t targetPixelIndex = averagedGrid->getIndex(k, targetL, targetM);
				Pixel& p = pixels[targetPixelIndex];

				if (!isSet<int>(p.flags, Constants::SY2_AEROSOL_SUCCESS_FLAG | Constants::SY2_AEROSOL_HIGH_ERROR_FLAG | Constants::SY2_AEROSOL_TOO_LOW_FLAG)) {
					double w = 0.00000625;
					double tau550 = w * aerosolOpticalThickness(p.lat, p.lon);
					double tau550err = 0.0 * w;
					double alpha550 = 1.25 * w;
					double minPixelDistance = numeric_limits<double>::max();

					for (long sourceL = targetL - n; sourceL <= targetL + n; sourceL++) {
						for (long sourceM = targetM - n; sourceM <= targetM + n; sourceM++) {
							if (averagedGrid->isValidPosition(k, sourceL, sourceM)) {
								const size_t sourcePixelIndex = averagedGrid->getIndex(k, sourceL, sourceM);
								const Pixel& q = pixels[sourcePixelIndex];
								if (isSet(q.flags, Constants::SY2_AEROSOL_SUCCESS_FLAG)) {
									const double d = (sourceL - targetL) * (sourceL - targetL) + (sourceM - targetM) * (sourceM - targetM);
									if (d < minPixelDistance) {
										minPixelDistance = d;
										p.aerosolModel = q.aerosolModel;
									}

									tau550 += q.aot / (d * d);
									tau550err += q.aotError / (d * d);
									alpha550 += q.angstromExponent / (d * d);
									w += 1.0 / (d * d);
								}
							}
						}
					}
					p.aot = tau550 / w;
					p.aotError = tau550err / w;
					p.angstromExponent = alpha550 / w;
					p.flags |= Constants::SY2_AEROSOL_FILLED_FLAG;
				}
			}
		}
	}

	/*
	long lastFilterableL;
	if (lastL < averagedGrid->getMaxL()) {
		lastFilterableL = lastFillableL - 1;
	} else {
		lastFilterableL = lastL;
	}

	valarray<double> tauValues(9);
	valarray<double> errValues(9);
	for (long targetL = firstL; targetL <= lastFilterableL; targetL++) {
		context.getLogging().info("Filtering line l = " + lexical_cast<string>(targetL), getId());

		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long targetM = averagedGrid->getFirstM(); targetM <= averagedGrid->getMaxM(); targetM++) {
				const size_t pixelIndex = averagedGrid->getIndex(k, targetL, targetM);
				Pixel& p = pixels[pixelIndex];

				size_t i = 0;
				for (long sourceL = targetL - 1; sourceL <= targetL + 1; sourceL++) {
					for (long sourceM = targetM - 1; sourceM <= targetM + 1; sourceM++) {
						if (!averagedGrid->isValidPosition(k, sourceL, sourceM)) {
							continue;
						}
						tauValues[i] = p.aot;
						errValues[i] = p.aotError;
						i++;
					}
				}
				const size_t h = i / 2;
				std::nth_element(&tauValues[0], &tauValues[h], &tauValues[i]);
				std::nth_element(&errValues[0], &errValues[h], &errValues[i]);
				p.aotFiltered = tauValues[h];
				p.aotErrorFiltered = errValues[h];
			}
		}
	}
	*/

	context.getLogging().info("Putting lines ...", getId());
	putPixels(pixels, firstL, lastL);

	context.setLastComputedL(*averagedSegment, *this, lastFillableL);
	context.setFirstRequiredL(*averagedSegment, *this, lastFillableL + 1 - n);
}

void Aer::getPixels(Context& context, valarray<Pixel>& pixels) const {
	const PixelInitializer pixelInitializer(context);
	for (long l = averagedGrid->getFirstL(); l <= averagedGrid->getLastL(); l++) {
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t index = averagedGrid->getIndex(k, l, m);
				pixelInitializer.getPixel(index, pixels[index]);
			}
		}
	}
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
			p.a = a;
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
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "OLC_R_atm");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLN_R_atm");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "SLO_R_atm");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "t");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "rho_atm");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "D");
	getLookupTable(context, "S3__SY_2_SYCPAX.nc", "weight_ang_tot");
	getLookupTable(context, "S3__SY_2_SYRTAX.nc", "C_O3");

	AuxdataProvider& cpAuxdataProvider = getAuxdataProvider(context, Constants::AUX_ID_SYCPAX);
	AuxdataProvider& rtAuxdataProvider = getAuxdataProvider(context, Constants::AUX_ID_SYRTAX);

	cpAuxdataProvider.getVectorShort("AMIN", amins);
	cpAuxdataProvider.getDouble("kappa", kappa);
	rtAuxdataProvider.getVectorDouble("A550", aerosolAngstromExponents);
}

void Aer::putPixels(const valarray<Pixel>& pixels, long firstL, long lastL) const {
	Accessor& aotAccessor = averagedSegment->getAccessor("T550");
	Accessor& aotErrorAccessor = averagedSegment->getAccessor("T550_er");
	Accessor& angstromExponentAccessor = averagedSegment->getAccessor("A550");
	Accessor& aerosolModelAccessor = averagedSegment->getAccessor("AMIN");
	Accessor& flagsAccessor = averagedSegment->getAccessor("SYN_flags");

	for (long l = firstL; l <= lastL; l++) {
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t index = averagedGrid->getIndex(k, l, m);
				const Pixel& p = pixels[index];

				aerosolModelAccessor.setUByte(index, p.aerosolModel);
				flagsAccessor.setUShort(index, p.flags);
				if (p.aot == Constants::FILL_VALUE_DOUBLE) {
					aotAccessor.setFillValue(index);
				} else {
					aotAccessor.setDouble(index, p.aot);
				}
				if (p.aotError == Constants::FILL_VALUE_DOUBLE) {
					aotErrorAccessor.setFillValue(index);
				} else {
					aotErrorAccessor.setDouble(index, p.aotError);
				}
				if (p.angstromExponent == Constants::FILL_VALUE_DOUBLE) {
					angstromExponentAccessor.setFillValue(index);
				} else {
					angstromExponentAccessor.setDouble(index, p.angstromExponent);
				}
			}
		}
	}
}

double Aer::aerosolOpticalThickness(double lat, double lon) {
	const double c = cos(lat * D2R);
	return 0.2 * (c - 0.25) * c * c * c + 0.05;
}

