/*
 * File:   SynL2Writer.cpp
 * Author: thomass
 *
 * Created on November 17, 2010, 3:40 PM
 */

#include <fstream>
#include <stdexcept>

#include "../core/Config.h"
#include "../util/IOUtils.h"
#include "../util/NetCDF.h"

#include "SynL2Writer.h"

using std::runtime_error;

SynL2Writer::SynL2Writer() :
        BasicModule("SY2_WRITER") {
}

SynL2Writer::~SynL2Writer() {
	pair<string, int> fileIdPair;
	foreach(fileIdPair, ncFileIdMap)
			{
				try {
					NetCDF::closeFile(fileIdPair.second);
				} catch (exception& ignored) {
				}
			}
}

void SynL2Writer::process(Context& context) {
	const Dictionary& dict = context.getDictionary();
	const vector<SegmentDescriptor*> segmentDescriptors = dict.getProductDescriptor(Constants::PRODUCT_SY2).getSegmentDescriptors();

	foreach(const SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const string segmentName = segmentDescriptor->getName();
				if (context.hasSegment(segmentName)) {
					const Segment& segment = context.getSegment(segmentName);
					const Grid& grid = segment.getGrid();
					const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();
					const long firstL = segment.getGrid().getFirstL();
					context.getLogging().debug("Segment [" + segment.toString() + "]: firstL = " + lexical_cast<string>(firstL), getId());
					const long lastL = segment.getGrid().getLastL();
					context.getLogging().debug("Segment [" + segment.toString() + "]: lastL = " + lexical_cast<string>(lastL), getId());

					if (firstL <= lastL) {
						foreach(const VariableDescriptor* variableDescriptor, variableDescriptors)
								{
									const string varName = variableDescriptor->getName();
									if (segment.hasVariable(varName)) {
										const string ncFileBasename = variableDescriptor->getNcFileBasename();
										if (!contains(ncVarIdMap, varName)) {
											BOOST_THROW_EXCEPTION( logic_error("Unknown variable '" + varName + "'."));
										}
										if (!contains(ncFileIdMap, ncFileBasename)) {
											BOOST_THROW_EXCEPTION( logic_error("Unknown netCDF file '" + ncFileBasename + "'."));
										}
										if (!contains(ncDimIdMap, ncFileBasename)) {
											BOOST_THROW_EXCEPTION( logic_error("Unknown netCDF file '" + ncFileBasename + "'."));
										}
										const int varId = ncVarIdMap[varName];
										const int ncId = ncFileIdMap[ncFileBasename];
										const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
										const valarray<size_t> starts = IOUtils::createStartVector(dimIds.size(), firstL);
										const valarray<size_t> sizes = IOUtils::createCountVector(dimIds.size(), grid.getSizeK(), lastL - firstL + 1, grid.getSizeM());
										context.getLogging().progress("Writing variable " + varName + " of segment [" + segment.toString() + "]", getId());
										const Accessor& accessor = segment.getAccessor(varName);
										NetCDF::putData(ncId, varId, starts, sizes, accessor.getUntypedData());
									}
								}
						context.setLastComputedL(segment, *this, lastL);
					}
				}
			}
}

void SynL2Writer::start(Context& context) {
	targetDirPath = path(context.getJobOrder().getIpfProcessors().at(0).getOutputList().at(0).getFileName());
	if (!targetDirPath.has_root_directory()) {
		targetDirPath = Constants::S3_SYNERGY_HOME / targetDirPath;
	}
	context.getLogging().info("target product path is '" + targetDirPath.string() + "'", getId());

	const ProductDescriptor& productDescriptor = context.getDictionary().getProductDescriptor(Constants::PRODUCT_SY2);
	const vector<SegmentDescriptor*> segmentDescriptors = productDescriptor.getSegmentDescriptors();

	foreach(SegmentDescriptor* segmentDescriptor, segmentDescriptors)
			{
				const string segmentName = segmentDescriptor->getName();
				if (context.hasSegment(segmentName)) {
					const Segment& segment = context.getSegment(segmentName);
					const vector<VariableDescriptor*> variableDescriptors = segmentDescriptor->getVariableDescriptors();

					foreach(VariableDescriptor* variableDescriptor, variableDescriptors)
							{
								context.getLogging().info("Defining variable for " + variableDescriptor->toString(), getId());
								createNcVar(productDescriptor, *segmentDescriptor, *variableDescriptor, segment.getGrid());
							}
				}
			}

	pair<string, int> fileIdPair;

	foreach(fileIdPair, ncFileIdMap)
			{
				NetCDF::terminateFile(fileIdPair.second);
			}
}

void SynL2Writer::stop(Context& context) {
    copyTemplateFiles();
    string manifest = readManifest();
	setStartTime(context, manifest);
	setChecksums(manifest);
	writeManifest(manifest);

	ncVarIdMap.clear();
	ncDimIdMap.clear();
	ncFileIdMap.clear();
}


void SynL2Writer::copyTemplateFiles() const {
    const string sourceDirPath = Constants::S3_SYNERGY_HOME + "/src/main/resources/SAFE_metacomponents";
    const vector<string> fileNames = IOUtils::getFileNames(sourceDirPath);
    foreach(string fileName, fileNames) {
        boost::filesystem::copy_file(sourceDirPath + "/" + fileName, targetDirPath / fileName);
    }
    boost::filesystem::create_directory(path(targetDirPath.string() + "/schema"));
    const vector<string> schemaFileNames = IOUtils::getFileNames(sourceDirPath + "/schema");
    foreach(string fileName, schemaFileNames) {
        boost::filesystem::copy_file(sourceDirPath + "/schema/" + fileName, targetDirPath / "schema" / fileName);
    }
}

string SynL2Writer::readManifest() const {
    std::ifstream ifs(string(targetDirPath.string() + "/manifest_SYN.template").c_str(), std::ifstream::in);
    std::ostringstream oss;
    char c;
    while (ifs.get(c)) {
        oss.put(c);
    }
    ifs.close();
    return oss.str();
}

void SynL2Writer::setStartTime(Context& context, string& manifest) const {
//    struct tm* ptm = localtime(&context.getStartTime());
//    char buffer[32];
//    strftime(buffer, 32, "%Y-%m-%dT%H:%M:%S", ptm);
//    string startTime = string(buffer);
//    replaceString("\\$\\{processing-start-time\\}", startTime, manifest);
}

void SynL2Writer::setChecksums(string& manifest) const {
    pair<string, int> fileIdPair;
    foreach(fileIdPair, ncFileIdMap) {
        string checksum = getMd5Sum(targetDirPath.string() + "/" + fileIdPair.first + ".nc");
        replaceString("\\s*\\$\\{checksum-" + fileIdPair.first + "\\.nc\\}\\xml*", checksum, manifest);
        NetCDF::closeFile(fileIdPair.second);
    }
}

void SynL2Writer::writeManifest(string& manifest) const {
    std::ofstream ofs(string(targetDirPath.string() + "/manifest_SYN.xml").c_str(), std::ofstream::out);
    for(size_t i = 0; i < manifest.size(); i++) {
        ofs.put(manifest[i]);
    }
    ofs.close();
}

void SynL2Writer::replaceString(const string& toReplace, const string& replacement, string& input) const {
    regex expr(toReplace.c_str());
    input = regex_replace(input, expr, replacement);
}

string SynL2Writer::getMd5Sum(const string& file) const {
    FILE* pipe = popen(string(Constants::MD5SUM_EXECUTABLE + " " + file).c_str(), "r");
    if (!pipe || !boost::filesystem::exists(path(file))) {
        BOOST_THROW_EXCEPTION(std::invalid_argument("Could not perform command 'md5sum' on file '" + file + "'."));
    }
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    replaceString("\\s+.*", "", result);
    return result;
}

void SynL2Writer::createNcVar(const ProductDescriptor& productDescriptor, const SegmentDescriptor& segmentDescriptor, const VariableDescriptor& variableDescriptor, const Grid& grid) {
	const string varName = variableDescriptor.getName();
	const string ncFileBasename = variableDescriptor.getNcFileBasename();

	if (!contains(ncFileIdMap, ncFileBasename)) {
		if (!boost::filesystem::exists(targetDirPath)) {
			if (!boost::filesystem::create_directories(targetDirPath)) {
				BOOST_THROW_EXCEPTION( runtime_error("Cannot create directory '" + targetDirPath.string() + "'."));
			}
		}
		const path ncFilePath = targetDirPath / (variableDescriptor.getNcFileBasename() + ".nc");
		const int fileId = NetCDF::createFile(ncFilePath.string());

		foreach(const Attribute* attribute, productDescriptor.getAttributes())
				{
					NetCDF::putAttribute(fileId, NC_GLOBAL, *attribute);
				}

		foreach(const Attribute* attribute, segmentDescriptor.getAttributes())
				{
					NetCDF::putAttribute(fileId, NC_GLOBAL, *attribute);
				}

		const long sizeK = grid.getSizeK();
		const long sizeL = grid.getMaxL() - grid.getMinL() + 1;
		const long sizeM = grid.getSizeM();
		const vector<Dimension*> dimensions = variableDescriptor.getDimensions();
		const size_t dimCount = dimensions.size();

		valarray<int> dimIds(dimCount);
		switch (dimCount) {
		case 3:
			dimIds[0] = NetCDF::defineDimension(fileId, dimensions[0]->getName(), sizeK);
			dimIds[1] = NetCDF::defineDimension(fileId, dimensions[1]->getName(), sizeL);
			dimIds[2] = NetCDF::defineDimension(fileId, dimensions[2]->getName(), sizeM);
			break;
		case 2:
			dimIds[0] = NetCDF::defineDimension(fileId, dimensions[0]->getName(), sizeL);
			dimIds[1] = NetCDF::defineDimension(fileId, dimensions[1]->getName(), sizeM);
			break;
		case 1:
			dimIds[0] = NetCDF::defineDimension(fileId, dimensions[0]->getName(), sizeM);
			break;
		default:
			BOOST_THROW_EXCEPTION( logic_error("SynL2Writer::createNcVar(): invalid number of dimensions (" + variableDescriptor.getName() + ")"));
			break;
		}

		ncFileIdMap[ncFileBasename] = fileId;
		ncDimIdMap.insert(make_pair(ncFileBasename, dimIds));
	}
	const int fileId = ncFileIdMap[ncFileBasename];
	const valarray<int>& dimIds = ncDimIdMap[ncFileBasename];
	const int varId = NetCDF::defineVariable(fileId, variableDescriptor.getNcVarName(), variableDescriptor.getType(), dimIds);
	ncVarIdMap[varName] = varId;

	foreach(const Attribute* attribute, variableDescriptor.getAttributes())
			{
				NetCDF::putAttribute(fileId, varId, *attribute);
			}
}
