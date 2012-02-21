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

#include "VgtWriter.h"

VgtWriter::VgtWriter(const string& productId) :
		AbstractWriter("VGT_WRITER", productId) {
}

VgtWriter::~VgtWriter() {
}

vector<SegmentDescriptor*> VgtWriter::getSegmentDescriptors(const ProductDescriptor& productDescriptor) const {
	vector<SegmentDescriptor*> allSegmentDescriptors = productDescriptor.getSegmentDescriptors();
	vector<SegmentDescriptor*> segmentDescriptors;
	foreach (SegmentDescriptor* segmentDescriptor, allSegmentDescriptors)
			{
				const string segmentName = segmentDescriptor->getName();

				if (!isCoordinateSegment(segmentName) && !isTiePointCoordinateSegment(segmentName) && !segmentName.compare("CRS") == 0) {
					segmentDescriptors.push_back(segmentDescriptor);
				}
			}

	return segmentDescriptors;
}

void VgtWriter::defineCoordinateVariables(const Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const {
	vector<VariableDescriptor*> coordinateVariableDescriptors;
	if (isTiePointSegment(segmentName)) {
		coordinateVariableDescriptors = getTiePointCoordinateVariableDescriptors(productDescriptor);
	} else {
		coordinateVariableDescriptors = getCoordinateVariableDescriptors(productDescriptor);
	}
	foreach (const VariableDescriptor* coordinateVariableDescriptor, coordinateVariableDescriptors)
			{
				const vector<Dimension*> dimensions = coordinateVariableDescriptor->getDimensions();
				valarray<int> dimIds(dimensions.size());
				for (size_t i = 0; i < dimensions.size(); i++) {
					int dimId = NetCDF::findDimension(fileId, dimensions[i]->getName());
					if (dimId == -1) {
						dimId = NetCDF::defineDimension(fileId, dimensions[i]->getName(), dimensions[i]->getSize());
					}
					dimIds[i] = dimId;
				}
				NetCDF::defineVariable(fileId, coordinateVariableDescriptor->getName(), coordinateVariableDescriptor->getType(), dimIds);
			}

	const VariableDescriptor& crsDescriptor = productDescriptor.getSegmentDescriptor("CRS").getVariableDescriptor("crs");
	const int crsId = NetCDF::defineVariable(fileId, crsDescriptor.getName(), crsDescriptor.getType());

	foreach (const Attribute* attribute, crsDescriptor.getAttributes())
			{
				context.getLogging().debug("Defining attribute '" + attribute->getName() + "'", getId()); // do not remove this line
				NetCDF::putAttribute(fileId, crsId, *attribute);
			}
}

void VgtWriter::writeCoordinateVariables(Context& context, int fileId, const ProductDescriptor& productDescriptor, const string& segmentName) const {
	vector<VariableDescriptor*> coordinateVariableDescriptors;
	if (isTiePointSegment(segmentName)) {
		coordinateVariableDescriptors = getTiePointCoordinateVariableDescriptors(productDescriptor);
	} else {
		coordinateVariableDescriptors = getCoordinateVariableDescriptors(productDescriptor);
	}
	foreach (const VariableDescriptor* coordinateVariableDescriptor, coordinateVariableDescriptors)
			{
				const Segment& coordinateSegment = context.getSegment(coordinateVariableDescriptor->getSegmentName());
				const string& coordinateName = coordinateVariableDescriptor->getName();

				if (context.getLastComputedL(coordinateSegment, *this) < coordinateSegment.getGrid().getMaxL()) {
					const int varId = NetCDF::getVariableId(fileId, coordinateName);
					const Accessor& accessor = coordinateSegment.getAccessor(coordinateName);
					NetCDF::putData(fileId, varId, accessor.getUntypedData());
					context.setLastComputedL(coordinateSegment, *this, coordinateSegment.getGrid().getMaxL());
				}
			}
}

vector<VariableDescriptor*> VgtWriter::getTiePointCoordinateVariableDescriptors(const ProductDescriptor& productDescriptor) const {
	const vector<SegmentDescriptor*> segmentDescriptors = productDescriptor.getSegmentDescriptors();

	vector<VariableDescriptor*> tiePointCoordinateVariableDescriptors;
	foreach (SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				if (isTiePointCoordinateSegment(segmentDescriptor->getName())) {
					vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
					foreach(VariableDescriptor* v, variableDescriptors)
							{
								tiePointCoordinateVariableDescriptors.push_back(v);
							}
				}
			}

	return tiePointCoordinateVariableDescriptors;
}

vector<VariableDescriptor*> VgtWriter::getCoordinateVariableDescriptors(const ProductDescriptor& productDescriptor) const {
	const vector<SegmentDescriptor*> segmentDescriptors = productDescriptor.getSegmentDescriptors();

	vector<VariableDescriptor*> coordinateVariableDescriptors;
	foreach (SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				if (isCoordinateSegment(segmentDescriptor->getName())) {
					vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
					foreach(VariableDescriptor* v, variableDescriptors)
							{
								coordinateVariableDescriptors.push_back(v);
							}
				}
			}

	return coordinateVariableDescriptors;
}

bool VgtWriter::isTiePointCoordinateSegment(const string& segmentName) {
	return segmentName.compare(Constants::SEGMENT_VGT_LAT_TP) == 0 || segmentName.compare(Constants::SEGMENT_VGT_LON_TP) == 0 || segmentName.compare(Constants::SEGMENT_VGT_LAT_TP_BNDS) == 0
			|| segmentName.compare(Constants::SEGMENT_VGT_LON_TP_BNDS) == 0;
}

bool VgtWriter::isTiePointSegment(const string& segmentName) {
	return segmentName.compare(Constants::SEGMENT_VGT_TP) == 0;
}

bool VgtWriter::isCoordinateSegment(const string& segmentName) {
	return segmentName.compare(Constants::SEGMENT_VGT_LAT) == 0 || segmentName.compare(Constants::SEGMENT_VGT_LON) == 0 || segmentName.compare(Constants::SEGMENT_VGT_LAT_BNDS) == 0
			|| segmentName.compare(Constants::SEGMENT_VGT_LON_BNDS) == 0;
}
