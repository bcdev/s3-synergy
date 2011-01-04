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
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <stdexcept>
#include <boost/algorithm/string/predicate.hpp>

#include "Dictionary.h"
#include "VariableImpl.h"

using boost::filesystem::is_directory;
using boost::filesystem::directory_iterator;
using boost::filesystem::path;

Dictionary::Dictionary(string config) : configFile(config) {
//    parse();
}

Dictionary::~Dictionary() {
    for( size_t i = 0; i < variables.size(); i++ ) {
        delete variables[i];
    }
}

void Dictionary::parse() {
    string variableDefPath = xmlParser.evaluateToString(configFile, "/Config/Variable_Definition_Files_Path");
    string L1CPath = variableDefPath + "/L1C";
    string L2SynPath = variableDefPath + "/L2_SYN";

    vector<string> L1CFiles = getFiles(L1CPath);
    for (size_t i = 0; i < L1CFiles.size(); i++) {
        parseVariablesFile(L1CPath, L1CFiles[i]);
    }
    vector<string> L2SynFiles = getFiles(L2SynPath);
    for (size_t i = 0; i < L2SynFiles.size(); i++) {
        parseVariablesFile(L2SynPath, L2SynFiles[i]);
    }

}

vector<Variable*> Dictionary::getVariables() const {
    return variables;
}

Variable& Dictionary::getVariable(const string& symbolicName) {
    for (size_t i = 0; i < variables.size(); i++) {
        if (variables[i]->getSymbolicName().compare(symbolicName) == 0) {
            return *(variables[i]);
        }
    }
    throw std::invalid_argument("No variable with symbolic name " + symbolicName + ".");
}

const string Dictionary::getNcFileName(const string& ncName) const {
    for (size_t i = 0; i < variables.size(); i++) {
        if (variables[i]->getNcName().compare(ncName) == 0) {
            return variables[i]->getFileName();
        }
    }
    throw std::invalid_argument("No filename for variable " + ncName + ".");
}

string Dictionary::getNcVarName(const string& symbolicName) {
    return getVariable(symbolicName).getNcName();
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
        string query = "/dataset/variables/variable[name=\"" + variableName + "\"]/symbolic_name";
        string symbolicName = xmlParser.evaluateToString(path, query);
        if( symbolicName.empty() ) {
            symbolicName = variableName;
        }
        Variable* var = new VariableImpl(variableName, symbolicName);
        var->setFileName(xmlParser.evaluateToString(path, "/dataset/global_attributes/attribute[name=\"dataset_name\"]/value"));
        vector<Attribute*> attributes = parseAttributes(path, variableName);
        for (size_t k = 0; k < attributes.size(); k++) {
            var->addAttribute(attributes[k]);
        }
        variables.push_back(var);
    }
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

NcType Dictionary::mapToNcType(const string& type) {

    // see S3-L2-SD-08-G-ARG-IODD, page 41

    if (type.compare("sc") == 0) {
        return ncByte; // signed char (8-bit signed)
    } else if (type.compare("ss") == 0 || type.compare("uc") == 0 || type.compare("us") == 0) {
        return ncShort; // 16-bit signed
    } else if (type.compare("sl") == 0) {
        return ncInt; // 32-bit signed
    } else if (type.compare("fl") == 0) {
        return ncFloat; // 32-bit floating point
    } else if (type.compare("db") == 0 || type.compare("sll") == 0 || type.compare("ul") == 0) {
        return ncDouble; // 64-bit floating point
    }

    // fallback
    return ncDouble;
}