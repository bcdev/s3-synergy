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

#include <stdexcept>

#include "../core/NetCDF.h"
#include "../util/Utils.h"

#include "SegmentWriter.h"

using std::runtime_error;

using boost::lexical_cast;

SegmentWriter::SegmentWriter() :
		BasicModule("SEG_WRITER") {
}

SegmentWriter::~SegmentWriter() {
	pair<string, int> fileIdPair;
	reverse_foreach (fileIdPair, ncFileIdMap) {
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
	foreach (const string& segmentName, segmentIds) {
	    const Segment& segment = context.getSegment(segmentName);
	    const Grid& grid = segment.getGrid();
	    const long firstL = segment.getGrid().getMinInMemoryL();
	    context.getLogging().debug("Segment [" + segment.toString() + "]: firstL = " + lexical_cast<string>(firstL), getId());
	    const long lastL = segment.getGrid().getMaxInMemoryL();
	    context.getLogging().debug("Segment [" + segment.toString() + "]: lastL = " + lexical_cast<string>(lastL), getId());

	    if (firstL <= lastL) {
	        const vector<string> variableNames = segment.getVariableNames();
	        foreach (const string varName, variableNames) {
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
	            Utils::createStartVector(dimIds.size(), firstL, origin);
	            Utils::createCountVector(dimIds.size(), grid.getSizeK(), lastL - firstL + 1, grid.getSizeM(), shape);
	            context.getLogging().progress("Writing variable " + varName + " of segment [" + segment.toString() + "]", getId());
	            const Accessor& accessor = segment.getAccessor(varName);
	            NetCDF::putData(ncId, varId, origin, shape, accessor.getUntypedData());
	        }
	        context.setLastComputedL(segment, *this, lastL);
	    }
	}
}

void SegmentWriter::start(Context& context) {
	targetDirPath = path(context.getJobOrder().getIpfProcessors().at(0).getOutputList().at(0).getFileName());
	targetDirPath = targetDirPath.parent_path() / (targetDirPath.stem().string() + ".BREAKPOINT");
	if (!targetDirPath.has_root_directory()) {
		targetDirPath = Constants::S3_SYNERGY_HOME / targetDirPath;
	}
	context.getLogging().info("target product path is '" + targetDirPath.string() + "'", getId());

	const vector<string> segmentIds = context.getSegmentIds();

	foreach (string segmentId, segmentIds) {
	    const Segment& segment = context.getSegment(segmentId);
	    const vector<string> variableNames = segment.getVariableNames();

	    foreach (string variableName, variableNames) {
	        context.getLogging().info("Defining variable for " + variableName, getId());
	        createNcVar(segment, variableName);
	    }
	}

	pair<string, int> fileIdPair;

	foreach (fileIdPair, ncFileIdMap) {
	    NetCDF::terminateFile(fileIdPair.second);
	}
}

void SegmentWriter::stop(Context& context) {
	pair<string, int> fileIdPair;

	reverse_foreach (fileIdPair, ncFileIdMap) {
	    NetCDF::closeFile(fileIdPair.second);
	}
	ncVarIdMap.clear();
	ncDimIdMap.clear();
	ncFileIdMap.clear();
}

void SegmentWriter::createNcVar(const Segment& segment, const string& varName) {
	const string& ncFileBasename = segment.getId();

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
