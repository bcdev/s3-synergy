/*
 * Ave.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: thomasstorm
 */

#include <cmath>

#include "Ave.h"
#include "../util/IOUtils.h"

using std::min;

// todo - read from auxdata
const int8_t Ave::AVERAGING_FACTOR = 8;

Ave::Ave() : BasicModule("AVE") {
}

Ave::~Ave() {
}

void Ave::start(Context& context) {
    collocatedSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    const Grid& collocatedGrid = collocatedSegment->getGrid();
    minCollocatedL = collocatedGrid.getMinL();
    const size_t sizeL = collocatedGrid.getSizeL() / AVERAGING_FACTOR;
    const size_t sizeM = ceil(collocatedGrid.getSizeM() / AVERAGING_FACTOR);
    const size_t sizeK = collocatedGrid.getSizeK();
    const size_t maxL = ceil((collocatedGrid.getMaxL() - minCollocatedL + 1) / AVERAGING_FACTOR);
    averagedSegment = &context.addSegment(Constants::SEGMENT_SYN_AVERAGED, sizeL, sizeM, sizeK, 0, maxL);
    averagedGrid = &averagedSegment->getGrid();

    setupVariables(context);
    addFlagsVariable(context);
    synFlags = &collocatedSegment->getAccessor("SYN_flags");
}

void Ave::stop(Context& context) {
}

void Ave::process(Context& context) {
    const long firstL = context.getFirstComputableL(*averagedSegment, *this);
    const long lastComputableL = context.getLastComputableL(*collocatedSegment, *this);
    long lastL = (context.getLastComputableL(*collocatedSegment, *this) - collocatedSegment->getGrid().getMinL() + 1) / AVERAGING_FACTOR;
    if (lastComputableL < collocatedSegment->getGrid().getMaxL()) {
        lastL--;
    }

    context.getLogging().debug("Segment [" + averagedSegment->toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
    context.getLogging().debug("Segment [" + averagedSegment->toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

    averageVariables(context, firstL, lastL);
    averageFlags(context, firstL, lastL);
    context.setLastComputedL(*averagedSegment, *this, lastL);
    context.setFirstRequiredL(*collocatedSegment, *this, (lastL + 1)* AVERAGING_FACTOR);
}

void Ave::averageVariables(Context& context, long firstL, long lastL) {
    foreach(string& varName, variables) {
        context.getLogging().progress("Averaging variable '" + varName + "'...", getId());
        for (long l_prime = firstL; l_prime <= lastL; l_prime++) {
            if(l_prime % 100 == 0) {
                context.getLogging().progress("   ...averaging line " + lexical_cast<string>(l_prime), getId());
            }
            for (long k = averagedGrid->getFirstK(); k < averagedGrid->getFirstK() + averagedGrid->getSizeK(); k++) {
                for (long m_prime = averagedGrid->getFirstM(); m_prime < averagedGrid->getFirstM() + averagedGrid->getSizeM(); m_prime++) {

                    double a = 0.0;
                    long I = 0;
                    long K = 0;

                    for(long l = l_prime * AVERAGING_FACTOR + minCollocatedL; l < (l_prime + 1) * AVERAGING_FACTOR + minCollocatedL; l++) {
                        for(long m = m_prime * AVERAGING_FACTOR; m < (m_prime + 1) * AVERAGING_FACTOR; m++) {
                            if(!collocatedSegment->getGrid().isValidPosition(k, l, m)) {
                                continue;
                            }
                            const long collocatedIndex = collocatedSegment->getGrid().getIndex(k, l, m);

                            I++;
                            const uint16_t flag = synFlags->getUShort(collocatedIndex);
                            const bool isLand = (flag & 32) == 32;
                            const bool isCloud = (flag & 1) == 1;
                            if(isLand && !isCloud && !isFillValue(varName, collocatedIndex)) {
                                a += getValue(varName, collocatedIndex);
                                K++;
                            }
                        }
                    }

                    const long averagedIndex = averagedGrid->getIndex(k, l_prime, m_prime);
                    if(K == I) {
                        averagedSegment->getAccessor(varName).setDouble(averagedIndex, a / K);
                    } else {
                        averagedSegment->getAccessor(varName).setFillValue(averagedIndex);
                    }
                }
            }
        }
    }
}

void Ave::averageFlags(Context& context, long firstL, long lastL) {
    context.getLogging().progress("Averaging variable 'SYN_flags'...", getId());
    for (long l_prime = firstL + collocatedSegment->getGrid().getMinL(); l_prime <= lastL; l_prime++) {
        if(l_prime % 100 == 0) {
            context.getLogging().progress("   ...averaging line " + lexical_cast<string>(l_prime), getId());
        }
        for (long k = averagedGrid->getFirstK(); k < averagedGrid->getFirstK() + averagedGrid->getSizeK(); k++) {
            for (long m_prime = averagedGrid->getFirstM(); m_prime < averagedGrid->getFirstM() + averagedGrid->getSizeM(); m_prime++) {

                uint16_t flags = getFlagFillValue(context);
                bool isPartlyCloudy = false;
                bool isPartlyWater = false;
                bool isBorder = false;

                for(long l = l_prime * AVERAGING_FACTOR + minCollocatedL; l < (l_prime + 1) * AVERAGING_FACTOR + minCollocatedL; l++) {
                    for(long m = m_prime * AVERAGING_FACTOR; m < (m_prime + 1) * AVERAGING_FACTOR; m++) {
                        if(collocatedSegment->getGrid().isValidPosition(k, l, m)) {
                            const long collocatedIndex = collocatedSegment->getGrid().getIndex(k, l, m);
                            flags = synFlags->getUShort(collocatedIndex);
                            const bool isLand = (flags & 32) == 32;
                            const bool isCloud = (flags & 1) == 1;
                            const bool isCloudFilled = (flags & 8) == 8;
                            isPartlyCloudy = isPartlyCloudy || isCloud || isCloudFilled;
                            isPartlyWater = isPartlyWater || !isLand;
                        } else {
                            isBorder = true;
                        }
                    }
                }
                flags |= isPartlyCloudy ? 256 : 0;
                flags |= isPartlyWater ? 512 : 0;
                flags |= isBorder ? 1024 : 0;
                const size_t averagedIndex = averagedGrid->getIndex(k, l_prime, m_prime);
                averagedSynFlags->setUShort(averagedIndex, flags);
            }
        }
    }
}

bool Ave::isFillValue(const string& variableName, const long index) const {
    const Accessor& accessor = collocatedSegment->getAccessor(variableName);
    return accessor.isFillValue(index);
}

double Ave::getValue(const string& variableName, const long index) const {
    const Accessor& accessor = collocatedSegment->getAccessor(variableName);
    return accessor.getDouble(index);
}

uint16_t Ave::getFlagFillValue(Context& context) {
    const ProductDescriptor& pd = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2);
    const SegmentDescriptor& sd = pd.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
    const VariableDescriptor& vd = sd.getVariableDescriptor("SYN_flags");
    return vd.getFillValue<uint16_t>();
}

void Ave::addFlagsVariable(Context& context) {
    const ProductDescriptor& pd = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2);
    const SegmentDescriptor& sd = pd.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
    const VariableDescriptor& vd = sd.getVariableDescriptor("SYN_flags");
    context.getLogging().progress("Adding variable '" + vd.toString() + "' to segment '" + averagedSegment->toString() + "'.", getId());
    averagedSegment->addVariable(vd);
    averagedSynFlags = &averagedSegment->getAccessor("SYN_flags");
}

void Ave::setupVariables(Context& context) {
    variables = collocatedSegment->getVariableNames();
    vector<string> result;

    foreach(string varName, variables) {
        if(matches(varName)) {
            result.push_back(varName);
        }
    }
    variables = result;
    foreach(string varName, variables) {
        context.getLogging().progress("Adding variable '" + varName + "' to segment '" + averagedSegment->toString() + "'.", getId());
        const Accessor& accessor = collocatedSegment->getAccessor(varName);
        averagedSegment->addVariable(varName, accessor.getType(), accessor.getScaleFactor(), accessor.getAddOffset());
    }
}

bool Ave::matches(const string& varName) {
    const regex e("L_[0-9][0-9]?(_er)?(_exception)?");
    return regex_match(varName, e);
}
