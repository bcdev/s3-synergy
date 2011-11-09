/*
 * Aer.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>
#include <iostream>

#include "../core/TiePointInterpolator.h"
#include "../util/ErrorMetric.h"
#include "../util/LookupTableReader.h"
#include "../util/AuxdataProvider.h"

#include "Aer.h"

using std::copy;
using std::min;
using std::numeric_limits;
using std::set;

static void copyValarray(const std::valarray<double>& source, std::valarray<double>& target) {
	target.resize(source.size());
	copy(&source[0], &source[source.size()], &target[0]);
}

class PixelInitializer {

public:
	PixelInitializer(const Context& context);
	~PixelInitializer();
	Pixel& getPixel(long k, long l, long m, Pixel& p) const;

private:
	const Context& context;

	const Segment& averagedSegment;
	const Segment& olcInfoSegment;
	const Segment& slnInfoSegment;
	const Segment& sloInfoSegment;

	const Grid& averagedGrid;
	const Grid& olcInfoGrid;
	const Grid& slnInfoGrid;
	const Grid& sloInfoGrid;

	const Accessor& synFlags;
	const Accessor& lat;
	const Accessor& lon;

	const TiePointInterpolator<double> tiePointInterpolatorOlc;
	const TiePointInterpolator<double> tiePointInterpolatorSln;
	const TiePointInterpolator<double> tiePointInterpolatorSlo;

	vector<Accessor*> radiances;
	vector<Accessor*> solarIrradiancesOlc;
	vector<Accessor*> solarIrradiancesSln;
	vector<Accessor*> solarIrradiancesSlo;

	const valarray<double>& cO3;

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
		context(context), averagedSegment(context.getSegment(Constants::SEGMENT_SYN_AVERAGED)), olcInfoSegment(context.getSegment(Constants::SEGMENT_OLC_INFO)), slnInfoSegment(
				context.getSegment(Constants::SEGMENT_SLN_INFO)), sloInfoSegment(context.getSegment(Constants::SEGMENT_SLO_INFO)), averagedGrid(averagedSegment.getGrid()), olcInfoGrid(
				olcInfoSegment.getGrid()), slnInfoGrid(slnInfoSegment.getGrid()), sloInfoGrid(sloInfoSegment.getGrid()), synFlags(averagedSegment.getAccessor("SYN_flags")), lat(
				averagedSegment.getAccessor("latitude")), lon(averagedSegment.getAccessor("longitude")), tiePointInterpolatorOlc(
				context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_OLC_TP).getAccessor("OLC_TP_lat").getDoubles()), tiePointInterpolatorSln(
				context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_SLN_TP).getAccessor("SLN_TP_lat").getDoubles()), tiePointInterpolatorSlo(
				context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lon").getDoubles(), context.getSegment(Constants::SEGMENT_SLO_TP).getAccessor("SLO_TP_lat").getDoubles()), cO3(
				((AuxdataProvider&) context.getObject(Constants::AUX_ID_SYRTAX)).getVectorDouble("C_O3")) {
	for (size_t b = 0; b < 30; b++) {
		radiances.push_back(&averagedSegment.getAccessor("L_" + lexical_cast<string>(b + 1)));
	}
	for (size_t b = 0; b < 18; b++) {
		solarIrradiancesOlc.push_back(&averagedSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
	}
	for (size_t b = 18; b < 24; b++) {
		solarIrradiancesSln.push_back(&averagedSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
	}
	for (size_t b = 24; b < 30; b++) {
		solarIrradiancesSlo.push_back(&averagedSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(b + 1)));
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

	copyValarray(szaOlc.getDoubles(), szaTiePointsOlc);
	copyValarray(saaOlc.getDoubles(), saaTiePointsOlc);
	copyValarray(vzaOlc.getDoubles(), vzaTiePointsOlc);
	copyValarray(vaaOlc.getDoubles(), vaaTiePointsOlc);
	copyValarray(vzaSln.getDoubles(), vzaTiePointsSln);
	copyValarray(vzaSlo.getDoubles(), vzaTiePointsSlo);
	copyValarray(vaaSln.getDoubles(), vaaTiePointsSln);
	copyValarray(vaaSlo.getDoubles(), vaaTiePointsSlo);

	copyValarray(olciTiepointSegment.getAccessor("ozone").getDoubles(), ozoneTiePoints);
	copyValarray(olciTiepointSegment.getAccessor("air_pressure").getDoubles(), airPressureTiePoints);

	if (olciTiepointSegment.hasVariable("water_vapour")) {
		copyValarray(olciTiepointSegment.getAccessor("water_vapour").getDoubles(), waterVapourTiePoints);
	}
}

PixelInitializer::~PixelInitializer() {
}

Pixel& PixelInitializer::getPixel(long k, long l, long m, Pixel& p) const {
	p.k = k;
	p.l = l;
	p.m = m;
	p.index = averagedGrid.getIndex(k, l, m);

	/*
	 * Radiances
	 */
	for (size_t b = 0; b < 30; b++) {
		const Accessor& radiance = *radiances[b];
		if (!radiance.isFillValue(p.index)) {
			p.radiances[b] = radiance.getDouble(p.index);
		} else {
			p.radiances[b] = Constants::FILL_VALUE_DOUBLE;
		}
	}
	/*
	 * Solar irradiances
	 */
	for (size_t i = 0; i < 18; i++) {
		const Accessor& accessor = *solarIrradiancesOlc[i];
		if (!accessor.isFillValue(p.index)) {
			p.solarIrradiances[i] = accessor.getDouble(p.index);
		} else {
			p.solarIrradiances[i] = Constants::FILL_VALUE_DOUBLE;
		}
	}
	for (size_t i = 0; i < 6; i++) {
		const Accessor& accessor = *solarIrradiancesSln[i];
		if (!accessor.isFillValue(p.index)) {
			p.solarIrradiances[i + 18] = accessor.getDouble(p.index);
		} else {
			p.solarIrradiances[i + 18] = Constants::FILL_VALUE_DOUBLE;
		}
	}
	for (size_t i = 0; i < 6; i++) {
		const Accessor& accessor = *solarIrradiancesSlo[i];
		if (!accessor.isFillValue(p.index)) {
			p.solarIrradiances[i + 24] = accessor.getDouble(p.index);
		} else {
			p.solarIrradiances[i + 24] = Constants::FILL_VALUE_DOUBLE;
		}
	}
	/*
	 * Ozone coefficients
	 */
	// TODO - remove from pixel
	for (size_t i = 0; i < 30; i++) {
		p.cO3[i] = cO3[i];
	}

	/*
	 * Flags
	 */
	p.synFlags = synFlags.getUShort(p.index);

	/*
	 * Geo-location
	 */
	p.lat = lat.getDouble(p.index);
	p.lon = lon.getDouble(p.index);

	/*
	 * Tie Point data
	 */
	valarray<double> tpiWeights(1);
	valarray<size_t> tpiIndexes(1);
	tiePointInterpolatorOlc.prepare(p.lat, p.lon, tpiWeights, tpiIndexes);

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

	tiePointInterpolatorSln.prepare(p.lat, p.lon, tpiWeights, tpiIndexes);
	p.vaaSln = tiePointInterpolatorSln.interpolate(vaaTiePointsSln, tpiWeights, tpiIndexes);
	p.vzaSln = tiePointInterpolatorSln.interpolate(vzaTiePointsSln, tpiWeights, tpiIndexes);

	tiePointInterpolatorSlo.prepare(p.lat, p.lon, tpiWeights, tpiIndexes);
	p.vaaSlo = tiePointInterpolatorSlo.interpolate(vaaTiePointsSlo, tpiWeights, tpiIndexes);
	p.vzaSlo = tiePointInterpolatorSlo.interpolate(vzaTiePointsSlo, tpiWeights, tpiIndexes);

	/*
	 * Anything else
	 */
	p.tau550 = Constants::FILL_VALUE_DOUBLE;
	p.tau550Error = Constants::FILL_VALUE_DOUBLE;
	p.tau550Filtered = Constants::FILL_VALUE_DOUBLE;
	p.tau550ErrorFiltered = Constants::FILL_VALUE_DOUBLE;
	p.alpha550 = Constants::FILL_VALUE_DOUBLE;
	p.amin = numeric_limits<short>::min();
	p.E2 = numeric_limits<double>::max();

	return p;
}

Aer::Aer() :
		BasicModule("AER") {
}

Aer::~Aer() {
}

void Aer::start(Context& context) {
	readAuxdata(context);
	averagedSegment = &context.getSegment(Constants::SEGMENT_SYN_AVERAGED);
	SegmentDescriptor& collocatedSegmentDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550"));
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("T550_er"));
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("A550"));
	averagedSegment->addVariable(collocatedSegmentDescriptor.getVariableDescriptor("AMIN"));
	averagedSegment->addVariable("T550_filtered", Constants::TYPE_DOUBLE);
	averagedSegment->addVariable("T550_er_filtered", Constants::TYPE_DOUBLE);

	averagedGrid = &averagedSegment->getGrid();
}

void Aer::process(Context& context) {
	const long firstL = context.getFirstComputableL(*averagedSegment, *this);
	long lastL = context.getLastComputableL(*averagedSegment, *this);

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

				if (isSet(p.synFlags, Constants::SY2_PARTLY_CLOUDY_FLAG)) {
					continue;
				}
				if (isSet(p.synFlags, Constants::SY2_PARTLY_WATER_FLAG)) {
					continue;
				}
				if (isSet(p.synFlags, Constants::SY2_AEROSOL_SUCCESS_FLAG)) {
					continue;
				}
				if (isSet(p.synFlags, Constants::SY2_AEROSOL_FILLED_FLAG)) {
					continue;
				}

				retrieveAerosolProperties(p, q, em);
			}
		}
	}

	long lastFillableL;
	if (lastL < averagedGrid->getMaxL()) {
		lastFillableL = lastL - 9;
	} else {
		lastFillableL = lastL;
	}

	size_t iterationCount = 0;
	unsigned n = 1;
	size_t missingPixelCount;

	do {
		missingPixelCount = 0;
		if (iterationCount >= 5 && iterationCount <= 12) {
			n++;
		}
		for (long targetL = firstL; targetL <= lastFillableL; targetL++) {
			context.getLogging().info("Filling line l = " + lexical_cast<string>(targetL), getId());
			for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
				for (long targetM = averagedGrid->getFirstM(); targetM <= averagedGrid->getMaxM(); targetM++) {
					const size_t targetPixelIndex = averagedGrid->getIndex(k, targetL, targetM);
					Pixel& p = pixels[targetPixelIndex];

					if (!isSet(p.synFlags, Constants::SY2_AEROSOL_SUCCESS_FLAG) && !isSet(p.synFlags, Constants::SY2_AEROSOL_FILLED_FLAG)) {
						missingPixelCount++;

						double tau550 = 0.0;
						double tau550err = 0.0;
						double alpha550 = 0.0;
						size_t pixelCount = 0;
						long minPixelDistance = numeric_limits<long>::max();

						for (long sourceL = targetL - n; sourceL <= targetL + n; sourceL++) {
							for (long sourceM = targetM - n; sourceM <= targetM + n; sourceM++) {
								if (averagedGrid->isValidPosition(k, sourceL, sourceM)) {
									const size_t sourcePixelIndex = averagedGrid->getIndex(k, sourceL, sourceM);
									const Pixel& q = pixels[sourcePixelIndex];
									if (isSet(q.synFlags, Constants::SY2_AEROSOL_SUCCESS_FLAG) || isSet(q.synFlags, Constants::SY2_AEROSOL_FILLED_FLAG)) {
										const long dist = (sourceL - targetL) * (sourceL - targetL) + (sourceM - targetM) * (sourceM - targetM);
										if (dist < minPixelDistance) {
											minPixelDistance = dist;
											p.amin = q.amin;
										}

										tau550 += q.tau550;
										tau550err += q.tau550Error;
										alpha550 += q.alpha550;

										pixelCount++;
									}
								}
							}
						}
						if (pixelCount > 0) {
							p.tau550 = tau550 / pixelCount;
							p.tau550Error = tau550err / pixelCount;
							p.alpha550 = alpha550 / pixelCount;
							p.synFlags |= Constants::SY2_AEROSOL_FILLED_FLAG;
						}
					}
				}
			}
		}
		context.getLogging().info(lexical_cast<string>(missingPixelCount) + " pixels filled", getId());
		iterationCount++;
	} while (missingPixelCount != 0);

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
						tauValues[i] = p.tau550;
						errValues[i] = p.tau550Error;
						i++;
					}
				}
				const size_t h = i / 2;
				std::nth_element(&tauValues[0], &tauValues[h], &tauValues[i]);
				std::nth_element(&errValues[0], &errValues[h], &errValues[i]);
				p.tau550Filtered = tauValues[h];
				p.tau550ErrorFiltered = errValues[h];
			}
		}
	}

	context.getLogging().info("Putting lines ...", getId());
	putPixels(pixels, firstL, lastL);

	context.setLastComputedL(*averagedSegment, *this, lastFilterableL);
	if (lastFilterableL < averagedGrid->getMaxL()) {
		context.setFirstRequiredL(*averagedSegment, *this, lastFilterableL - 10);
	} else {
		context.setFirstRequiredL(*averagedSegment, *this, -1);
	}
}

void Aer::getPixels(Context& context, valarray<Pixel>& pixels) const {
	const PixelInitializer pixelInitializer(context);
	for (long l = averagedGrid->getFirstL(); l <= averagedGrid->getLastL(); l++) {
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t index = averagedGrid->getIndex(k, l, m);
				pixelInitializer.getPixel(k, l, m, pixels[index]);
			}
		}
	}
}

void Aer::retrieveAerosolProperties(Pixel& p, Pixel& q, ErrorMetric& em) {
	using std::sqrt;

	for (size_t i = 0; i < amins->size(); i++) {
		const int16_t amin = (*amins)[i];
		q.assign(p);
		q.amin = amin;
		bool success = em.findMinimum(q);
		if (success && q.E2 < p.E2) {
			p.assign(q);
			p.alpha550 = (*aerosolAngstromExponents)[amin];
			p.amin = amin;
		}
	}
	if (p.amin > 0) {
		if (p.tau550 > 0.0001) {
			double a = em.computeErrorSurfaceCurvature(p);
			if (a > 0.0) {
				p.tau550Error = kappa * sqrt(p.E2 / a);
				if (p.tau550 > 0.1 && p.tau550Error > 5.0 * p.tau550) {
					p.synFlags |= Constants::SY2_AEROSOL_HIGH_ERROR_FLAG;
				} else {
					p.synFlags |= Constants::SY2_AEROSOL_SUCCESS_FLAG;
				}
			} else {
				p.synFlags |= Constants::SY2_AEROSOL_NEGATIVE_CURVATURE_FLAG;
			}
		} else {
			p.synFlags |= Constants::SY2_AEROSOL_TOO_LOW_FLAG;
		}
	}
}

void Aer::readAuxdata(Context& context) {
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

	amins = &cpAuxdataProvider.getVectorShort("AMIN");
	kappa = cpAuxdataProvider.getDouble("kappa");
	aerosolAngstromExponents = &rtAuxdataProvider.getVectorDouble("A550");
}

void Aer::putPixels(const valarray<Pixel>& pixels, long firstL, long lastL) const {
	Accessor& amin = averagedSegment->getAccessor("AMIN");
	Accessor& t550 = averagedSegment->getAccessor("T550");
	Accessor& t550Err = averagedSegment->getAccessor("T550_er");
	Accessor& a550 = averagedSegment->getAccessor("A550");
	Accessor& synFlags = averagedSegment->getAccessor("SYN_flags");
	Accessor& t550Filtered = averagedSegment->getAccessor("T550_filtered");
	Accessor& t550ErrFiltered = averagedSegment->getAccessor("T550_er_filtered");

	for (long l = firstL; l <= lastL; l++) {
		for (long k = averagedGrid->getFirstK(); k <= averagedGrid->getMaxK(); k++) {
			for (long m = averagedGrid->getFirstM(); m <= averagedGrid->getMaxM(); m++) {
				const size_t index = averagedGrid->getIndex(k, l, m);
				const Pixel& p = pixels[index];

				amin.setUByte(p.index, p.amin);
				if (p.tau550 == Constants::FILL_VALUE_DOUBLE) {
					t550.setFillValue(index);
				} else {
					try {
						t550.setDouble(index, p.tau550);
					} catch (std::exception& e) {
						std::cout << "line " << l << ": " << e.what() << ": " << "t550 = " << p.tau550 << std::endl;
						throw e;
					}
				}
				if (p.tau550Error == Constants::FILL_VALUE_DOUBLE) {
					t550Err.setFillValue(index);
				} else {
					try {
						t550Err.setDouble(index, p.tau550Error);
					} catch (std::exception& e) {
						std::cout << "line " << l << ": " << e.what() << ": " << "t550Err = " << p.tau550Error << std::endl;
						throw e;
					}
				}
				if (p.alpha550 == Constants::FILL_VALUE_DOUBLE) {
					a550.setFillValue(index);
				} else {
					try {
						a550.setDouble(index, p.alpha550);
					} catch (std::exception& e) {
						std::cout << "line " << l << ": " << e.what() << ": " << "a550 = " << p.alpha550 << std::endl;
						throw e;
					}
				}
				synFlags.setUShort(index, p.synFlags);
				if (p.tau550Filtered == Constants::FILL_VALUE_DOUBLE) {
					t550Filtered.setFillValue(index);
				} else {
					try {
						t550Filtered.setDouble(index, p.tau550Filtered);
					} catch (std::exception& e) {
						std::cout << "line " << l << ": " << e.what() << ": " << "t550Filtered = " << p.tau550Filtered << std::endl;
						throw e;
					}
				}
				if (p.tau550ErrorFiltered == Constants::FILL_VALUE_DOUBLE) {
					t550ErrFiltered.setFillValue(index);
				} else {
					try {
						t550ErrFiltered.setDouble(index, p.tau550ErrorFiltered);
					} catch (std::exception& e) {
						std::cout << "line " << l << ": " << e.what() << ": " << "t550ErrorFiltered = " << p.tau550ErrorFiltered << std::endl;
						throw e;
					}
				}
			}
		}
	}
}

