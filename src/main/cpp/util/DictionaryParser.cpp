/*
 * File:   DictionaryParser.cpp
 * Author: ralf
 *
 * Created on August 10, 2011, 3:19 PM
 */

#include "../core/Boost.h"
#include "../core/Constants.h"

#include "DictionaryParser.h"

DictionaryParser::DictionaryParser() : xmlParser(), exclusionSet() {
    exclusionSet.insert("dimensions.xml");
}

DictionaryParser::DictionaryParser(const DictionaryParser& orig) {
}

DictionaryParser::~DictionaryParser() {
}

Dictionary* DictionaryParser::parse(const string& dictionaryPath) {
    Dictionary* dictionary = new Dictionary();
    vector<string> productDescriptorNames = getDirectoryNames(dictionaryPath);

    foreach(string productDescriptorName, productDescriptorNames) {
        ProductDescriptor& productDescriptor = dictionary->addProductDescriptor(productDescriptorName);
        const string subDirPath = dictionaryPath + "/" + productDescriptorName;
        vector<string> varDescriptorFileNames = getFileNames(subDirPath);
        foreach (string varDescriptorFileName, varDescriptorFileNames) {
            if (!contains(exclusionSet, varDescriptorFileName)) {
                parseVariables(subDirPath, varDescriptorFileName, productDescriptor);
            }
        }
    }
    
    return dictionary;
}

void DictionaryParser::parseVariables(const string& dirPath, const string& descriptorFileName, ProductDescriptor& productDescriptor) const {
    const string descriptorFilePath = dirPath + "/" + descriptorFileName;
    const vector<string> variableNames = xmlParser.evaluateToStringList(descriptorFilePath, "/dataset/variables/variable/name/child::text()");

    foreach (string name, variableNames) {
        string query;
        string ncName;

        query = "/dataset/variables/variable[name=\"" + name + "\"]/ncname";
        ncName = xmlParser.evaluateToString(descriptorFilePath, query);
        if (ncName.empty()) {
            ncName = name;
        }
        
        query = "/dataset/variables/variable[name=\"" + name + "\"]/segment_name";
        const string segmentName = xmlParser.evaluateToString(descriptorFilePath, query);
        if (!productDescriptor.hasSegmentDescriptor(segmentName)) {
            productDescriptor.addSegmentDescriptor(segmentName);
        }
        
        query = "/dataset/variables/variable[name=\"" + name + "\"]/type";
        const int type = toType(xmlParser.evaluateToString(descriptorFilePath, query));
        
        VariableDescriptor& descriptor = productDescriptor.getSegmentDescriptor(segmentName).addVariableDescriptor(name);
        descriptor.setType(type);
        descriptor.setNcName(ncName);
        descriptor.setNcFileName(xmlParser.evaluateToString(descriptorFilePath, "/dataset/global_attributes/attribute[name=\"dataset_name\"]/value"));
        descriptor.setSegmentName(segmentName);

        parseAttributes(descriptorFilePath, name, descriptor);
        parseDimensions(descriptorFilePath, name, descriptor);
    }
}

void DictionaryParser::parseAttributes(const string& filePath, const string& variableName, VariableDescriptor& descriptor) const {
    string query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute/name/child::text()";
    const vector<string> attributeNames = xmlParser.evaluateToStringList(filePath, query);

    foreach (string attributeName, attributeNames) {
        query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute[name=\"" + attributeName + "\"]/type";
        const int type = toType(xmlParser.evaluateToString(filePath, query));
        query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute[name=\"" + attributeName + "\"]/value";
        string value = xmlParser.evaluateToString(filePath, query);
        descriptor.addAttribute(type, attributeName, value);
    }
}

void DictionaryParser::parseDimensions(const string& filePath, const string& variableName, VariableDescriptor& descriptor) const {
    string query = "/dataset/variables/variable[name=\"" + variableName + "\"]/dimensions/id/child::text()";
    string dimensionId = xmlParser.evaluateToString(filePath, query);
    if (dimensionId.empty()) {
        return;
    }
    const string dimensionFilePath = (path(filePath).parent_path() / "dimensions.xml").string();
    query = "/dataset/set[@name=\"" + dimensionId + "\"]/dimensions/dimension/name/child::text()";
    vector<string> dimensionNames = xmlParser.evaluateToStringList(dimensionFilePath, query);

    foreach(string name, dimensionNames) {
        Dimension& d = descriptor.addDimension(name);
        query = "/dataset/set[@name=\"" + dimensionId + "\"]/dimensions/dimension[name=\"" + name + "\"]/size/child::text()";
        const string sizeString = xmlParser.evaluateToString(dimensionFilePath, query);
        if (!sizeString.empty()) {
            const int size = lexical_cast<int>(sizeString);
            d.setSize(size);
        }
    }
}

int DictionaryParser::toType(const string& typeString) const {
    if (typeString.compare("byte") == 0) {
        return Constants::TYPE_BYTE;
    }
    if (typeString.compare("ubyte") == 0) {
        return Constants::TYPE_UBYTE;
    }
    if (typeString.compare("short") == 0) {
        return Constants::TYPE_SHORT;
    }
    if (typeString.compare("ushort") == 0) {
        return Constants::TYPE_USHORT;
    }
    if (typeString.compare("int") == 0) {
        return Constants::TYPE_INT;
    }
    if (typeString.compare("uint") == 0) {
        return Constants::TYPE_UINT;
    }
    if (typeString.compare("long") == 0) {
        return Constants::TYPE_LONG;
    }
    if (typeString.compare("ulong") == 0) {
        return Constants::TYPE_ULONG;
    }
    if (typeString.compare("float") == 0) {
        return Constants::TYPE_FLOAT;
    }
    if (typeString.compare("double") == 0) {
        return Constants::TYPE_DOUBLE;
    }
    if (typeString.compare("string") == 0) {
        return Constants::TYPE_STRING;
    }
    return Constants::TYPE_CHAR;
}

vector<string> DictionaryParser::getDirectoryNames(const string& directory) const {
    vector<string> directoryNames;
    if (is_directory(directory)) {
        for (directory_iterator iter(directory); iter != directory_iterator(); ++iter) {
            if (is_directory(iter->path())) {
                directoryNames.push_back(iter->path().filename());
            }
        }
    }
    return directoryNames;
}

vector<string> DictionaryParser::getFileNames(const string& directory) const {
    vector<string> fileNames;
    if (is_directory(directory)) {
        for (directory_iterator iter(directory); iter != directory_iterator(); ++iter) {
            if (!is_directory(iter->path())) {
                fileNames.push_back(iter->path().filename());
            }
        }
    }
    return fileNames;
}