/*
 * Ave.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: thomasstorm
 */

#include "Ave.h"

const size_t Ave::AVERAGING_FACTOR = 8;

Ave::Ave() : BasicModule("AVE") {
}

Ave::~Ave() {
}

void Ave::start(Context& context) {
    const Grid& collocatedGrid = context.getSegment(Constants::SEGMENT_SYN_COLLOCATED).getGrid();
    const size_t sizeL = collocatedGrid.getSizeL() / AVERAGING_FACTOR;
    const size_t sizeM = collocatedGrid.getSizeM() / AVERAGING_FACTOR;
    const size_t sizeK = collocatedGrid.getSizeK();
    context.addSegment(Constants::SEGMENT_SYN_AVERAGED, sizeL, sizeM, sizeK, 0, sizeL - 1);

    const ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor(Constants::PRODUCT_SY2);
    const SegmentDescriptor& collocatedDescriptor = productDescriptor.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
    variables = collocatedDescriptor.getVariableDescriptors();
}

void Ave::stop(Context& context) {
	// TODO: cleanup
}

void Ave::process(Context& context) {

    foreach(VariableDescriptor* vd, variables) {

    }

}
