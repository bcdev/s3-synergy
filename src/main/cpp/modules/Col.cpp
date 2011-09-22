/*
 * Col.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: ralf
 */

#include "Col.h"

#include <cmath>

Col::Col() :
		BasicModule("COL") {
}

Col::~Col() {
}

void Col::start(Context& context) {
	const Segment& s = context.getSegment(Constants::SEGMENT_OLC);
	const Grid& g = s.getGrid();

	Segment& t = context.addSegment(Constants::SEGMENT_SYN_COLLOCATED, g.getSizeL(), g.getSizeM(), g.getSizeK(), g.getMinL(), g.getMaxL());

	const ProductDescriptor& sourceProductDescriptor = context.getDictionary()->getProductDescriptor("SY1");

	// TODO - read mapping from auxiliary data
	const size_t channelMapping[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16, 17, 18, 19, 21 };
	for (size_t i = 0; i < 18; i++) {
		const string sourceName = "L_" + lexical_cast<string>(channelMapping[i]);
		const string targetName = "L_" + lexical_cast<string>(i + 1);
		if (channelMapping[i] == 17) {
			addVariableAlias(context, t, targetName, s, sourceName);
		} else {
			addVariable(context, t, targetName, s, sourceName, sourceProductDescriptor);
			sourceChannelMap[targetName] = channelMapping[i];
		}
	}
	for (size_t i = 0; i < 18; i++) {
		const string sourceName = "L_" + lexical_cast<string>(channelMapping[i]) + "_er";
		const string targetName = "L_" + lexical_cast<string>(i + 1) + "_er";
		if (channelMapping[i] == 17) {
			addVariableAlias(context, t, targetName, s, sourceName);
		} else {
			addVariable(context, t, targetName, s, sourceName, sourceProductDescriptor);
			sourceChannelMap[targetName] = channelMapping[i];
		}
	}

	addVariableAlias(context, t, "longitude", s, "longitude");
	addVariableAlias(context, t, "latitude", s, "latitude");
	addVariableAlias(context, t, "altitude", s, "altitude");
}

/*
 void Col::addSlstrVariables(Context& context) {
 const ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor("SY1");
 for (size_t i = 1; i < 7; i++) {
 const string sourceName = "L_" + lexical_cast<string>(i);
 const string targetName = "L_" + lexical_cast<string>(i + 18);
 addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_SLN);
 }
 for (size_t i = 1; i < 7; i++) {
 const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
 const string targetName = "L_" + lexical_cast<string>(i + 18) + "_exception";
 addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_SLN);
 }
 for (size_t i = 1; i < 7; i++) {
 const string sourceName = "L_" + lexical_cast<string>(i);
 const string targetName = "L_" + lexical_cast<string>(i + 24);
 addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_SLO);
 }
 for (size_t i = 1; i < 7; i++) {
 const string sourceName = "L_" + lexical_cast<string>(i) + "_exception";
 const string targetName = "L_" + lexical_cast<string>(i + 24) + "_exception";
 addVariable(context, productDescriptor, sourceName, targetName, Constants::SEGMENT_SLO);
 }

 addVariable(context, productDescriptor, SLN_CONFIDENCE_FLAG_VARIABLE, SLN_CONFIDENCE_FLAG_VARIABLE, Constants::SEGMENT_SLN);
 addVariable(context, productDescriptor, SLO_CONFIDENCE_FLAG_VARIABLE, SLO_CONFIDENCE_FLAG_VARIABLE, Constants::SEGMENT_SLO);
 }
 */

void Col::stop(Context& context) {
	// TODO: cleanup
}

void Col::process(Context& context) {
	using std::floor;
	using std::min;

	const Segment& s = context.getSegment(Constants::SEGMENT_OLC);
	const Segment& t = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);

	const Grid& sourceGrid = s.getGrid();
	const Grid& targetGrid = t.getGrid();

	const size_t firstL = context.getFirstComputableL(t, *this);
	const size_t lastL = context.getLastComputableL(t);

	// TODO: make segment's firstL etc. and context's lastComputedL etc. be of type int32_t or let the lastXXX
	// variables point to one line behind
	size_t firstRequiredL = lastL;
	size_t lastComputedL = lastL;

	foreach(const string& targetName, targetNames)
			{
				const string& sourceName = sourceNameMap[targetName];
				const Accessor& sourceAccessor = s.getAccessor(sourceName);

				Accessor& targetAccessor = t.getAccessor(targetName);
				const string& deltaRowName = "delta_y_" + lexical_cast<string>(sourceChannelMap[targetName]);
				const string& deltaColName = "delta_x_" + lexical_cast<string>(sourceChannelMap[targetName]);

				const Accessor& deltaRowAccessor = s.getAccessor(deltaRowName);
				const Accessor& deltaColAccessor = s.getAccessor(deltaColName);

				for (size_t l = firstL; l <= lastL; l++) {
					for (size_t k = targetGrid.getFirstK(); k < targetGrid.getFirstK() + targetGrid.getSizeK(); k++) {
						for (size_t m = targetGrid.getFirstM(); m < targetGrid.getFirstM() + targetGrid.getSizeM(); m++) {
							const size_t targetIndex = targetGrid.getIndex(k, l, m);

							if (deltaRowAccessor.isFillValue(targetIndex)) {
								continue;
							}
							if (deltaColAccessor.isFillValue(targetIndex)) {
								continue;
							}

							// TODO: loop over all bands here?
							const size_t sourceL = l + floor(deltaRowAccessor.getDouble(targetIndex));
							const size_t sourceM = m + floor(deltaColAccessor.getDouble(targetIndex));

							if (sourceL > context.getLastComputableL(s)) {
								firstRequiredL = min(sourceL, firstRequiredL);
								lastComputedL = min(l - 1, lastComputedL);
								goto nextVariable;
							}
							if (sourceL < sourceGrid.getMinL() || sourceL > sourceGrid.getMaxL()) {
								targetAccessor.setFillValue(targetIndex);
								continue;
							}
							if (sourceM < sourceGrid.getMinM() || sourceL > sourceGrid.getMaxM()) {
								targetAccessor.setFillValue(targetIndex);
								continue;
							}

							const size_t sourceIndex = sourceGrid.getIndex(k, sourceL, sourceM);

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
				nextVariable: ;
			}
	// TODO: context.setFirstRequiredL(t, *this, firstRequiredL);
	context.setLastComputedL(t, *this, lastComputedL);
}

void Col::addVariable(Context& context, Segment& t, const string& targetName, const Segment& s, const string& sourceName, const ProductDescriptor& p) {
	const VariableDescriptor& v = p.getSegmentDescriptor(s.getId()).getVariableDescriptor(sourceName);
	context.getLogging()->progress("Adding variable '" + targetName + "' to segment '" + t.getId() + "'", getId());
	t.addVariable(v, targetName);
	sourceNameMap[targetName] = sourceName;
	targetNames.push_back(targetName);
}

void Col::addVariableAlias(Context& context, Segment& t, const string& targetName, const Segment& s, const string& sourceName) const {
	context.getLogging()->progress("Adding alias '" + targetName + "' to segment '" + t.getId() + "'", getId());
	t.addVariableAlias(targetName, s, sourceName);
}
