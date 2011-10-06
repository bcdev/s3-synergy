/*
 * File:   SegmentWriter.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <stdexcept>

#include "../util/IOUtils.h"
#include "../util/NetCDF.h"

#include "SegmentWriter.h"

using std::runtime_error;

SegmentWriter::SegmentWriter() :
		Writer("SY2_WRITER") {
}

SegmentWriter::~SegmentWriter() {
	pair<string, int> fileIdPair;
	foreach(fileIdPair, ncFileIdMap) {
	    try {
	        NetCDF::closeFile(fileIdPair.second);
	    } catch (exception& ignored) {
	    }
	}
}

void SegmentWriter::process(Context& context) {

    const vector<string> segmentIds = context.getSegmentIds();
	foreach(const string& segmentName, segmentIds) {
	    const Segment& segment = context.getSegment(segmentName);
		const Grid& grid = segment.getGrid();
		const long firstL = segment.getGrid().getFirstL();
		context.getLogging()->debug("Segment [" + segment.toString() + "]: firstL = " + lexical_cast<string>(firstL), getId());
		const long lastL = segment.getGrid().getLastL();
		context.getLogging()->debug("Segment [" + segment.toString() + "]: lastL = " + lexical_cast<string>(lastL), getId());

		if (firstL <= lastL) {
		    const vector<string> variableNames = segment.getVariableNames();
		    foreach(const string varName, variableNames) {
		        const string ncFileBasename = segment.getId();
		        const string variableKey = segmentName + varName;
		        if(!contains(ncVarIdMap, variableKey)) {
		            continue;
		        }
		        if (!contains(ncFileIdMap, ncFileBasename)) {
		            continue;
		        }
		        if (!contains(ncDimIdMap, ncFileBasename)) {
		            continue;
		        }
		        const int varId = ncVarIdMap[segmentName + varName];
		        const int ncId = ncFileIdMap[ncFileBasename];
		        const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
		        const valarray<size_t> starts = IOUtils::createStartVector(dimIds.size(), firstL);
		        const valarray<size_t> sizes = IOUtils::createCountVector(dimIds.size(), grid.getSizeK(), lastL - firstL + 1, grid.getSizeM());
		        context.getLogging()->progress("Writing variable " + varName + " of segment [" + segment.toString() + "]", getId());
		        const Accessor& accessor = segment.getAccessor(varName);
		        NetCDF::putData(ncId, varId, starts, sizes, accessor.getUntypedData());
		    }
		    context.setLastComputedL(segment, *this, lastL);
		}
	}
}

void SegmentWriter::start(Context& context) {
	targetDirPath = path(context.getJobOrder()->getIpfProcessors().at(0).getOutputList().at(0).getFileName());
	targetDirPath = targetDirPath.parent_path() / (targetDirPath.stem() + ".BREAKPOINT");
	if (!targetDirPath.has_root_directory()) {
		targetDirPath = getInstallationPath() / targetDirPath;
	}
	context.getLogging()->info("target product path is '" + targetDirPath.string() + "'", getId());

	const vector<string> segmentIds = context.getSegmentIds();

	foreach(string segmentId, segmentIds) {
	    const Segment& segment = context.getSegment(segmentId);
	    const vector<string> variableNames = segment.getVariableNames();

	    foreach(string variableName, variableNames) {
	        context.getLogging()->info("Defining variable for " + variableName, getId());
	        createNcVar(segment, variableName);
	    }
	}

	pair<string, int> fileIdPair;

	foreach(fileIdPair, ncFileIdMap) {
	    NetCDF::terminateFile(fileIdPair.second);
	}
}

void SegmentWriter::stop(Context& context) {
	pair<string, int> fileIdPair;

	foreach(fileIdPair, ncFileIdMap) {
	    NetCDF::closeFile(fileIdPair.second);
	}
	ncVarIdMap.clear();
	ncDimIdMap.clear();
	ncFileIdMap.clear();
}

void SegmentWriter::createNcVar(const Segment& segment, const string& varName) {
	const string ncFileBasename = segment.getId();

	if (!contains(ncFileIdMap, ncFileBasename)) {
		if (!boost::filesystem::exists(targetDirPath)) {
			if (!boost::filesystem::create_directories(targetDirPath)) {
				BOOST_THROW_EXCEPTION( runtime_error("Cannot create directory '" + targetDirPath.string() + "'."));
			}
		}
		const path ncFilePath = targetDirPath / (ncFileBasename + ".nc");
		const int fileId = NetCDF::createFile(ncFilePath.string());

		const Grid& grid = segment.getGrid();

		const long sizeK = grid.getSizeK();
		const long sizeL = grid.getMaxL() - grid.getMinL() + 1;
		const long sizeM = grid.getSizeM();

		valarray<int> dimIds(3);
		dimIds[0] = NetCDF::defineDimension(fileId, "K", sizeK);
		dimIds[1] = NetCDF::defineDimension(fileId, "L", sizeL);
		dimIds[2] = NetCDF::defineDimension(fileId, "M", sizeM);

		ncFileIdMap[ncFileBasename] = fileId;
		ncDimIdMap.insert(make_pair(ncFileBasename, dimIds));
	}
	const int fileId = ncFileIdMap[ncFileBasename];
	const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
	const Accessor& accessor = segment.getAccessor(varName);
	const int varId = NetCDF::defineVariable(fileId, varName, accessor.getType(), dimIds);
	const string variableKey = segment.getId() + varName;
	ncVarIdMap[variableKey] = varId;

	const Attribute fillValue(accessor.getType(), "_FillValue", accessor.getFillValue());
	NetCDF::putAttribute(fileId, varId, fillValue);

	const Attribute scaleFactor(Constants::TYPE_FLOAT, "scale_factor", lexical_cast<string>(accessor.getScaleFactor()));
	NetCDF::putAttribute(fileId, varId, scaleFactor);

	const Attribute addOffset(Constants::TYPE_FLOAT, "add_offset", lexical_cast<string>(accessor.getAddOffset()));
	NetCDF::putAttribute(fileId, varId, addOffset);

}