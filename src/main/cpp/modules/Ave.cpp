/*
 * Ave.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: thomasstorm
 */

#include "Ave.h"

const int8_t Ave::AVERAGING_FACTOR = 8;

Ave::Ave() : BasicModule("AVE") {
}

Ave::~Ave() {
}

void Ave::start(Context& context) {
    collocatedSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
    const Grid& collocatedGrid = collocatedSegment->getGrid();
    const size_t sizeL = collocatedGrid.getSizeL() / AVERAGING_FACTOR;
    const size_t sizeM = collocatedGrid.getSizeM() / AVERAGING_FACTOR;
    const size_t sizeK = collocatedGrid.getSizeK();
    averagedSegment = &context.addSegment(Constants::SEGMENT_SYN_AVERAGED, sizeL, sizeM, sizeK, 0, sizeL - 1);
    averagedGrid = &averagedSegment->getGrid();

    const ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor(Constants::PRODUCT_SY2);
    const SegmentDescriptor& collocatedDescriptor = productDescriptor.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
    variables = collocatedDescriptor.getVariableDescriptors();
    synFlags = &collocatedSegment->getAccessor("SYN_flags");
}

void Ave::stop(Context& context) {
	// TODO: cleanup
}

void Ave::process(Context& context) {

    foreach(VariableDescriptor* vd, variables) {

        string varName = vd->getName();
        if(varName.compare("SYN_flags") == 0) {
            continue;
        }

        context.getLogging()->progress("Averaging variable '" + vd->getName() + "'.", getId());

        double a = 0.0;
        long I = 0;
        long K = 0;

        for (long k = averagedGrid->getFirstK(); k < averagedGrid->getFirstK() + averagedGrid->getSizeK(); k++) {
            for (long l_strich = averagedGrid->getFirstL(); l_strich <= averagedGrid->getLastL(); l_strich++) {
                for (long m_strich = averagedGrid->getFirstM(); m_strich < averagedGrid->getFirstM() + averagedGrid->getSizeM(); m_strich++) {

                    for(long l = l_strich * AVERAGING_FACTOR; l < (l_strich + 1) * AVERAGING_FACTOR; l++) {
                        for(long m = m_strich * AVERAGING_FACTOR; m < (m_strich + 1) * AVERAGING_FACTOR; m++) {
                            if(!isValidPosition(averagedGrid, k, l_strich, m_strich)) {
                                continue;
                            }
                            const long collocatedIndex = collocatedSegment->getGrid().getIndex(k, l, m);

                            I++;
                            uint16_t f = synFlags->getUShort(collocatedIndex);
                            const bool isLand = f & 32 == 32;
                            const bool isCloud = f & 1 == 1;
                            if(isLand && !isCloud && !isFillValue(varName, collocatedIndex)) {
                                a += getValue(varName, collocatedIndex);
                                K++;
                            }
                        }
                    }

                    const long averagedIndex = averagedGrid->getIndex(k, l_strich, m_strich);
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

bool Ave::isValidPosition(const Grid* grid, long k, long l, long m) const {
    return (grid->getFirstK() <= k &&
            grid->getMaxK() >= k &&
            grid->getFirstL() <= l &&
            grid->getMaxL() >= l &&
            grid->getFirstM() <= m &&
            grid->getMaxM() >= m);
}

bool Ave::isFillValue(const string& variableName, const long index) const {
    const Accessor& accessor = collocatedSegment->getAccessor(variableName);
    return accessor.isFillValue(index);
}

double Ave::getValue(const string& variableName, const long index) const {
    const Accessor& accessor = collocatedSegment->getAccessor(variableName);
    return accessor.getDouble(index);
}
