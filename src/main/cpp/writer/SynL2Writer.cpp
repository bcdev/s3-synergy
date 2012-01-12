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

SynL2Writer::SynL2Writer() : AbstractWriter("SY2_WRITER") {

}

const string SynL2Writer::getProductDescriptorIdentifier() const {
    return Constants::PRODUCT_SY2;
}

void SynL2Writer::createSafeProduct(const Context& context) {
    copyTemplateFiles();
    string manifest = readManifest();
	setStartTime(context, manifest);
	setChecksums(manifest);
	writeManifest(manifest);
	removeManifestTemplate();
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

void SynL2Writer::setStartTime(const Context& context, string& manifest) const {
    struct tm* ptm = localtime(&context.getStartTime());
    char buffer[32];
    strftime(buffer, 32, "%Y-%m-%dT%H:%M:%S", ptm);
    string startTime = string(buffer);
    replaceString("\\$\\{processing-start-time\\}", startTime, manifest);
}

void SynL2Writer::setChecksums(string& manifest) const {
    pair<string, int> fileIdPair;
    foreach(fileIdPair, ncFileIdMap) {
        string checksum = getMd5Sum(targetDirPath.string() + "/" + fileIdPair.first + ".nc");
        replaceString("\\s*\\$\\{checksum-" + fileIdPair.first + "\\.nc\\}\\s*", checksum, manifest);
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

void SynL2Writer::removeManifestTemplate() const {
    path manifestTemplate = path(targetDirPath.string() + "/manifest_SYN.template");
    boost::filesystem::remove(manifestTemplate);
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
