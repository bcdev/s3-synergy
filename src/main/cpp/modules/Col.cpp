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

const string Col::SLO_CONFIDENCE_FLAG_VARIABLE_NAME = "SLO_confidence";
const string Col::SLN_CONFIDENCE_FLAG_VARIABLE_NAME = "SLN_confidence";

void Col::start(Context& context) {
    const Grid& sourceGrid = context.getSegment(Constants::SEGMENT_OLC).getGrid();

    context.addSegment(Constants::SEGMENT_SYN_COLLOCATED, sourceGrid.getSizeL(), sourceGrid.getSizeM(), sourceGrid.getSizeK(), sourceGrid.getMinL(), sourceGrid.getMaxL());

    addOlciVariables(context);
    addSlstrVariables(context);
}

void Col::process(Context& context) {
    using std::floor;
    using std::min;

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

    foreach(const string& targetName, targetNames)
            {
                const Segment& s = *sourceSegmentMap[targetName];
                sourceAccessors.push_back(&s.getAccessor(sourceNameMap[targetName]));
                targetAccessors.push_back(&t.getAccessor(targetName));
                xAccessors.push_back(&olc.getAccessor(collocationNameMapX[targetName]));
                yAccessors.push_back(&olc.getAccessor(collocationNameMapY[targetName]));
            }

    for (long l = firstL; l <= lastL; l++) {
        context.getLogging().progress("Collocating line l = " + lexical_cast<string>(l), getId());

        firstRequiredLMap[&olc] = olc.getGrid().getLastL() + 1;
        firstRequiredLMap[&sln] = sln.getGrid().getLastL() + 1;
        firstRequiredLMap[&slo] = slo.getGrid().getLastL() + 1;

        for (size_t i = 0; i < targetNames.size(); i++) {
            const string& targetName = targetNames[i];
            const Segment& s = *sourceSegmentMap[targetName];
            const Grid& sourceGrid = s.getGrid();

            const Accessor& sourceAccessor = *sourceAccessors[i];
            Accessor& targetAccessor = *targetAccessors[i];

            const Accessor& collocationXAccessor = *xAccessors[i];
            const Accessor& collocationYAccessor = *yAccessors[i];

            for (long k = targetGrid.getFirstK(); k < targetGrid.getFirstK() + targetGrid.getSizeK(); k++) {
                for (long m = targetGrid.getFirstM(); m < targetGrid.getFirstM() + targetGrid.getSizeM(); m++) {
                    const size_t targetIndex = targetGrid.getIndex(k, l, m);

                    if (collocationXAccessor.isFillValue(targetIndex) || collocationYAccessor.isFillValue(targetIndex)) {
                        continue;
                    }

                    long sourceK;
                    long sourceL;
                    long sourceM;

                    if (s.getId().compare(Constants::SEGMENT_OLC) == 0) {
                        sourceK = k;
                        sourceL = l + floor(collocationYAccessor.getDouble(targetIndex));
                        sourceM = m + floor(collocationXAccessor.getDouble(targetIndex));
                    } else {
                        sourceK = 0;
                        sourceL = floor(collocationYAccessor.getDouble(targetIndex));
                        sourceM = floor(collocationXAccessor.getDouble(targetIndex));
                    }

                    if (sourceL < sourceGrid.getMinL() || sourceL > sourceGrid.getMaxL()) {
                        targetAccessor.setFillValue(targetIndex);
                        continue;
                    }
                    if (sourceM < sourceGrid.getMinM() || sourceM > sourceGrid.getMaxM()) {
                        targetAccessor.setFillValue(targetIndex);
                        continue;
                    }

                    firstRequiredLMap[&s] = min(sourceL, firstRequiredLMap[&s]);
                    const long lastComputableL = context.getLastComputableL(s, *this);
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
    // TODO - read mapping from auxiliary data
    const size_t channelMapping[18] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16, 17, 18, 19, 21 };
    for (size_t i = 0; i < 18; i++) {
        const string sourceName = "L_" + lexical_cast<string>(channelMapping[i]);
        const string targetName = "L_" + lexical_cast<string>(i + 1);
        if (channelMapping[i] == 17) {
            addVariableAlias(context, targetSegment, targetName, sourceSegment, sourceName);
        } else {
            addVariable(context, targetSegment, targetName, sourceSegment, sourceName, sourceProductDescriptor);
            collocationNameMapX[targetName] = "delta_x_" + lexical_cast<string>(channelMapping[i]);
            collocationNameMapY[targetName] = "delta_y_" + lexical_cast<string>(channelMapping[i]);
        }
    }

    for (size_t i = 0; i < 18; i++) {
        const string sourceName = "L_" + lexical_cast<string>(channelMapping[i]) + "_er";
        const string targetName = "L_" + lexical_cast<string>(i + 1) + "_er";
        if (channelMapping[i] == 17) {
            addVariableAlias(context, targetSegment, targetName, sourceSegment, sourceName);
        } else {
            addVariable(context, targetSegment, targetName, sourceSegment, sourceName, sourceProductDescriptor);
            collocationNameMapX[targetName] = "delta_x_" + lexical_cast<string>(channelMapping[i]);
            collocationNameMapY[targetName] = "delta_y_" + lexical_cast<string>(channelMapping[i]);
        }
    }

    addVariable(context, targetSegment, "OLC_flags", sourceSegment, "OLC_flags", sourceProductDescriptor);
    collocationNameMapX["OLC_flags"] = "delta_x_" + lexical_cast<string>(1);
    collocationNameMapY["OLC_flags"] = "delta_y_" + lexical_cast<string>(1);

    addVariableAlias(context, targetSegment, "longitude", sourceSegment, "longitude");
    addVariableAlias(context, targetSegment, "latitude", sourceSegment, "latitude");
    addVariableAlias(context, targetSegment, "altitude", sourceSegment, "altitude");
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
    collocationNameMapX[SLN_CONFIDENCE_FLAG_VARIABLE_NAME] = "x_corr_" + lexical_cast<string>(1);
    collocationNameMapY[SLN_CONFIDENCE_FLAG_VARIABLE_NAME] = "y_corr_" + lexical_cast<string>(1);
    addVariable(context, t, SLO_CONFIDENCE_FLAG_VARIABLE_NAME, oblique, SLO_CONFIDENCE_FLAG_VARIABLE_NAME, sourceProductDescriptor);
    collocationNameMapX[SLO_CONFIDENCE_FLAG_VARIABLE_NAME] = "x_corr_o";
    collocationNameMapY[SLO_CONFIDENCE_FLAG_VARIABLE_NAME] = "y_corr_o";
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
