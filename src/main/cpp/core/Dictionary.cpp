/*
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * File:   Dictionary.cpp
 * Author: thomass
 *
 * Created on December 21, 2010, 1:55 PM
 */

#include "Dictionary.h"
#include "../util/IOUtils.h"
#include "Constants.h"

Attribute::Attribute(int type, const string& name, const string& value) : name(name), type(type) {
    this->value = value;
}

Attribute::~Attribute() {
}

string Attribute::toString() const {
    std::ostringstream oss;
    oss << "Attribute " << "[";
    oss << "type = " << type << ", ";
    oss << "name = " << name << ", ";
    oss << "value = " << value << "]";
    return oss.str();
}

Dimension::Dimension(const string& name, size_t size) : name(name) {
    this->size = size;
}

Dimension::~Dimension() {
}

string Dimension::toString() const {
    std::ostringstream oss;
    oss << "Dimension: " << "[";
    oss << "name = " << name << ", ";
    oss << "size = " << size << "]";
    return oss.str();
}

VariableDescriptor::VariableDescriptor(const string& name) : Descriptor<Attribute, Dimension>(name) {
    type = 0;
    ncFileName = "";
    ncVarName = "";
    segmentName = "";
}

VariableDescriptor::~VariableDescriptor() {
}

vector<VariableDescriptor*> ProductDescriptor::getVariables() const {
    vector<VariableDescriptor*> variableDescriptors;
    foreach(SegmentDescriptor* segmentDescriptor, getSegmentDescriptors()) {
        foreach(VariableDescriptor* variableDescriptor, segmentDescriptor->getVariableDescriptors()) {
            variableDescriptors.push_back(variableDescriptor);
        }
    }
    return variableDescriptors;
}

VariableDescriptor* ProductDescriptor::getVariable(const string& name) const {
    foreach(SegmentDescriptor* segmentDescriptor, getSegmentDescriptors()) {
        foreach(VariableDescriptor* variableDescriptor, segmentDescriptor->getVariableDescriptors()) {
            if (variableDescriptor->getName().compare(name) == 0) {
                return variableDescriptor;
            }
        }
    }
    throw std::invalid_argument("Dictionary::getVariable: no variable descriptor with name '" + name + "'.");
}

string VariableDescriptor::toString() const {
    std::ostringstream oss;
    oss << "Variable " << "[";
    oss << "name = " << getName() << ", ";
    oss << "type = " << type << ", ";
    oss << "ncFileName = " << ncFileName << ", ";
    oss << "ncVarName = " << ncVarName << "]";

    return oss.str();
};

Dictionary::Dictionary() : Descriptor<Attribute, ProductDescriptor>("Dictionary") {
}

Dictionary::Dictionary(string config) : Descriptor<Attribute, ProductDescriptor>("Dictionary"), configFile(config) {
    init();
}

Dictionary::~Dictionary() {
}

void Dictionary::init() {
    const string L1C_IDENTIFIER = Constants::SYMBOLIC_NAME_L1C;
    const string L2_IDENTIFIER = Constants::SYMBOLIC_NAME_SYN_L2;
    string variableDefPath = xmlParser.evaluateToString(configFile, "/Config/Variable_Definition_Files_Path");
    ProductDescriptor& l1c = addProductDescriptor(L1C_IDENTIFIER);
    ProductDescriptor& l2 = addProductDescriptor(L2_IDENTIFIER);

    string L1CPath = variableDefPath + "/" + L1C_IDENTIFIER;
    string L2SynPath = variableDefPath + "/" + L2_IDENTIFIER;

    vector<string> L1CFiles = IOUtils::getFiles(L1CPath);
    for (size_t i = 0; i < L1CFiles.size(); i++) {
        parseVariablesFile(L1CPath, L1CFiles[i], l1c);
    }
    vector<string> L2SynFiles = IOUtils::getFiles(L2SynPath);
    for (size_t i = 0; i < L2SynFiles.size(); i++) {
        parseVariablesFile(L2SynPath, L2SynFiles[i], l2);
    }
}

void Dictionary::parseVariablesFile(string& variableDefPath, string& file, ProductDescriptor& productDescriptor) {
    string path = variableDefPath + "/" + file;
    const vector<string> ncVariableNames = xmlParser.evaluateToStringList(path, "/dataset/variables/variable/name/child::text()");

    for (size_t i = 0; i < ncVariableNames.size(); i++) {
        string ncVariableName = ncVariableNames[i];
        string query = "/dataset/variables/variable[name=\"" + ncVariableName + "\"]/symbolic_name";
        string symbolicName = xmlParser.evaluateToString(path, query);
        if (symbolicName.empty()) {
            symbolicName = ncVariableName;
        }
        query = "/dataset/variables/variable[name=\"" + ncVariableName + "\"]/segment_name";
        string segmentName = xmlParser.evaluateToString(path, query);
        if (!productDescriptor.hasSegmentDescriptor(segmentName)) {
            productDescriptor.addSegmentDescriptor(segmentName);
        }
        VariableDescriptor& var = productDescriptor.getSegmentDescriptor(segmentName).addVariableDescriptor(symbolicName);
        var.setNcVarName(ncVariableName);
        var.setNcFileName(xmlParser.evaluateToString(path, "/dataset/global_attributes/attribute[name=\"dataset_name\"]/value"));
        var.setSegmentName(segmentName);

        parseAttributes(path, ncVariableName, var);
    }
}

void Dictionary::parseAttributes(string& file, string& variableName, VariableDescriptor& var) {
    string query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute/name/child::text()";
    const vector<string> attributeNames = xmlParser.evaluateToStringList(file, query);
    for (size_t i = 0; i < attributeNames.size(); i++) {
        string attributeName = attributeNames[i];
        int type = mapToNcType(xmlParser.evaluateToString(file, query));
        query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute[name=\"" + attributeName + "\"]/value";
        string value = xmlParser.evaluateToString(file, query);
        var.addAttribute(type, attributeName, value);
    }
}

int Dictionary::mapToNcType(const string& typeString) {
    if (typeString.compare("byte") == 0) {
        return NC_BYTE;
    }
    if (typeString.compare("ubyte") == 0) {
        return NC_UBYTE;
    }
    if (typeString.compare("short") == 0) {
        return NC_SHORT;
    }
    if (typeString.compare("ushort") == 0) {
        return NC_USHORT;
    }
    if (typeString.compare("int") == 0) {
        return NC_INT;
    }
    if (typeString.compare("uint") == 0) {
        return NC_UINT;
    }
    if (typeString.compare("long") == 0) {
        return NC_INT64;
    }
    if (typeString.compare("ulong") == 0) {
        return NC_UINT64;
    }
    if (typeString.compare("float") == 0) {
        return NC_FLOAT;
    }
    if (typeString.compare("double") == 0) {
        return NC_DOUBLE;
    }
    if (typeString.compare("string") == 0) {
        return NC_STRING;
    }
    return NC_CHAR;
}