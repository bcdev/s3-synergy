/*
 * Ave.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: thomasstorm
 */

#include <algorithm>
#include <cmath>

#include "Ave.h"

Ave::Ave() :
		BasicModule("AVE") {
}

Ave::~Ave() {
}

void Ave::start(Context& context) {
	using std::ceil;

	averagingFactor = getAuxdataProvider(context, Constants::AUX_ID_SYCPAX).getUByte("ave_square");
	sourceSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Grid& sourceGrid = sourceSegment->getGrid();
	const size_t sizeL = sourceGrid.getSizeL() / averagingFactor;
	const size_t sizeM = ceil(sourceGrid.getSizeM() / double(averagingFactor));
	const size_t sizeK = sourceGrid.getSizeK();
	const size_t maxL = ceil((sourceGrid.getMaxL() - sourceGrid.getMinL() + 1) / double(averagingFactor)) - 1;
	targetSegment = &context.addSegment(Constants::SEGMENT_SYN_AVERAGED, sizeL, sizeM, sizeK, 0, maxL);

	addVariables(context);
}

void Ave::process(Context& context) {
	using std::min;

	const long lastSourceL = context.getLastComputableL(*sourceSegment, *this);
	const long firstTargetL = context.getFirstComputableL(*targetSegment, *this);
	long lastTargetL = (lastSourceL - sourceSegment->getGrid().getMinL() + 1) / averagingFactor;
	if (lastSourceL < sourceSegment->getGrid().getMaxL()) {
		lastTargetL--;
	}
	lastTargetL = min(lastTargetL, context.getLastComputableL(*targetSegment, *this));

	context.getLogging().debug("Segment [" + targetSegment->toString() + "]: firstComputableL = " + lexical_cast<string>(firstTargetL), getId());
	context.getLogging().debug("Segment [" + targetSegment->toString() + "]: lastComputableL = " + lexical_cast<string>(lastTargetL), getId());

	averageVariables(context.getLogging(), firstTargetL, lastTargetL);

	context.setLastComputedL(*targetSegment, *this, lastTargetL);
	context.setFirstRequiredL(*sourceSegment, *this, (lastTargetL + 1) * averagingFactor);
}

void Ave::toLatLon(double x, double y, double z, double& lat, double& lon) {
	using std::sqrt;

	const double length = std::sqrt(x * x + y * y + z * z);

	x /= length;
	y /= length;
	z /= length;

	lat = std::asin(z) * R2D;
	lon = std::atan2(y, x) * R2D;
}

void Ave::accumulateLatLon(double lat, double lon, double& x, double& y, double& z) {
	using std::cos;
	using std::sin;

	const double u = lon * D2R;
	const double v = lat * D2R;
	const double w = cos(v);

	x += cos(u) * w;
	y += sin(u) * w;
	z += sin(v);
}

void Ave::averageVariables(Logging& logging, long firstL, long lastL) {
	const Grid& sourceGrid = sourceSegment->getGrid();
	const Grid& targetGrid = targetSegment->getGrid();

	const Accessor& sourceFlagsAccessor = sourceSegment->getAccessor("SYN_flags");

#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) > 40100
#pragma omp parallel for
#endif
	for (long targetL = firstL; targetL <= lastL; targetL++) {
		logging.progress("Averaging line l = " + lexical_cast<string>(targetL), getId());

		for (size_t i = 0; i < variableNames.size(); i++) {
			const string& variableName = variableNames[i];
			const Accessor& sourceAccessor = sourceSegment->getAccessor(variableName);
			Accessor& targetAccessor = targetSegment->getAccessor(variableName);

			for (long k = targetGrid.getFirstK(); k < targetGrid.getFirstK() + targetGrid.getSizeK(); k++) {
				for (long targetM = targetGrid.getFirstM(); targetM < targetGrid.getFirstM() + targetGrid.getSizeM(); targetM++) {
					const size_t targetIndex = targetGrid.getIndex(k, targetL, targetM);

					double sum = 0.0;
					long pixelCount = 0;
					long validCount = 0;

					for (long sourceL = targetL * averagingFactor + sourceGrid.getMinL(); sourceL < (targetL + 1) * averagingFactor + sourceGrid.getMinL(); sourceL++) {
						for (long sourceM = targetM * averagingFactor; sourceM < (targetM + 1) * averagingFactor; sourceM++) {
							if (!sourceGrid.isValidPosition(k, sourceL, sourceM)) {
								continue;
							}
							const size_t sourceIndex = sourceGrid.getIndex(k, sourceL, sourceM);

							pixelCount++;
							const uint16_t synFlags = sourceFlagsAccessor.getUShort(sourceIndex);
							const bool land = (synFlags & Constants::SY2_LAND_FLAG) == Constants::SY2_LAND_FLAG;
							const bool cloud = (synFlags & Constants::SY2_CLOUD_FLAG) == Constants::SY2_CLOUD_FLAG;
							if (land && !cloud && !sourceAccessor.isFillValue(sourceIndex)) {
								sum += sourceAccessor.getDouble(sourceIndex);
								validCount++;
							}
						}
					}
					if (validCount == pixelCount) {
						targetAccessor.setDouble(targetIndex, sum / validCount);
					} else {
						targetAccessor.setFillValue(targetIndex);
					}
				}
			}
		}
		averageFlags(targetL);
		averageLatLon(targetL);
	}
}

void Ave::averageFlags(long targetL) {
	const Grid& sourceGrid = sourceSegment->getGrid();
	const Grid& targetGrid = targetSegment->getGrid();

	const Accessor& sourceAccessor = sourceSegment->getAccessor("SYN_flags");
	Accessor& targetAccessor = targetSegment->getAccessor("SYN_flags");

	for (long k = targetGrid.getFirstK(); k < targetGrid.getFirstK() + targetGrid.getSizeK(); k++) {
		for (long targetM = targetGrid.getFirstM(); targetM < targetGrid.getFirstM() + targetGrid.getSizeM(); targetM++) {
			uint16_t averagedFlags = 0;
			bool partlyCloudy = false;
			bool partlyWater = false;
			bool border = false;

			for (long sourceL = targetL * averagingFactor + sourceGrid.getMinL(); sourceL < (targetL + 1) * averagingFactor + sourceGrid.getMinL(); sourceL++) {
				for (long sourceM = targetM * averagingFactor; sourceM < (targetM + 1) * averagingFactor; sourceM++) {
					if (sourceGrid.isValidPosition(k, sourceL, sourceM)) {
						const long sourceIndex = sourceSegment->getGrid().getIndex(k, sourceL, sourceM);
						const uint16_t synFlags = sourceAccessor.getUShort(sourceIndex);
						const bool land = (synFlags & Constants::SY2_LAND_FLAG) == Constants::SY2_LAND_FLAG;
						const bool cloud = (synFlags & Constants::SY2_CLOUD_FLAG) == Constants::SY2_CLOUD_FLAG;
						const bool cloudFilled = (synFlags & Constants::SY2_CLOUD_FILLED_FLAG) == Constants::SY2_CLOUD_FILLED_FLAG;
						partlyCloudy = partlyCloudy || cloud || cloudFilled;
						partlyWater = partlyWater || !land;
					} else {
						border = true;
					}
				}

			}

			averagedFlags |= partlyCloudy ? Constants::SY2_PARTLY_CLOUDY_FLAG : 0;
			averagedFlags |= partlyWater ? Constants::SY2_PARTLY_WATER_FLAG : 0;
			averagedFlags |= border ? Constants::SY2_BORDER_FLAG : 0;
			const size_t targetIndex = targetGrid.getIndex(k, targetL, targetM);
			targetAccessor.setUShort(targetIndex, averagedFlags);
		}
	}
}

void Ave::averageLatLon(long targetL) {
	const Grid& sourceGrid = sourceSegment->getGrid();
	const Grid& targetGrid = targetSegment->getGrid();

	const Accessor& sourceLatAccessor = sourceSegment->getAccessor("latitude");
	const Accessor& sourceLonAccessor = sourceSegment->getAccessor("longitude");

	Accessor& targetLatAccessor = targetSegment->getAccessor("latitude");
	Accessor& targetLonAccessor = targetSegment->getAccessor("longitude");

	for (long k = targetGrid.getFirstK(); k < targetGrid.getFirstK() + targetGrid.getSizeK(); k++) {
		for (long targetM = targetGrid.getFirstM(); targetM < targetGrid.getFirstM() + targetGrid.getSizeM(); targetM++) {
			double x = 0.0;
			double y = 0.0;
			double z = 0.0;
			long n = 0;

			for (long sourceL = targetL * averagingFactor + sourceGrid.getMinL(); sourceL < (targetL + 1) * averagingFactor + sourceGrid.getMinL(); sourceL++) {
				for (long sourceM = targetM * averagingFactor; sourceM < (targetM + 1) * averagingFactor; sourceM++) {
					if (!sourceGrid.isValidPosition(k, sourceL, sourceM)) {
						continue;
					}
					const long sourceIndex = sourceGrid.getIndex(k, sourceL, sourceM);
					if (!sourceLatAccessor.isFillValue(sourceIndex) && !sourceLonAccessor.isFillValue(sourceIndex)) {
						const double lat = sourceLatAccessor.getDouble(sourceIndex);
						const double lon = sourceLonAccessor.getDouble(sourceIndex);

						accumulateLatLon(lat, lon, x, y, z);
						n++;
					}
				}
			}
			const size_t targetIndex = targetGrid.getIndex(k, targetL, targetM);
			if (n > 0) {
				double lat;
				double lon;
				toLatLon(x, y, z, lat, lon);
				targetLatAccessor.setDouble(targetIndex, lat);
				targetLonAccessor.setDouble(targetIndex, lon);
			} else {
				targetLatAccessor.setFillValue(targetIndex);
				targetLonAccessor.setFillValue(targetIndex);
			}
		}
	}
}

void Ave::addVariables(Context& context) {
	const vector<string> allVariableNames = sourceSegment->getVariableNames();

	for (size_t i = 0; i < allVariableNames.size(); i++) {
		const string& variableName = allVariableNames[i];
		if (isRadianceName(variableName) || variableName.compare("SYN_flags") == 0) {
			const Accessor& sourceAccessor = sourceSegment->getAccessor(variableName);
			context.getLogging().info("Adding variable '" + variableName + "' to segment '" + targetSegment->toString() + "'.", getId());
			targetSegment->addVariable(variableName, sourceAccessor.getType(), sourceAccessor.getScaleFactor(), sourceAccessor.getAddOffset());

			variableNames.push_back(variableName);
		}
	}
	context.getLogging().info("Adding variable 'longitude' to segment '" + targetSegment->toString() + "'.", getId());
	targetSegment->addVariable("longitude", Constants::TYPE_DOUBLE);
	context.getLogging().info("Adding variable 'latitude' to segment '" + targetSegment->toString() + "'.", getId());
	targetSegment->addVariable("latitude", Constants::TYPE_DOUBLE);
}

bool Ave::isRadianceName(const string& variableName) {
	static const regex regularExpression("(solar_irradiance|L)_[0-9][0-9]?(_er)?");
	return regex_match(variableName, regularExpression);
}
