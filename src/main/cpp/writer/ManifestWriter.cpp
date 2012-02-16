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

#include <fstream>
#include <stdexcept>

#include "../core/Constants.h"
#include "../util/Utils.h"

#include "ManifestWriter.h"

ManifestWriter::ManifestWriter(const string& productId) : BasicModule("MAN_WRITER"), manifestName("manifest") {
}

ManifestWriter::~ManifestWriter() {
}

void ManifestWriter::start(Context& context) {
	targetDirPath = path(context.getJobOrder().getIpfProcessors().at(0).getOutputList().at(0).getFileName());
	if (!targetDirPath.has_root_directory()) {
		targetDirPath = Constants::S3_SYNERGY_HOME / targetDirPath;
	}
	context.getLogging().info("target product path is '" + targetDirPath.string() + "'", getId());
}

void ManifestWriter::stop(Context& context) {
	copyTemplateFiles();
	string manifest = readManifest();
	setStartTime(context, manifest);
	setChecksums(manifest);
	writeManifest(manifest);
	removeManifestTemplate();
}

void ManifestWriter::copyTemplateFiles() const {
	const string sourceDirPath = Constants::S3_SYNERGY_HOME + "/src/main/resources/SAFE_metacomponents/" + productId;
	const vector<string> fileNames = Utils::getFileNames(sourceDirPath);
	foreach(string fileName, fileNames)
			{
				boost::filesystem::copy_file(sourceDirPath + "/" + fileName, targetDirPath / fileName, copy_option::overwrite_if_exists);
			}
	boost::filesystem::create_directory(path(targetDirPath.string() + "/schema"));
	const vector<string> schemaFileNames = Utils::getFileNames(sourceDirPath + "/schema");
	foreach(string fileName, schemaFileNames)
			{
				boost::filesystem::copy_file(sourceDirPath + "/schema/" + fileName, targetDirPath / "schema" / fileName, copy_option::overwrite_if_exists);
			}
}

string ManifestWriter::readManifest() const {
	std::ifstream ifs(string(targetDirPath.string() + "/" + manifestName + ".template").c_str(), std::ifstream::in);
	std::ostringstream oss;
	char c;
	while (ifs.get(c)) {
		oss.put(c);
	}
	ifs.close();
	return oss.str();
}

void ManifestWriter::setStartTime(const Context& context, string& manifest) const {
    time_t t = context.getStartTime();
	struct tm* ptm = localtime(&t);
	char buffer[32];
	strftime(buffer, 32, "%Y-%m-%dT%H:%M:%S", ptm);
	string startTime(buffer);
	Utils::replaceString("\\$\\{processing-start-time\\}", startTime, manifest);
}

void ManifestWriter::setChecksums(string& manifest) const {
	const vector<path> filePaths = Utils::getFilePaths(targetDirPath.string(), ".nc");
	foreach (path filePath, filePaths)
			{
				string checksum = getMd5Sum(filePath.string());
				Utils::replaceString("\\s*\\$\\{checksum-" + filePath.filename().string() + "\\}\\s*", checksum, manifest);
			}
}

void ManifestWriter::writeManifest(string& manifest) const {
	std::ofstream ofs(string(targetDirPath.string() + "/" + manifestName + ".xml").c_str(), std::ofstream::out);
	for (size_t i = 0; i < manifest.size(); i++) {
		ofs.put(manifest[i]);
	}
	ofs.close();
}

void ManifestWriter::removeManifestTemplate() const {
	const path manifestTemplate(targetDirPath.string() + "/" + manifestName + ".template");
	boost::filesystem::remove(manifestTemplate);
}

string ManifestWriter::getMd5Sum(const string& file) {
	FILE* pipe = popen(string(Constants::MD5SUM_EXECUTABLE + " " + file).c_str(), "r");
	if (!pipe || !boost::filesystem::exists(path(file))) {
		BOOST_THROW_EXCEPTION(std::runtime_error("Could not perform command 'md5sum' on file '" + file + "'."));
	}
	char buffer[128];
	std::string result = "";
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	Utils::replaceString("\\s+.*", "", result);

	return result;
}
