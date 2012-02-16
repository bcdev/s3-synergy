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

#include <cstdlib>

#include "../core/JobOrder.h"
#include "../core/NetCDF.h"
#include "../util/Utils.h"

#include "SynL1Reader.h"

using std::getenv;
using std::min;
using std::max;

SynL1Reader::SynL1Reader() :
        BasicModule("SY1_READER"), addedSegments() {
}

SynL1Reader::~SynL1Reader() {
	pair<string, int> fileIdPair;
	reverse_foreach(fileIdPair, ncFileIdMap) {
	    try {
	        NetCDF::closeFile(fileIdPair.second);
	    } catch (exception& ignored) {
	        // ok
	    }
	}
}

long SynL1Reader::getSegmentSize(const string& segmentName, const long rowCount) const {
	if (segmentName.compare(Constants::SEGMENT_GEO) == 0) {
		return rowCount;
	}
	if (rowCount < segmentLineCount) {
		return rowCount;
	}
	return segmentLineCount;
}

void SynL1Reader::start(Context& context) {
	segmentLineCount = 400;
	const string segmentLineCountString = context.getJobOrder().getIpfConfiguration().getDynamicProcessingParameter("Segment_Line_Count");
	if (!segmentLineCountString.empty()) {
		segmentLineCount = lexical_cast<long>(segmentLineCountString);
	}
	context.getLogging().info("Segment line count is " + lexical_cast<string>(segmentLineCount), getId());

	const Dictionary& dict = context.getDictionary();
	const vector<SegmentDescriptor*> segmentDescriptors = dict.getProductDescriptor(Constants::PRODUCT_SY1).getSegmentDescriptors();

	sourceDirPath = path(context.getSourcePath());
	if (!sourceDirPath.has_root_directory()) {
		sourceDirPath = Constants::S3_SYNERGY_HOME / sourceDirPath;
	}
	context.getLogging().info("Source product path is '" + sourceDirPath.string() + "'", getId());

	foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors) {
	    const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
	    const string& segmentName = segmentDescriptor->getName();

	    foreach(VariableDescriptor* variableDescriptor, variableDescriptors) {
	        const string varName = variableDescriptor->getName();
	        const string ncVarName = variableDescriptor->getNcVarName();
	        const string ncFileBasename = variableDescriptor->getNcFileBasename();
	        const int fileId = getNcFile(ncFileBasename);
	        const int varId = NetCDF::getVariableId(fileId, ncVarName.c_str());
	        const int dimCount = NetCDF::getDimensionCount(fileId, varId);
	        const valarray<int> dimIds = NetCDF::getDimensionIds(fileId, varId);

	        long camCount;
	        long rowCount;
	        long colCount;

	        // Set grid parameters and write them to dictionary
	        switch (dimCount) {
	        case 3:
	            camCount = NetCDF::getDimensionLength(fileId, dimIds[0]);
	            rowCount = NetCDF::getDimensionLength(fileId, dimIds[1]);
	            colCount = NetCDF::getDimensionLength(fileId, dimIds[2]);
	            addDimensionToDictionary(fileId, dimIds[0], *variableDescriptor, camCount);
	            addDimensionToDictionary(fileId, dimIds[1], *variableDescriptor, rowCount);
	            addDimensionToDictionary(fileId, dimIds[2], *variableDescriptor, colCount);
	            break;
	        case 2:
	            camCount = 1;
	            rowCount = NetCDF::getDimensionLength(fileId, dimIds[0]);
	            colCount = NetCDF::getDimensionLength(fileId, dimIds[1]);
	            addDimensionToDictionary(fileId, dimIds[0], *variableDescriptor, rowCount);
	            addDimensionToDictionary(fileId, dimIds[1], *variableDescriptor, colCount);
	            break;
	        case 1:
	            camCount = 1;
	            rowCount = 1;
	            colCount = NetCDF::getDimensionLength(fileId, dimIds[0]);
	            addDimensionToDictionary(fileId, dimIds[0], *variableDescriptor, colCount);
	            break;
	        default:
	            BOOST_THROW_EXCEPTION( runtime_error("Invalid number of dimensions for variable '" + ncVarName + "'."));
	            break;
	        }
	        // Create a new segment, if necessary
	        if (!context.hasSegment(segmentName)) {
	            const long sizeL = getSegmentSize(segmentName, rowCount);
	            context.getLogging().info("Adding segment '" + segmentName + "' to context", getId());
	            context.addSwathSegment(segmentName, sizeL, colCount, camCount, 0, rowCount - 1);
	            addedSegments.push_back(segmentName);
	        }

	        // Copy variable attributes to dictionary
	        const int type = NetCDF::getVariableType(fileId, varId);
	        variableDescriptor->setType(type);
	        const size_t attrCount = NetCDF::getAttributeCount(fileId, varId);
	        for (size_t i = 0; i < attrCount; i++) {
	            const string attrName = NetCDF::getAttributeName(fileId, varId, i);
	            if(!variableDescriptor->hasAttribute(attrName)) {
	                const Attribute attr = NetCDF::getAttribute(fileId, varId, attrName);
	                context.getLogging().info("Adding attribute '" + attr.toString() + "' to variable '" + varName + "'", getId());
	                variableDescriptor->addAttribute(attr);
	            }
	        }
	        // Add variable to segment
	        Segment& segment = context.getSegment(segmentName);
	        context.getLogging().info("Adding accessor for " + variableDescriptor->toString() + " to segment " + segment.toString(), getId());
	        segment.addVariable(*variableDescriptor);
	        ncVarIdMap[varName] = varId;
	    }
	}
}

void SynL1Reader::stop(Context& context) {
	pair<string, int> fileIdPair;
	reverse_foreach (fileIdPair, ncFileIdMap) {
	    context.getLogging().info("Closing netCDF file '" + fileIdPair.first + ".nc'", getId());
	    NetCDF::closeFile(fileIdPair.second);
	}
	ncVarIdMap.clear();
	ncFileIdMap.clear();

	reverse_foreach (string segment, addedSegments) {
	    context.removeSegment(segment);
	}
	addedSegments.clear();
}

void SynL1Reader::process(Context& context) {
	const Dictionary& dict = context.getDictionary();
	const vector<SegmentDescriptor*> segmentDescriptors = dict.getProductDescriptor(Constants::PRODUCT_SY1).getSegmentDescriptors();

	valarray<size_t> starts;
	valarray<size_t> counts;

	foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors) {
	    const Segment& segment = context.getSegment(segmentDescriptor->getName());
	    const Grid& grid = segment.getGrid();
	    if (!context.hasLastComputedL(segment, *this) || context.getLastComputedL(segment, *this) < grid.getMinInMemoryL() + grid.getSizeL() - 1) {
	        const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
	        const long firstL = segment.getGrid().getMinInMemoryL();
	        context.getLogging().debug("Segment [" + segment.toString() + "]: firstL = " + lexical_cast<string>(firstL), getId());
	        const long lastL = segment.getGrid().getMaxInMemoryL();
	        context.getLogging().debug("Segment [" + segment.toString() + "]: lastL = " + lexical_cast<string>(lastL), getId());

	        foreach(VariableDescriptor* variableDescriptor, variableDescriptors) {
	            const string varName = variableDescriptor->getName();
	            const string ncFileName = variableDescriptor->getNcFileBasename();

	            if (!contains(ncVarIdMap, varName)) {
	                BOOST_THROW_EXCEPTION(logic_error("Unknown variable '" + varName + "'."));
	            }
	            if (!contains(ncFileIdMap, ncFileName)) {
	                BOOST_THROW_EXCEPTION(logic_error("Unknown netCDF file '" + ncFileName + "'."));
	            }
	            const int varId = ncVarIdMap[varName];
	            const int fileId = ncFileIdMap[ncFileName];
	            const size_t dimCount = variableDescriptor->getDimensions().size();
	            Utils::createStartVector(dimCount, firstL, starts);
	            Utils::createCountVector(dimCount, grid.getSizeK(), lastL - firstL + 1, grid.getSizeM(), counts);
	            context.getLogging().progress("Reading variable '" + varName + "' into segment " + segment.toString(), getId());
	            const Accessor& accessor = segment.getAccessor(varName);
	            NetCDF::getVariableData(fileId, varId, starts, counts, accessor.getUntypedData());
	        }
	        context.setLastComputedL(segment, *this, lastL);
	    }
	}
}

int SynL1Reader::getNcFile(const string& ncFileBasename) {
	if (contains(ncFileIdMap, ncFileBasename)) {
		return ncFileIdMap[ncFileBasename];
	}
	const path ncFilePath = sourceDirPath / (ncFileBasename + ".nc");
	const int fileId = NetCDF::openFileReadOnly(ncFilePath.string());
	ncFileIdMap[ncFileBasename] = fileId;

	return fileId;
}

void SynL1Reader::addDimensionToDictionary(int fileId, int dimId, VariableDescriptor& variableDescriptor, size_t dimSize) const {
    const string& dimName = NetCDF::getDimensionName(fileId, dimId);
    if(variableDescriptor.hasDimension(dimName) ) {
        variableDescriptor.getDimension(dimName).setSize(dimSize);
    } else {
        variableDescriptor.addDimension(dimName).setSize(dimSize);
    }
}
