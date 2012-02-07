/*
 * Col.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#include "Col.h"
#include "../util/TimeConverter.h"

#include <cmath>

Col::Col() :
		BasicModule("COL") {
}

Col::~Col() {
}

const string Col::SLO_CONFIDENCE_FLAG_VARIABLE_NAME = "SLO_confidence";
const string Col::SLN_CONFIDENCE_FLAG_VARIABLE_NAME = "SLN_confidence";
// TODO - read mapping from auxiliary data
const size_t Col::OLC_TO_SYN_CHANNEL_MAPPING[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16, 17, 18, 19, 21 };

void Col::start(Context& context) {
	const Grid& sourceGrid = context.getSegment(Constants::SEGMENT_OLC).getGrid();

	context.addSwathSegment(Constants::SEGMENT_SYN_COLLOCATED, sourceGrid.getSizeL(), sourceGrid.getSizeM(), sourceGrid.getSizeK(), sourceGrid.getMinL(), sourceGrid.getMaxL());

	addOlciVariables(context);
	addSlstrVariables(context);
}

void Col::process(Context& context) {
	using std::floor;
	using std::min;

	const TimeConverter tc(context.getJobOrder().getIpfConfiguration().getSensingTimeStart());

	const Segment& olc = context.getSegment(Constants::SEGMENT_OLC);
	const Segment& sln = context.getSegment(Constants::SEGMENT_SLN);
	const Segment& slo = context.getSegment(Constants::SEGMENT_SLO);

	const Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const Grid& targetGrid = t.getGrid();

	const long firstL = context.getFirstComputableL(t, *this);
	context.getLogging().debug("Segment [" + t.toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
	long lastL = context.getLastComputableL(t, *this);
	context.getLogging().debug("Segment [" + t.toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

	vector<Accessor*> sourceAccessors;
	vector<Accessor*> targetAccessors;
	vector<Accessor*> xAccessors;
	vector<Accessor*> yAccessors;
	map<const Segment*, long> firstRequiredLMap;

	foreach(const string& targetName, targetNames) {
	    const Segment& s = *sourceSegmentMap[targetName];
	    sourceAccessors.push_back(&s.getAccessor(sourceNameMap[targetName]));
	    targetAccessors.push_back(&t.getAccessor(targetName));

	    xAccessors.push_back(&olc.getAccessor(collocationNameMapX[targetName]));
	    yAccessors.push_back(&olc.getAccessor(collocationNameMapY[targetName]));
	}

	for (long l = firstL; l <= lastL; l++) {
		context.getLogging().progress("Processing line l = " + lexical_cast<string>(l), getId());

		firstRequiredLMap[&olc] = olc.getGrid().getLastL() + 1;
		firstRequiredLMap[&sln] = sln.getGrid().getLastL() + 1;
		firstRequiredLMap[&slo] = slo.getGrid().getLastL() + 1;

		for (size_t i = 0; i < targetNames.size(); i++) {
			const string& targetName = targetNames[i];
			const Segment& sourceSegment = *sourceSegmentMap[targetName];
			const Grid& sourceGrid = sourceSegment.getGrid();

			const Accessor& sourceAccessor = *sourceAccessors[i];
			Accessor& targetAccessor = *targetAccessors[i];

			const Accessor& collocationXAccessor = *xAccessors[i];
			const Accessor& collocationYAccessor = *yAccessors[i];

			const long lastComputableL = context.getLastComputableL(sourceSegment, *this);
			long& firstRequiredL = firstRequiredLMap[&sourceSegment];

			for (long k = targetGrid.getMinK(); k <= targetGrid.getMaxK(); k++) {
				for (long m = targetGrid.getMinM(); m <= targetGrid.getMaxM(); m++) {
					const size_t targetIndex = targetGrid.getIndex(k, l, m);

					if (collocationXAccessor.isFillValue(targetIndex) || collocationYAccessor.isFillValue(targetIndex)) {
						targetAccessor.setFillValue(targetIndex);
						continue;
					}

					long sourceK;
					long sourceL;
					long sourceM;

					if (sourceSegment == olc) {
						sourceK = k;
						sourceL = l + (long) floor(collocationYAccessor.getDouble(targetIndex));
						sourceM = m + (long) floor(collocationXAccessor.getDouble(targetIndex));
					} else {
						sourceK = 0;
						sourceL = (long) floor(collocationYAccessor.getDouble(targetIndex));
						sourceM = (long) floor(collocationXAccessor.getDouble(targetIndex));
					}

					if (sourceL < sourceGrid.getMinL() || sourceL > sourceGrid.getMaxL()) {
						targetAccessor.setFillValue(targetIndex);
						continue;
					}
					if (sourceM < sourceGrid.getMinM() || sourceM > sourceGrid.getMaxM()) {
						targetAccessor.setFillValue(targetIndex);
						continue;
					}

					firstRequiredL = min(sourceL, firstRequiredL);
					if (sourceL > lastComputableL) {
						lastL = min(l - 1, lastL);
						continue;
					}

					const size_t sourceIndex = sourceGrid.getIndex(sourceK, sourceL, sourceM);

					switch (sourceAccessor.getType()) {
					case Constants::TYPE_BYTE: {
						targetAccessor.setByte(targetIndex, sourceAccessor.getByte(sourceIndex));
						break;
					}
					case Constants::TYPE_UBYTE: {
						targetAccessor.setUByte(targetIndex, sourceAccessor.getUByte(sourceIndex));
						break;
					}
					case Constants::TYPE_SHORT: {
						targetAccessor.setShort(targetIndex, sourceAccessor.getShort(sourceIndex));
						break;
					}
					case Constants::TYPE_USHORT: {
						targetAccessor.setUShort(targetIndex, sourceAccessor.getUShort(sourceIndex));
						break;
					}
					case Constants::TYPE_INT: {
						targetAccessor.setInt(targetIndex, sourceAccessor.getInt(sourceIndex));
						break;
					}
					case Constants::TYPE_UINT: {
						targetAccessor.setUInt(targetIndex, sourceAccessor.getUInt(sourceIndex));
						break;
					}
					case Constants::TYPE_LONG: {
						targetAccessor.setLong(targetIndex, sourceAccessor.getLong(sourceIndex));
						break;
					}
					case Constants::TYPE_ULONG: {
						targetAccessor.setULong(targetIndex, sourceAccessor.getULong(sourceIndex));
						break;
					}
					case Constants::TYPE_FLOAT: {
						targetAccessor.setFloat(targetIndex, sourceAccessor.getFloat(sourceIndex));
						break;
					}
					case Constants::TYPE_DOUBLE: {
						targetAccessor.setDouble(targetIndex, sourceAccessor.getDouble(sourceIndex));
						break;
					}
					default:
						BOOST_THROW_EXCEPTION(std::invalid_argument("Cannot collocate variable '" + targetName + "': unsupported data type."));
						break;
					}
				}
			}
		}

		const Segment& olcInfo = context.getSegment(Constants::SEGMENT_OLC_INFO);
		const Segment& slnInfo = context.getSegment(Constants::SEGMENT_SLN_INFO);
		const Segment& sloInfo = context.getSegment(Constants::SEGMENT_SLO_INFO);
		const Grid& olcInfoGrid = olcInfo.getGrid();
		const Grid& slnInfoGrid = slnInfo.getGrid();
		const Grid& sloInfoGrid = sloInfo.getGrid();

		for (size_t i = 0; i < 18; i++) {
			Accessor* targetAccessor = solarIrradianceAccessors[i];
			const Accessor* sourceAccessor = sourceAccessorMap[targetAccessor];
			const Accessor* xCollocationAccessor = xCollocationAccessorMap[targetAccessor];

			for (long k = targetGrid.getMinK(); k <= targetGrid.getMaxK(); k++) {
				for (long m = targetGrid.getMinM(); m <= targetGrid.getMaxM(); m++) {
					const size_t targetIndex = targetGrid.getIndex(k, l, m);

					double deltaX = 0.0;
					if (xCollocationAccessor != 0) {
						if (xCollocationAccessor->isFillValue(targetIndex)) {
							targetAccessor->setFillValue(targetIndex);
							continue;
						}
						deltaX = xCollocationAccessor->getDouble(targetIndex);
					}

					const long sourceM = m + (long) floor(deltaX);
					if (sourceM < olcInfoGrid.getMinM() || sourceM > olcInfoGrid.getMaxM()) {
						targetAccessor->setFillValue(targetIndex);
					} else {
						const size_t sourceIndex = olcInfoGrid.getIndex(k, OLC_TO_SYN_CHANNEL_MAPPING[i] - 1, sourceM);
						if (sourceAccessor->isFillValue(sourceIndex)) {
							targetAccessor->setFillValue(targetIndex);
						} else {
							targetAccessor->setDouble(targetIndex, sourceAccessor->getDouble(sourceIndex));
						}
					}
				}
			}
		}
		for (size_t i = 18; i < 30; i++) {
			Accessor* targetAccessor = solarIrradianceAccessors[i];
			const Accessor* sourceAccessor = sourceAccessorMap[targetAccessor];
			const Accessor* yCollocationAccessor = yCollocationAccessorMap[targetAccessor];

			for (long k = targetGrid.getMinK(); k <= targetGrid.getMaxK(); k++) {
				for (long m = targetGrid.getMinM(); m <= targetGrid.getMaxM(); m++) {
					const size_t targetIndex = targetGrid.getIndex(k, l, m);

					double y = 0.0;
					if (yCollocationAccessor != 0) {
						if (yCollocationAccessor->isFillValue(targetIndex)) {
							targetAccessor->setFillValue(targetIndex);
							continue;
						}
						y = yCollocationAccessor->getDouble(targetIndex);
					}

					const long sourceL = (long) floor(y);
					if (sourceL < 0) {
						targetAccessor->setFillValue(targetIndex);
					} else {
						const long detectorIndex = sourceL % 4;
						const size_t sourceIndex = i < 24 ? slnInfoGrid.getIndex(0, 0, detectorIndex) : sloInfoGrid.getIndex(0, 0, detectorIndex);
						if (sourceAccessor->isFillValue(sourceIndex)) {
							targetAccessor->setFillValue(targetIndex);
						} else {
							targetAccessor->setDouble(targetIndex, sourceAccessor->getDouble(sourceIndex));
						}
					}
				}
			}
		}
	}

	context.setFirstRequiredL(olc, *this, min(firstRequiredLMap[&olc], lastL));
	context.setFirstRequiredL(sln, *this, firstRequiredLMap[&sln]);
	context.setFirstRequiredL(slo, *this, firstRequiredLMap[&slo]);
	context.setFirstRequiredL(t, *this, firstRequiredLMap[&olc]);
	context.setLastComputedL(t, *this, lastL);
}

void Col::addOlciVariables(Context& context) {
	const Segment& sourceSegment = context.getSegment(Constants::SEGMENT_OLC);
	Segment& targetSegment = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
	const ProductDescriptor & sourceProductDescriptor = context.getDictionary().getProductDescriptor("SY1");
	for (size_t i = 0; i < 18; i++) {
		const string sourceName = "L_" + lexical_cast<string>(OLC_TO_SYN_CHANNEL_MAPPING[i]);
		const string targetName = "L_" + lexical_cast<string>(i + 1);
		if (OLC_TO_SYN_CHANNEL_MAPPING[i] == OLC_REFERENCE_CHANNEL) {
			addVariableAlias(context, targetSegment, targetName, sourceSegment, sourceName);
		} else {
			addVariable(context, targetSegment, targetName, sourceSegment, sourceName, sourceProductDescriptor);
			collocationNameMapX[targetName] = "delta_x_" + lexical_cast<string>(OLC_TO_SYN_CHANNEL_MAPPING[i]);
			collocationNameMapY[targetName] = "delta_y_" + lexical_cast<string>(OLC_TO_SYN_CHANNEL_MAPPING[i]);
		}
	}

	for (size_t i = 0; i < 18; i++) {
		const string sourceName = "L_" + lexical_cast<string>(OLC_TO_SYN_CHANNEL_MAPPING[i]) + "_er";
		const string targetName = "L_" + lexical_cast<string>(i + 1) + "_er";
		if (OLC_TO_SYN_CHANNEL_MAPPING[i] == OLC_REFERENCE_CHANNEL) {
			addVariableAlias(context, targetSegment, targetName, sourceSegment, sourceName);
		} else {
			addVariable(context, targetSegment, targetName, sourceSegment, sourceName, sourceProductDescriptor);
			collocationNameMapX[targetName] = "delta_x_" + lexical_cast<string>(OLC_TO_SYN_CHANNEL_MAPPING[i]);
			collocationNameMapY[targetName] = "delta_y_" + lexical_cast<string>(OLC_TO_SYN_CHANNEL_MAPPING[i]);
		}
	}

	addVariableAlias(context, targetSegment, "OLC_confidence", sourceSegment, "OLC_confidence");

//	addVariableAlias(context, targetSegment, "longitude", sourceSegment, "longitude");
//	addVariableAlias(context, targetSegment, "latitude", sourceSegment, "latitude");
	addVariableAlias(context, targetSegment, "altitude", sourceSegment, "altitude");

	const Segment& olcInfoSegment = context.getSegment(Constants::SEGMENT_OLC_INFO);
	for (size_t i = 0; i < 18; i++) {
		const string targetName = "solar_irradiance_" + lexical_cast<string>(i + 1);

		context.getLogging().info("Adding variable '" + targetName + "' to segment '" + targetSegment.getId() + "'", getId());
		Accessor* targetAccessor = &targetSegment.addVariable(targetName, Constants::TYPE_DOUBLE);

		solarIrradianceAccessors.push_back(targetAccessor);
		sourceAccessorMap[targetAccessor] = &olcInfoSegment.getAccessor("solar_irradiance");
		if (OLC_TO_SYN_CHANNEL_MAPPING[i] == OLC_REFERENCE_CHANNEL) {
			xCollocationAccessorMap[targetAccessor] = 0;
			yCollocationAccessorMap[targetAccessor] = 0;
		} else {
			xCollocationAccessorMap[targetAccessor] = &sourceSegment.getAccessor("delta_x_" + lexical_cast<string>(OLC_TO_SYN_CHANNEL_MAPPING[i]));
			yCollocationAccessorMap[targetAccessor] = &sourceSegment.getAccessor("delta_y_" + lexical_cast<string>(OLC_TO_SYN_CHANNEL_MAPPING[i]));
		}
	}
}

void Col::addSlstrVariables(Context& context) {
	const Segment& nadir = context.getSegment(Constants::SEGMENT_SLN);
	const Segment& oblique = context.getSegment(Constants::SEGMENT_SLO);
	Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);

	const ProductDescriptor& sourceProductDescriptor = context.getDictionary().getProductDescriptor("SY1");
	for (size_t i = 1; i < 7; i++) {
		const string sourceName = "L_" + lexical_cast<string>(i);
		const string targetName = "L_" + lexical_cast<string>(i + 18);
		addVariable(context, t, targetName, nadir, sourceName, sourceProductDescriptor);
		collocationNameMapX[targetName] = "x_corr_" + lexical_cast<string>(i);
		collocationNameMapY[targetName] = "y_corr_" + lexical_cast<string>(i);
	}
	for (size_t i = 1; i < 7; i++) {
		const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
		const string targetName = "L_" + lexical_cast<string>(i + 18) + "_exception";
		addVariable(context, t, targetName, nadir, sourceName, sourceProductDescriptor);
		collocationNameMapX[targetName] = "x_corr_" + lexical_cast<string>(i);
		collocationNameMapY[targetName] = "y_corr_" + lexical_cast<string>(i);
	}
	for (size_t i = 1; i < 7; i++) {
		const string sourceName = "L_" + lexical_cast<string>(i);
		const string targetName = "L_" + lexical_cast<string>(i + 24);
		addVariable(context, t, targetName, oblique, sourceName, sourceProductDescriptor);
		collocationNameMapX[targetName] = "x_corr_o";
		collocationNameMapY[targetName] = "y_corr_o";
	}
	for (size_t i = 1; i < 7; i++) {
		const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
		const string targetName = "L_" + lexical_cast<string>(i + 24) + "_exception";
		addVariable(context, t, targetName, oblique, sourceName, sourceProductDescriptor);
		collocationNameMapX[targetName] = "x_corr_o";
		collocationNameMapY[targetName] = "y_corr_o";
	}

	addVariable(context, t, SLN_CONFIDENCE_FLAG_VARIABLE_NAME, nadir, SLN_CONFIDENCE_FLAG_VARIABLE_NAME, sourceProductDescriptor);
	collocationNameMapX[SLN_CONFIDENCE_FLAG_VARIABLE_NAME] = "x_corr_" + lexical_cast<string>(3);
	collocationNameMapY[SLN_CONFIDENCE_FLAG_VARIABLE_NAME] = "y_corr_" + lexical_cast<string>(3);
	addVariable(context, t, SLO_CONFIDENCE_FLAG_VARIABLE_NAME, oblique, SLO_CONFIDENCE_FLAG_VARIABLE_NAME, sourceProductDescriptor);
	collocationNameMapX[SLO_CONFIDENCE_FLAG_VARIABLE_NAME] = "x_corr_o";
	collocationNameMapY[SLO_CONFIDENCE_FLAG_VARIABLE_NAME] = "y_corr_o";

	const Segment& olcSegment = context.getSegment(Constants::SEGMENT_OLC);
	const Segment& slnInfoSegment = context.getSegment(Constants::SEGMENT_SLN_INFO);
	const Segment& sloInfoSegment = context.getSegment(Constants::SEGMENT_SLN_INFO);
	for (size_t i = 1; i < 7; i++) {
		const string targetName = "solar_irradiance_" + lexical_cast<string>(i + 18);

		context.getLogging().info("Adding variable '" + targetName + "' to segment '" + t.getId() + "'", getId());
		Accessor* targetAccessor = &t.addVariable(targetName, Constants::TYPE_DOUBLE);

		solarIrradianceAccessors.push_back(targetAccessor);
		sourceAccessorMap[targetAccessor] = &slnInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(i));
		xCollocationAccessorMap[targetAccessor] = &olcSegment.getAccessor("x_corr_" + lexical_cast<string>(i));
		yCollocationAccessorMap[targetAccessor] = &olcSegment.getAccessor("y_corr_" + lexical_cast<string>(i));
	}
	for (size_t i = 1; i < 7; i++) {
		const string targetName = "solar_irradiance_" + lexical_cast<string>(i + 24);

		context.getLogging().info("Adding variable '" + targetName + "' to segment '" + t.getId() + "'", getId());
		Accessor* targetAccessor = &t.addVariable(targetName, Constants::TYPE_DOUBLE);

		solarIrradianceAccessors.push_back(targetAccessor);
		sourceAccessorMap[targetAccessor] = &sloInfoSegment.getAccessor("solar_irradiance_" + lexical_cast<string>(i));
		xCollocationAccessorMap[targetAccessor] = &olcSegment.getAccessor("x_corr_o");
		yCollocationAccessorMap[targetAccessor] = &olcSegment.getAccessor("y_corr_o");
	}
}

void Col::addVariable(Context& context, Segment& t, const string& targetName, const Segment& s, const string& sourceName, const ProductDescriptor& p) {
	const VariableDescriptor& v = p.getSegmentDescriptor(s.getId()).getVariableDescriptor(sourceName);
	context.getLogging().info("Adding variable '" + v.toString() + "' to segment '" + t.getId() + "'", getId());
	t.addVariable(v, targetName);
	sourceNameMap[targetName] = sourceName;
	sourceSegmentMap[targetName] = &s;
	targetNames.push_back(targetName);
}

void Col::addVariableAlias(Context& context, Segment& t, const string& targetName, const Segment& s, const string& sourceName) const {
	context.getLogging().info("Adding alias '" + targetName + "' to segment '" + t.getId() + "'", getId());
	t.addVariableAlias(targetName, s, sourceName);
}
