/*
 * Module responsible for classifying pixels.
 *
 * Preconditions:
 * 	- the segment 'SYN_COLLOCATED' must have been added to the context
 *  - that segment needs to comprise the variables 'OLC_flags', 'SLN_confidence', and 'SLO_confidence'
 *
 * Postconditions:
 *  - the variable 'SYN_flags' has been added to the segment 'SYN_COLLOCATED'
 *  - that variable comprises flags for land and cloud for each pixel
 *
 * Pcl.cpp
 *
 *  Created on: Sep 14, 2011
 *      Author: thomasstorm
 */

#include "Pcl.h"

Pcl::Pcl() :
		BasicModule("PCL") {
}

Pcl::~Pcl() {
}


void Pcl::start(Context& context) {
    setUpSegment(context);
    setUpSourceAccessors(context);
}


void Pcl::setUpSegment(Context& context) {
    collocatedSegment = &(context.getSegment(Constants::SEGMENT_SYN_COLLOCATED));
    const ProductDescriptor& productDescriptor = context.getDictionary()->getProductDescriptor(Constants::PRODUCT_SY2);
    const string variableName = "SYN_flags";
    const VariableDescriptor& synFlags = productDescriptor.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED).getVariableDescriptor(variableName);
    collocatedSegment->addVariable(variableName, synFlags.getType(), synFlags.getScaleFactor(), synFlags.getAddOffset());
	context.getLogging()->info("adding variable '" + variableName + "' to segment '" + collocatedSegment->getId() + "'.", getId());
}

void Pcl::setUpSourceAccessors(Context & context) {
    olcFlagsAccessor = &getSourceAccessor(context, "OLC_flags", Constants::SEGMENT_OLC);
    slnFlagsAccessor = &getSourceAccessor(context, "SLN_confidence", Constants::SEGMENT_SLN);
    sloFlagsAccessor = &getSourceAccessor(context, "SLO_confidence", Constants::SEGMENT_SLO);
}

void Pcl::stop(Context& context) {

}

const Accessor& Pcl::getSourceAccessor(Context& context, string variableName, string sourceSegmentId) {
	const ProductDescriptor& sy1Descriptor = context.getDictionary()->getProductDescriptor(Constants::PRODUCT_SY1);
	const VariableDescriptor& variableDescriptor = sy1Descriptor.getSegmentDescriptor(sourceSegmentId).getVariableDescriptor(variableName);
	const string& flagVariableName = variableDescriptor.getName();
	const string& segmentName = Constants::SEGMENT_SYN_COLLOCATED;
	return context.getSegment(segmentName).getAccessor(flagVariableName);
}

void Pcl::process(Context& context) {
	context.getLogging()->info("Setting flags for segment '" + collocatedSegment->toString() + "'.", getId());
	boost::shared_ptr<Dictionary> dictionary = context.getDictionary();
	const ProductDescriptor& l2Descriptor = dictionary->getProductDescriptor(Constants::PRODUCT_SY2);
	const VariableDescriptor& flagsDescriptor = l2Descriptor.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED).getVariableDescriptor("SYN_flags");
	const string targetVariableName = flagsDescriptor.getName();
	collocatedSegment->addVariable(targetVariableName, flagsDescriptor.getType(), flagsDescriptor.getScaleFactor(), flagsDescriptor.getAddOffset());
	Accessor& targetAccessor = collocatedSegment->getAccessor(targetVariableName);

	const valarray<int64_t> olcFlags = olcFlagsAccessor->getLongData();
	const valarray<int16_t> slnFlags = slnFlagsAccessor->getShortData();
	const valarray<int16_t> sloFlags = sloFlagsAccessor->getShortData();

	const Grid& collocatedGrid = collocatedSegment->getGrid();
	for (size_t k = collocatedGrid.getFirstK(); k < collocatedGrid.getFirstK() + collocatedGrid.getSizeK(); k++) {
		for (size_t l = collocatedGrid.getFirstL(); l < collocatedGrid.getFirstL() + collocatedGrid.getSizeL(); l++) {
			for (size_t m = collocatedGrid.getFirstM(); m < collocatedGrid.getFirstM() + collocatedGrid.getSizeM(); m++) {
				const size_t index = getIndex(k, l, m);
				const uint16_t value = getValue(index, olcFlags[index], slnFlags[index], sloFlags[index]);
				targetAccessor.setUShort(index, value);
			}
		}
	}
}

size_t Pcl::getIndex(size_t k, size_t l, size_t m) const {
	return collocatedSegment->getGrid().getIndex(k, l, m);
}

uint16_t Pcl::getValue(size_t index, int64_t olcFlags, int16_t slnFlags, int16_t sloFlags) const {
	const int64_t olciLandFlag = 2147483648;
	const int16_t slstrLandFlag = 8;
	const int16_t slstrCloudFlag = 16384;

    bool isLandPixel = (olcFlags & olciLandFlag) == olciLandFlag;
    isLandPixel &= (slnFlags & slstrLandFlag) == slstrLandFlag;
    isLandPixel &= (sloFlags & slstrLandFlag) == slstrLandFlag;
    bool isCloudPixel = (slnFlags & slstrCloudFlag) == slstrCloudFlag;
    isCloudPixel &= (sloFlags & slstrCloudFlag) == slstrCloudFlag;

	uint16_t result = 0;
    if(isLandPixel) {
		result |= 32;
	}

    if(isCloudPixel) {
		result |= 1;
	}
	return result;
}
