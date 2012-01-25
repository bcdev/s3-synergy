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
		BasicModule("SEG_WRITER") {
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
	valarray<size_t> origin;
	valarray<size_t> shape;
	foreach(const string& segmentName, segmentIds) {
	    const Segment& segment = context.getSegment(segmentName);
	    const Grid& grid = segment.getGrid();
	    const long firstL = segment.getGrid().getFirstL();
	    context.getLogging().debug("Segment [" + segment.toString() + "]: firstL = " + lexical_cast<string>(firstL), getId());
	    const long lastL = segment.getGrid().getLastL();
	    context.getLogging().debug("Segment [" + segment.toString() + "]: lastL = " + lexical_cast<string>(lastL), getId());

	    if (firstL <= lastL) {
	        const vector<string> variableNames = segment.getVariableNames();
	        foreach(const string varName, variableNames) {
	            const string ncFileBasename = segment.getId();
	            const string variableKey = segmentName + varName;
	            if (!contains(ncVarIdMap, variableKey)) {
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
	            IOUtils::createStartVector(dimIds.size(), firstL, origin);
	            IOUtils::createCountVector(dimIds.size(), grid.getSizeK(), lastL - firstL + 1, grid.getSizeM(), shape);
	            context.getLogging().progress("Writing variable " + varName + " of segment [" + segment.toString() + "]", getId());
	            const Accessor& accessor = segment.getAccessor(varName);
	            if (accessor.canReturnDataPointer()) {
	                NetCDF::putData(ncId, varId, origin, shape, accessor.getUntypedData());
	            } else {
	                const void* data = getData(accessor, firstL, lastL, grid);
	                NetCDF::putData(ncId, varId, origin, shape, data);
	            }
	        }
	        context.setLastComputedL(segment, *this, lastL);
	    }
	}
}

void SegmentWriter::start(Context& context) {
	targetDirPath = path(context.getJobOrder().getIpfProcessors().at(0).getOutputList().at(0).getFileName());
	targetDirPath = targetDirPath.parent_path() / (targetDirPath.stem() + ".BREAKPOINT");
	if (!targetDirPath.has_root_directory()) {
		targetDirPath = Constants::S3_SYNERGY_HOME / targetDirPath;
	}
	context.getLogging().info("target product path is '" + targetDirPath.string() + "'", getId());

	const vector<string> segmentIds = context.getSegmentIds();

	foreach(string segmentId, segmentIds) {
	    const Segment& segment = context.getSegment(segmentId);
	    const vector<string> variableNames = segment.getVariableNames();

	    foreach(string variableName, variableNames) {
	        context.getLogging().info("Defining variable for " + variableName, getId());
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

void* SegmentWriter::getData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    switch (accessor.getType()) {
    case Constants::TYPE_BYTE:
        return getByteData(accessor, firstL, lastL, grid);
    case Constants::TYPE_SHORT:
        return getShortData(accessor, firstL, lastL, grid);
    case Constants::TYPE_INT:
        return getIntData(accessor, firstL, lastL, grid);
    case Constants::TYPE_LONG:
        return getLongData(accessor, firstL, lastL, grid);
    case Constants::TYPE_UBYTE:
        return getUByteData(accessor, firstL, lastL, grid);
    case Constants::TYPE_USHORT:
        return getUShortData(accessor, firstL, lastL, grid);
    case Constants::TYPE_UINT:
        return getUIntData(accessor, firstL, lastL, grid);
    case Constants::TYPE_ULONG:
        return getULongData(accessor, firstL, lastL, grid);
    case Constants::TYPE_FLOAT:
        return getFloatData(accessor, firstL, lastL, grid);
    case Constants::TYPE_DOUBLE:
        return getDoubleData(accessor, firstL, lastL, grid);
    default:
        BOOST_THROW_EXCEPTION(runtime_error("Unsupported variable type."));
        break;
    }
}

void* SegmentWriter::getByteData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<int8_t> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getByte(index);
            counter++;
        }
    }
    return &data[0];
}

void* SegmentWriter::getShortData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<int16_t> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getShort(index);
            counter++;
        }
    }
    return &data[0];
}

void* SegmentWriter::getIntData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<int32_t> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getInt(index);
            counter++;
        }
    }
    return &data[0];
}

void* SegmentWriter::getLongData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<int64_t> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getLong(index);
            counter++;
        }
    }
    return &data[0];
}

void* SegmentWriter::getUByteData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<uint8_t> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getUByte(index);
            counter++;
        }
    }
    return &data[0];
}

void* SegmentWriter::getUShortData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<uint16_t> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getUShort(index);
            counter++;
        }
    }
    return &data[0];
}

void* SegmentWriter::getUIntData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<uint32_t> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getUInt(index);
            counter++;
        }
    }
    return &data[0];
}

void* SegmentWriter::getULongData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<uint64_t> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getULong(index);
            counter++;
        }
    }
    return &data[0];
}

void* SegmentWriter::getFloatData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<float> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getFloat(index);
            counter++;
        }
    }
    return &data[0];
}

void* SegmentWriter::getDoubleData(const Accessor& accessor, long firstL, long lastL, const Grid& grid) const {
    size_t counter = 0;
    valarray<double> data(accessor.getSampleCount());
    for (long l = firstL; l <= lastL; l++) {
        for (long m = grid.getFirstM(); m <= grid.getMaxM(); m++) {
            const size_t index = grid.getIndex(0, l, m);
            data[counter] = accessor.getDouble(index);
            counter++;
        }
    }
    return &data[0];
}
