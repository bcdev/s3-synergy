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
	const ProductDescriptor& productDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2);
	const VariableDescriptor& synFlags = productDescriptor.getSegmentDescriptor(Constants::SEGMENT_SYN_COLLOCATED).getVariableDescriptor("SYN_flags");
	collocatedSegment->addVariable(synFlags);
	context.getLogging().info("Adding variable 'SYN_flags' to segment '" + collocatedSegment->getId() + "'.", getId());
}

void Pcl::setUpSourceAccessors(Context & context) {
	olcFlagsAccessor = &collocatedSegment->getAccessor("OLC_flags");
	slnFlagsAccessor = &collocatedSegment->getAccessor("SLN_confidence");
	sloFlagsAccessor = &collocatedSegment->getAccessor("SLO_confidence");

	for (size_t b = 19; b <= 30; b++) {
		radianceAccessors.push_back(&collocatedSegment->getAccessor("L_" + lexical_cast<string>(b)));
	}
}

void Pcl::process(Context& context) {
	context.getLogging().info("Setting flags for segment '" + collocatedSegment->toString() + "'.", getId());
	Accessor& targetAccessor = collocatedSegment->getAccessor("SYN_flags");

	const valarray<uint32_t>& olcFlags = olcFlagsAccessor->getUIntData();
	const valarray<uint8_t>& slnFlags = slnFlagsAccessor->getUByteData();
	const valarray<uint8_t>& sloFlags = sloFlagsAccessor->getUByteData();

	const long firstL = context.getFirstComputableL(*collocatedSegment, *this);
	context.getLogging().debug("Segment [" + collocatedSegment->toString() + "]: firstComputableL = " + lexical_cast<string>(firstL), getId());
	long lastL = context.getLastComputableL(*collocatedSegment, *this);
	context.getLogging().debug("Segment [" + collocatedSegment->toString() + "]: lastComputableL = " + lexical_cast<string>(lastL), getId());

	const Grid& collocatedGrid = collocatedSegment->getGrid();

#pragma omp parallel for
	for (long l = firstL; l <= lastL; l++) {
		context.getLogging().progress("Setting flags for line l = " + lexical_cast<string>(l), getId());
		for (long k = collocatedGrid.getFirstK(); k < collocatedGrid.getFirstK() + collocatedGrid.getSizeK(); k++) {
			for (long m = collocatedGrid.getFirstM(); m < collocatedGrid.getFirstM() + collocatedGrid.getSizeM(); m++) {
				const size_t index = collocatedGrid.getIndex(k, l, m);
				uint16_t value = computeFlagValue(olcFlags[index], slnFlags[index], sloFlags[index]);

				bool noSLN = true;
				bool noSLO = true;
				for (size_t b = 0; b < 6; b++) {
					noSLN &= radianceAccessors[b]->isFillValue(index);
				}
				for (size_t b = 6; b < 12; b++) {
					noSLO &= radianceAccessors[b]->isFillValue(index);
				}
				if (noSLN) {
					value |= Constants::SY2_NO_SLN_FLAG;
				}
				if (noSLO) {
					value |= Constants::SY2_NO_SLO_FLAG;
				}

				targetAccessor.setUShort(index, value);
			}
		}
	}
	context.setLastComputedL(*collocatedSegment, *this, lastL);
}

uint16_t Pcl::computeFlagValue(uint32_t olcFlags, uint8_t slnFlags, uint8_t sloFlags) {
	const bool land = (olcFlags & Constants::SY1_OLCI_LAND_FLAG) == Constants::SY1_OLCI_LAND_FLAG;
	const bool cloud = (slnFlags & Constants::SY1_SLSTR_CLOUD_FLAG) == Constants::SY1_SLSTR_CLOUD_FLAG || (sloFlags & Constants::SY1_SLSTR_CLOUD_FLAG) == Constants::SY1_SLSTR_CLOUD_FLAG;

	uint16_t synFlags = 0;
	if (land) {
		synFlags |= Constants::SY2_LAND_FLAG;
	}
	if (cloud) {
		synFlags |= Constants::SY2_CLOUD_FLAG;
	}
	return synFlags;
}
