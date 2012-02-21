/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#include <algorithm>
#include <stdio.h>
#include <string.h>
#include "../../../main/cpp/util/Utils.h"
#include "VgtSegmentProvider.h"

using std::min;

VgtSegmentProvider::VgtSegmentProvider() :
		AbstractModule("VGT_SEGMENT_PROVIDER") {
}

VgtSegmentProvider::~VgtSegmentProvider() {
}

void VgtSegmentProvider::start(Context& context) {
	addCoordinateSegments(context);
	addNonCoordinateSegments(context);
}

void VgtSegmentProvider::stop(Context& context) {
	reverse_foreach (const string id, context.getSegmentIds())
			{
				context.removeSegment(id);
			}
}

void VgtSegmentProvider::process(Context& context) {
	setLastComputedLines(context, Constants::PRODUCT_VGP);
}

void VgtSegmentProvider::addNonCoordinateSegments(Context& context) {
	vector<SegmentDescriptor*> segmentDescriptors = getNonCoordinateSegmentDescriptors(context.getDictionary());
	foreach(SegmentDescriptor* segDesc, segmentDescriptors)
			{
				vector<VariableDescriptor*> variableDescriptors = segDesc->getVariableDescriptors();
				foreach(VariableDescriptor* varDesc, variableDescriptors)
						{
							const string& segmentName = segDesc->getName();
							if (segmentName.compare("CRS") == 0) {
								continue;
							}
							if (!context.hasSegment(segmentName)) {
								valarray<size_t> dimensionSizes = Utils::getDimensionSizes(varDesc);
								context.addMapSegment(segmentName, dimensionSizes[1], dimensionSizes[2]);
							}
							Segment& segment = context.getSegment(segmentName);
							if (!segment.hasVariable(varDesc->getName())) {
								segment.addVariable(*varDesc);
							}
						}
			}
}

void VgtSegmentProvider::addCoordinateSegments(Context& context) {
	vector<SegmentDescriptor*> segmentDescriptors = getCommonSegmentDescriptors(context.getDictionary());
	foreach(SegmentDescriptor* segDesc, segmentDescriptors)
			{
				vector<VariableDescriptor*> variableDescriptors = segDesc->getVariableDescriptors();
				foreach(VariableDescriptor* varDesc, variableDescriptors)
						{
							const string& segmentName = segDesc->getName();
							if (!context.hasSegment(segmentName)) {
								valarray<size_t> dimensionSizes = Utils::getDimensionSizes(varDesc);
								context.addSingleLineSegment(segmentName, dimensionSizes[2]);
							}
							Segment& segment = context.getSegment(segmentName);
							if (!segment.hasVariable(varDesc->getName())) {
								segment.addVariable(*varDesc);
							}
						}
			}
}

void VgtSegmentProvider::setLastComputedLines(Context& context, const string& identifier) {
	vector<SegmentDescriptor*> segmentDescriptors = context.getDictionary().getProductDescriptor(identifier).getSegmentDescriptors();
	foreach(SegmentDescriptor* segDesc, segmentDescriptors)
			{
				if (segDesc->getName().compare("CRS") == 0) {
					continue;
				}
				Segment& segment = context.getSegment(segDesc->getName());
				context.setLastComputedL(segment, *this, segment.getGrid().getMaxInMemoryL());
			}
}

const vector<SegmentDescriptor*> VgtSegmentProvider::getCommonSegmentDescriptors(const Dictionary& dict) const {
	const ProductDescriptor& vgtPProductDescriptor = dict.getProductDescriptor(Constants::PRODUCT_VGP);
	vector<SegmentDescriptor*> allVgtPSegmentDescriptors = vgtPProductDescriptor.getSegmentDescriptors();
	vector<SegmentDescriptor*> commonSegmentDescriptors;
	foreach(SegmentDescriptor* segmentDescriptor, allVgtPSegmentDescriptors)
			{
				if (isCommonDescriptor(*segmentDescriptor)) {
					commonSegmentDescriptors.push_back(segmentDescriptor);
				}
			}
	return commonSegmentDescriptors;
}

const vector<SegmentDescriptor*> VgtSegmentProvider::getNonCoordinateSegmentDescriptors(const Dictionary& dict) const {
	const ProductDescriptor& vgtPProductDescriptor = dict.getProductDescriptor(Constants::PRODUCT_VGP);
	vector<SegmentDescriptor*> allVgtPSegmentDescriptors = vgtPProductDescriptor.getSegmentDescriptors();
	vector<SegmentDescriptor*> nonCommonSegmentDescriptors;
	foreach(SegmentDescriptor* segmentDescriptor, allVgtPSegmentDescriptors)
			{
				if (!isCommonDescriptor(*segmentDescriptor)) {
					nonCommonSegmentDescriptors.push_back(segmentDescriptor);
				}
			}
	return nonCommonSegmentDescriptors;
}

bool VgtSegmentProvider::isCommonDescriptor(const SegmentDescriptor& segmentDescriptor) const {
	const string& segmentName = segmentDescriptor.getName();
	return segmentName.compare(Constants::SEGMENT_VGT_LAT) == 0 || segmentName.compare(Constants::SEGMENT_VGT_LAT_TP) == 0 || segmentName.compare(Constants::SEGMENT_VGT_LON) == 0
			|| segmentName.compare(Constants::SEGMENT_VGT_LON_TP) == 0 || segmentName.compare(Constants::SEGMENT_VGT_LAT_BNDS) == 0 || segmentName.compare(Constants::SEGMENT_VGT_LAT_TP_BNDS) == 0
			|| segmentName.compare(Constants::SEGMENT_VGT_LON_BNDS) == 0 || segmentName.compare(Constants::SEGMENT_VGT_LON_TP_BNDS) == 0;
}
