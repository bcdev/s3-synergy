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

#include <iostream>
#include <map>
#include <boost/lexical_cast.hpp>
#include <stdexcept>

#include "Dictionary.h"
#include "VariableImpl.h"

using boost::filesystem::is_directory;
using boost::filesystem::directory_iterator;
using boost::filesystem::path;

Dictionary::Dictionary(string config) : configFile(config) {
}

Dictionary::~Dictionary() {
}

void Dictionary::parseInputFiles() {
    string variableDefPath = xmlParser.evaluateToString(configFile, "/Config/Variable_Definition_Files_Path");

    vector<string> childFolders = getFiles(variableDefPath);
    for (size_t i = 0; i < childFolders.size(); i++) {
        parseVariablesFile(variableDefPath, childFolders[i]);
    }

}

vector<string> Dictionary::getFiles(string& directory) {
    vector<string> files;
    if (is_directory(directory)) {
        for (directory_iterator iter(directory); iter != directory_iterator(); ++iter) {
            path file = iter->path();
            if (!is_directory(file)) {
                files.push_back(file.filename());
            }
        }
    }

    return files;
}

void Dictionary::parseVariablesFile(string& variableDefPath, string& file) {
    string path = variableDefPath + "/" + file;
    const vector<string> variableNames = xmlParser.evaluateToStringList(path, "/dataset/variables/variable/name/child::text()");

    for (size_t i = 0; i < variableNames.size(); i++) {
        string variableName = variableNames[i];
        string query = "/dataset/variables/variable[name=\"" + variableName + "\"]/type";
        NcType type = mapToNcType(xmlParser.evaluateToString(path, query));
        Variable* var = new VariableImpl(variableName, type);
        var->setFileName(xmlParser.evaluateToString(path, "/dataset/global_attributes/attribute[name=\"dataset_name\"]/value"));
        vector<Dimension*> dimensions = parseDimensions(path, variableName);
        for (size_t j = 0; j < dimensions.size(); j++) {
            var->addDimension(dimensions[j]);
        }
        vector<Attribute*> attributes = parseAttributes(path, variableName);
        for (size_t k = 0; k < attributes.size(); k++) {
            var->addAttribute(attributes[k]);
        }
        variables.insert(var);
    }
}

vector<Dimension*> Dictionary::parseDimensions(string& file, string& variableName) {
    string query = "/dataset/variables/variable[name=\"" + variableName + "\"]/dimensions/dimension/name/child::text()";
    const vector<string> dimensionNames = xmlParser.evaluateToStringList(file, query);
    vector<Dimension*> dimensions;
    for (size_t i = 0; i < dimensionNames.size(); i++) {
        query = "/dataset/variables/variable[name=\"" + variableName + "\"]/dimensions/dimension[name=\"" + dimensionNames[i] + "\"]/range";
        string result = xmlParser.evaluateToString(file, query);
        if( result.empty() ) {
            throw std::runtime_error("Dimension " + dimensionNames[i] + " of Variable " + variableName + " has no range." );
        }
        size_t range = boost::lexical_cast<int>(result);
        dimensions.push_back(new Dimension(dimensionNames[i], range));
    }
    return dimensions;
}

vector<Attribute*> Dictionary::parseAttributes(string& file, string& variableName) {
    string query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute/name/child::text()";
    const vector<string> attributeNames = xmlParser.evaluateToStringList(file, query);
    vector<Attribute*> attributes;
    for (size_t i = 0; i < attributeNames.size(); i++) {
        string attributeName = attributeNames[i];
        query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute[name=\"" + attributeName + "\"]/type";
        string type = xmlParser.evaluateToString(file, query);
        query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute[name=\"" + attributeName + "\"]/value";
        string value = xmlParser.evaluateToString(file, query);
        attributes.push_back(new Attribute(type, attributeName, value));
    }
    return attributes;
}

set<Variable*> Dictionary::getVariablesToBeWritten() const {
    // TODO - implement
    return variables;
}

Variable& Dictionary::getVariable(const string& ncName) {
    set<Variable*>::iterator iter;
    for (iter = variables.begin(); iter != variables.end(); iter++) {
        if ((*iter)->getNcName().compare(ncName) == 0) {
            return **iter;
        }
    }
    throw std::invalid_argument("No variable with id " + ncName);
}

const string Dictionary::getNcFileName(const string& ncName) const {
    set<Variable*>::iterator iter;
    for (iter = variables.begin(); iter != variables.end(); iter++) {
        if ((*iter)->getNcName().compare(ncName) == 0) {
            return (*iter)->getFileName();
        }
    }
    throw std::invalid_argument("No filename for variable " + ncName);
}

NcType Dictionary::mapToNcType(const string& type) {

    // see S3-L2-SD-08-G-ARG-IODD, page 41

    if (type.compare("sc") == 0 ) {
        return ncByte; // signed char (8-bit signed)
    } else if (type.compare("ss") == 0 || type.compare("uc") == 0) {
        return ncShort; // 16-bit signed
    } else if (type.compare("us") == 0 || type.compare("sl") == 0) {
        return ncInt; // 32-bit signed
    } else if (type.compare("fl") == 0) {
        return ncFloat;
    } else if (type.compare("db") == 0 || type.compare("sll") == 0 || type.compare("ul") == 0) {
        return ncDouble;
    }

    // fallback
    return ncDouble;
}