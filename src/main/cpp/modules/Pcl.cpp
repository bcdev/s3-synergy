/*
 * Module responsible for classifying pixels.
 *
 * Preconditions:
 * 	- the segment 'SYN_COLLOCATED' must have been added to the context
 *  - that segment needs to comprise the variables 'OLC_flags', 'SLN_confidence', and 'SLO_confidence'
 *
 * Postconditions:
 *  - the variables 'SYN_flags', 'OLC_flags', 'SLN_flags', and 'SLO_flags' have been added to the segment 'SYN_COLLOCATED'
 *  - that variables comprise flags for land and cloud for each pixel
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
	const ProductDescriptor& productDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2);
	const SegmentDescriptor& collocatedDescriptor = productDescriptor.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED);
	const VariableDescriptor& synFlags = collocatedDescriptor.getVariableDescriptor("SYN_flags");
	const VariableDescriptor& olcFlags = collocatedDescriptor.getVariableDescriptor("OLC_flags");
	const VariableDescriptor& slnFlags = collocatedDescriptor.getVariableDescriptor("SLN_flags");
	const VariableDescriptor& sloFlags = collocatedDescriptor.getVariableDescriptor("SLO_flags");
	context.getLogging().info("Adding variable 'SYN_flags' to segment '" + collocatedSegment->getId() + "'.", getId());
	context.getLogging().info("Adding variable 'OLC_flags' to segment '" + collocatedSegment->getId() + "'.", getId());
	context.getLogging().info("Adding variable 'SLN_flags' to segment '" + collocatedSegment->getId() + "'.", getId());
	context.getLogging().info("Adding variable 'SLO_flags' to segment '" + collocatedSegment->getId() + "'.", getId());
	collocatedSegment->addVariable(synFlags);
	collocatedSegment->addVariable(olcFlags);
	collocatedSegment->addVariable(slnFlags);
	collocatedSegment->addVariable(sloFlags);
}

void Pcl::setUpSourceAccessors(Context& context) {
	olcFlagsAccessor = &collocatedSegment->getAccessor("OLC_confidence");
	slnFlagsAccessor = &collocatedSegment->getAccessor("SLN_confidence");
	sloFlagsAccessor = &collocatedSegment->getAccessor("SLO_confidence");

	for (size_t b = 1; b <= 30; b++) {
		radianceAccessors.push_back(&collocatedSegment->getAccessor("L_" + lexical_cast<string>(b)));
	}

	for(size_t b = 1; b <= 6; b++) {
	    slnExceptionAccessors.push_back(&collocatedSegment->getAccessor("L_" + lexical_cast<string>(b + 18) + "_exception"));
	    sloExceptionAccessors.push_back(&collocatedSegment->getAccessor("L_" + lexical_cast<string>(b + 24) + "_exception"));
	}
}

void Pcl::process(Context& context) {
	context.getLogging().info("Setting flags for segment '" + collocatedSegment->toString() + "'.", getId());
	Accessor& synTargetFlags = collocatedSegment->getAccessor("SYN_flags");
	Accessor& olcTargetFlags = collocatedSegment->getAccessor("OLC_flags");
	Accessor& slnTargetFlags = collocatedSegment->getAccessor("SLN_flags");
	Accessor& sloTargetFlags = collocatedSegment->getAccessor("SLO_flags");

	const valarray<uint32_t>& olcFlags = olcFlagsAccessor->getUIntData();
	const valarray<uint8_t>& slnFlags = slnFlagsAccessor->getUByteData();
	const valarray<uint8_t>& sloFlags = sloFlagsAccessor->getUByteData();

	const long firstL = context.getFirstComputableL(*collocatedSegment, *this);
	context.getLogging().debug("Segment [" + collocatedSegment->toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
	long lastL = context.getLastComputableL(*collocatedSegment, *this);
	context.getLogging().debug("Segment [" + collocatedSegment->toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

	const Grid& collocatedGrid = collocatedSegment->getGrid();

#if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100) > 40100
#pragma omp parallel for
#endif
	for (long l = firstL; l <= lastL; l++) {
		context.getLogging().progress("Setting flags for line l = " + lexical_cast<string>(l), getId());
		for (long k = collocatedGrid.getMinK(); k <= collocatedGrid.getMaxK(); k++) {
			for (long m = collocatedGrid.getMinM(); m <= collocatedGrid.getMaxM(); m++) {
				const size_t index = collocatedGrid.getIndex(k, l, m);

				const uint32_t currentOlcFlags = olcFlags[index];
				const uint8_t currentSlnFlags = slnFlags[index];
				const uint8_t currentSloFlags = sloFlags[index];
				uint16_t synValue = computeSynFlagValue(currentOlcFlags, currentSlnFlags, currentSloFlags, index);
				synTargetFlags.setUShort(index, synValue);

				const uint16_t olcValue = computeOlcFlagValue(currentOlcFlags);
				olcTargetFlags.setUShort(index, olcValue);

				const uint32_t slnValue = computeSlnFlagValue(currentSlnFlags, index);
				slnTargetFlags.setUShort(index, slnValue);

				const uint32_t sloValue = computeSloFlagValue(currentSloFlags, index);
				sloTargetFlags.setUShort(index, sloValue);
			}
		}
	}
	context.setLastComputedL(*collocatedSegment, *this, lastL);
}

uint16_t Pcl::computeSynFlagValue(uint32_t olcFlags, uint8_t slnFlags, uint8_t sloFlags, size_t index) {
	const bool land = isSet(olcFlags, Constants::SY1_OLCI_LAND_FLAG);
	const bool cloud = isSet(slnFlags, Constants::SY1_SLSTR_CLOUD_FLAG) || isSet(sloFlags, Constants::SY1_SLSTR_CLOUD_FLAG);

	uint16_t synFlags = 0;
	if (land) {
		synFlags |= Constants::SY2_LAND_FLAG;
	}
	if (cloud) {
		synFlags |= Constants::SY2_CLOUD_FLAG;
	}

    bool noOLC = true;
    bool noSLN = true;
    bool noSLO = true;
    for (size_t b = 0; b < 18; b++) {
        noOLC &= radianceAccessors[b]->isFillValue(index);
    }
    for (size_t b = 18; b < 24; b++) {
        noSLN &= radianceAccessors[b]->isFillValue(index);
    }
    for (size_t b = 24; b < 30; b++) {
        noSLO &= radianceAccessors[b]->isFillValue(index);
    }
    if (noOLC) {
        synFlags |= Constants::SY2_NO_OLC_FLAG;
    }
    if (noSLN) {
        synFlags |= Constants::SY2_NO_SLN_FLAG;
    }
    if (noSLO) {
        synFlags |= Constants::SY2_NO_SLO_FLAG;
    }

	return synFlags;
}

uint16_t Pcl::computeOlcFlagValue(uint32_t currentOlcFlags) {
    uint16_t olcValue = 0;
    bool isSummarySatured = false;
    for(uint32_t flag = 2097152U; flag >= 2U; flag = flag >> 1) {
        isSummarySatured |= isSet(currentOlcFlags, flag);
    }
    olcValue |= isSummarySatured ? 1 : 0;
    olcValue |= isSet(currentOlcFlags, 4194304U) ? 2 : 0;
    olcValue |= isSet(currentOlcFlags, 8388608U) ? 4 : 0;
    olcValue |= isSet(currentOlcFlags, 33554432U) ? 8 : 0;
    olcValue |= isSet(currentOlcFlags, 67108864U) ? 16 : 0;
    olcValue |= isSet(currentOlcFlags, 134217728U) ? 256 : 0;
    olcValue |= isSet(currentOlcFlags, 268435456U) ? 512 : 0;
    olcValue |= isSet(currentOlcFlags, 536870912U) ? 1024 : 0;
    olcValue |= isSet(currentOlcFlags, 1073741824U) ? 2048 : 0;
    olcValue |= isSet(currentOlcFlags, 2147483648U) ? 4096 : 0;
    return olcValue;
}

uint16_t Pcl::computeSlnFlagValue(uint32_t currentSlnFlags, size_t index) {
    uint16_t slnValue = 0;
    bool isSummaryISPAbsent = false;
    bool isSummaryPixelAbsent = false;
    bool isSummaryNotDecompressed = false;
    bool isSummaryNoSignal = false;
    bool isSummarySaturation = false;
    bool isSummaryInvalidRadiance = false;
    bool isSummaryNoParameters = false;
    bool isSummaryUnfilledPixel = false;
    foreach(const Accessor* slnExceptionAccessor, slnExceptionAccessors) {
        uint8_t currentExceptionFlag = slnExceptionAccessor->getUByte(index);
        isSummaryISPAbsent |= isSet(currentExceptionFlag, (uint8_t)1);
        isSummaryPixelAbsent |= isSet(currentExceptionFlag, (uint8_t)2);
        isSummaryNotDecompressed |= isSet(currentExceptionFlag, (uint8_t)4);
        isSummaryNoSignal |= isSet(currentExceptionFlag, (uint8_t)8);
        isSummarySaturation |= isSet(currentExceptionFlag, (uint8_t)16);
        isSummaryInvalidRadiance |= isSet(currentExceptionFlag, (uint8_t)32);
        isSummaryNoParameters |= isSet(currentExceptionFlag, (uint8_t)64);
        isSummaryUnfilledPixel |= isSet(currentExceptionFlag, (uint8_t)128);
    }

    slnValue |= isSummaryISPAbsent ? 1 : 0;
    slnValue |= isSummaryPixelAbsent ? 2 : 0;
    slnValue |= isSummaryNotDecompressed ? 4 : 0;
    slnValue |= isSummaryNoSignal ? 8 : 0;
    slnValue |= isSummarySaturation ? 16 : 0;
    slnValue |= isSummaryInvalidRadiance ? 32 : 0;
    slnValue |= isSummaryNoParameters ? 64 : 0;
    slnValue |= isSummaryUnfilledPixel ? 128 : 0;

    // todo - is aligned with L1c Products Definition, but doesn't work with test product

//    slnValue |= isSet(currentSlnFlags, 1U) ? 1 << 16 : 0; // cosmetic
//    slnValue |= isSet(currentSlnFlags, 4U) ? 1 << 18 : 0; // day
//    slnValue |= isSet(currentSlnFlags, 8U) ? 1 << 19 : 0; // twilight
//    slnValue |= isSet(currentSlnFlags, 16U) ? 1 << 20 : 0; // sun_glint
//    slnValue |= isSet(currentSlnFlags, 32U) ? 1 << 21 : 0; // snow
//    slnValue |= isSet(currentSlnFlags, 64U) ? 1 << 22 : 0; // summary_cloud
//    slnValue |= isSet(currentSlnFlags, 128U) ? 1 << 23 : 0; // summary_pointing

    return slnValue;
}

uint16_t Pcl::computeSloFlagValue(uint32_t currentSloFlags, size_t index) {
    uint16_t sloValue = 0;
    bool isSummaryISPAbsent = false;
    bool isSummaryPixelAbsent = false;
    bool isSummaryNotDecompressed = false;
    bool isSummaryNoSignal = false;
    bool isSummarySaturation = false;
    bool isSummaryInvalidRadiance = false;
    bool isSummaryNoParameters = false;
    bool isSummaryUnfilledPixel = false;
    foreach(const Accessor* sloExceptionAccessor, sloExceptionAccessors) {
        uint8_t currentExceptionFlag = sloExceptionAccessor->getUByte(index);
        isSummaryISPAbsent |= isSet(currentExceptionFlag, (uint8_t)1);
        isSummaryPixelAbsent |= isSet(currentExceptionFlag, (uint8_t)2);
        isSummaryNotDecompressed |= isSet(currentExceptionFlag, (uint8_t)4);
        isSummaryNoSignal |= isSet(currentExceptionFlag, (uint8_t)8);
        isSummarySaturation |= isSet(currentExceptionFlag, (uint8_t)16);
        isSummaryInvalidRadiance |= isSet(currentExceptionFlag, (uint8_t)32);
        isSummaryNoParameters |= isSet(currentExceptionFlag, (uint8_t)64);
        isSummaryUnfilledPixel |= isSet(currentExceptionFlag, (uint8_t)128);
    }

    sloValue |= isSummaryISPAbsent ? 1 : 0;
    sloValue |= isSummaryPixelAbsent ? 2 : 0;
    sloValue |= isSummaryNotDecompressed ? 4 : 0;
    sloValue |= isSummaryNoSignal ? 8 : 0;
    sloValue |= isSummarySaturation ? 16 : 0;
    sloValue |= isSummaryInvalidRadiance ? 32 : 0;
    sloValue |= isSummaryNoParameters ? 64 : 0;
    sloValue |= isSummaryUnfilledPixel ? 128 : 0;

    // todo - is aligned with L1c Products Definition, but doesn't work with test product

//    sloValue |= isSet(currentSloFlags, 1U) ? 1 << 16 : 0; // cosmetic
//    sloValue |= isSet(currentSloFlags, 4U) ? 1 << 18 : 0; // day
//    sloValue |= isSet(currentSloFlags, 8U) ? 1 << 19 : 0; // twilight
//    sloValue |= isSet(currentSloFlags, 16U) ? 1 << 20 : 0; // sun_glint
//    sloValue |= isSet(currentSloFlags, 32U) ? 1 << 21 : 0; // snow
//    sloValue |= isSet(currentSloFlags, 64U) ? 1 << 22 : 0; // summary_cloud
//    sloValue |= isSet(currentSloFlags, 128U) ? 1 << 23 : 0; // summary_pointing

    return sloValue;
}
