/*
 * File:   VgtWriter.cpp
 * Author: thomass
 *
 * Created on January 11, 2012, 16:30
 */

#include "VgtWriter.h"

VgtWriter::VgtWriter(const string& productId, const string& safeManifestName) :
		AbstractWriter("VGT_WRITER"), productId(productId), safeManifestName(safeManifestName) {
}

VgtWriter::~VgtWriter() {
}

const string& VgtWriter::getProductId() const {
	return productId;
}

const string& VgtWriter::getSafeManifestName() const {
	return safeManifestName;
}

const vector<SegmentDescriptor*> VgtWriter::getSegmentDescriptors(const Dictionary& dict) const {
	const ProductDescriptor& productDescriptor = dict.getProductDescriptor(getProductId());
	vector<SegmentDescriptor*> allSegmentDescriptors = productDescriptor.getSegmentDescriptors();
	vector<SegmentDescriptor*> nonCoordinateSegmentDescriptors;
	foreach (SegmentDescriptor* segmentDescriptor, allSegmentDescriptors)
			{
				if (!isCoordinateSegmentDescriptor(*segmentDescriptor)) {
					nonCoordinateSegmentDescriptors.push_back(segmentDescriptor);
				}
			}
	return nonCoordinateSegmentDescriptors;
}

void VgtWriter::writeCoordinateVariables(Context& context) {
	const vector<int> fileIds = getFileIds();
	foreach (const int fileId, fileIds)
			{
				foreach (SegmentDescriptor* sd, getCoordinateSegmentDescriptors(context.getDictionary()))
						{
							Segment& segment = context.getSegment(sd->getName());
							if (context.getLastComputedL(segment, *this) == segment.getGrid().getMaxL()) {
								continue;
							}
							const vector<string> varNames = segment.getVariableNames();
							foreach (string varName, varNames)
									{
										const int varId = NetCDF::getVariableId(fileId, varName);
										if (varId == -1) {
											continue;
										}
										const Accessor& accessor = segment.getAccessor(varName);
										NetCDF::putData(fileId, varId, accessor.getUntypedData());
									}
							context.setLastComputedL(segment, *this, segment.getGrid().getMaxL());
						}
			}
}

void VgtWriter::defineCoordinateDimensions(int fileId, const string& segmentName, const Dictionary& dict, map<const VariableDescriptor*, valarray<int> >& coordinateDimIds) {
	vector<VariableDescriptor*> variableDescriptors;
	if (isTiePointSegment(segmentName)) {
		variableDescriptors = getSubsampledCoordinateVariableDescriptors(dict);
	} else {
		variableDescriptors = getCoordinateVariableDescriptors(dict);
	}
	foreach (const VariableDescriptor* variableDescriptor, variableDescriptors)
			{
				const vector<Dimension*> dimensions = variableDescriptor->getDimensions();
				valarray<int> dimIds(dimensions.size());
				for (size_t j = 0; j < dimensions.size(); j++) {
					int dimId = NetCDF::findDimension(fileId, dimensions[j]->getName());
					if (dimId == -1) {
						dimId = NetCDF::defineDimension(fileId, dimensions[j]->getName(), dimensions[j]->getSize());
					}
					dimIds[j] = dimId;
				}
				coordinateDimIds.insert(make_pair(variableDescriptor, dimIds));
			}
}

void VgtWriter::defineCoordinateVariables(int fileId, const string& segmentName, const Dictionary& dict, const map<const VariableDescriptor*, valarray<int> >& coordinateDimIds) {
	vector<VariableDescriptor*> variableDescriptors;
	if (isTiePointSegment(segmentName)) {
		variableDescriptors = getSubsampledCoordinateVariableDescriptors(dict);
	} else {
		variableDescriptors = getCoordinateVariableDescriptors(dict);
	}
	foreach (const VariableDescriptor* variableDescriptor, variableDescriptors)
			{
				NetCDF::defineVariable(fileId, variableDescriptor->getName(), variableDescriptor->getType(), coordinateDimIds.at(variableDescriptor));
			}
}

vector<int> VgtWriter::getFileIds() const {
	typedef pair<string, int> Entry;

	vector<int> fileIds(ncFileIdMap.size());
	foreach(const Entry entry, ncFileIdMap)
			{
				fileIds.push_back(entry.second);
			}
	return fileIds;
}

vector<VariableDescriptor*> VgtWriter::getSubsampledCoordinateVariableDescriptors(const Dictionary& dict) const {
	const vector<SegmentDescriptor*> coordinateSegmentDescriptors = getCoordinateSegmentDescriptors(dict);

	vector<VariableDescriptor*> subsampledCoordinateVariableDescriptors;
	foreach (const SegmentDescriptor* coordinateSegmentDescriptor, coordinateSegmentDescriptors)
			{
				if (isTiePointCoordinateSegment(coordinateSegmentDescriptor->getName())) {
					vector<VariableDescriptor*> variableDescriptors = coordinateSegmentDescriptor->getVariableDescriptors();
					foreach(VariableDescriptor* v, variableDescriptors)
							{
								subsampledCoordinateVariableDescriptors.push_back(v);
							}
				}
			}
	return subsampledCoordinateVariableDescriptors;
}

vector<VariableDescriptor*> VgtWriter::getCoordinateVariableDescriptors(const Dictionary& dict) const {
	const vector<SegmentDescriptor*> coordinateSegmentDescriptors = getCoordinateSegmentDescriptors(dict);

	vector<VariableDescriptor*> coordinateVariableDescriptors;
	foreach (SegmentDescriptor* coordinateSegmentDescriptor, coordinateSegmentDescriptors)
			{
				if (!isTiePointCoordinateSegment(coordinateSegmentDescriptor->getName())) {
					vector<VariableDescriptor*> variableDescriptors = coordinateSegmentDescriptor->getVariableDescriptors();
					foreach(VariableDescriptor* v, variableDescriptors)
							{
								coordinateVariableDescriptors.push_back(v);
							}
				}
			}

	return coordinateVariableDescriptors;
}

vector<SegmentDescriptor*> VgtWriter::getCoordinateSegmentDescriptors(const Dictionary& dict) const {
	const ProductDescriptor& productDescriptor = dict.getProductDescriptor(getProductId());
	const vector<SegmentDescriptor*> allSegmentDescriptors = productDescriptor.getSegmentDescriptors();

	vector<SegmentDescriptor*> coordinateSegmentDescriptors;
	foreach (SegmentDescriptor* segmentDescriptor, allSegmentDescriptors)
			{
				if (isCoordinateSegmentDescriptor(*segmentDescriptor)) {
					coordinateSegmentDescriptors.push_back(segmentDescriptor);
				}
			}
	return coordinateSegmentDescriptors;
}

bool VgtWriter::isTiePointCoordinateSegment(const string& segmentName) {
	return segmentName.compare(Constants::SEGMENT_VGT_LAT_TP) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LON_TP) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LAT_TP_BNDS) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LON_TP_BNDS) == 0;
}

bool VgtWriter::isTiePointSegment(const string& segmentName) {
	return segmentName.compare(Constants::SEGMENT_VGT_TP) == 0;
}

bool VgtWriter::isCoordinateSegmentDescriptor(const SegmentDescriptor& segmentDescriptor) {
	const string& segmentName = segmentDescriptor.getName();
	return segmentName.compare(Constants::SEGMENT_VGT_LAT) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LAT_TP) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LON) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LON_TP) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LAT_BNDS) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LAT_TP_BNDS) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LON_BNDS) == 0 ||
			segmentName.compare(Constants::SEGMENT_VGT_LON_TP_BNDS) == 0;
}
