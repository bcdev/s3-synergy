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

#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <stdexcept>

#include "Dictionary.h"
#include "VariableImpl.h"
#include "IOUtils.h"

using std::ifstream;
using boost::algorithm::trim_copy;

Dictionary::Dictionary(string config) : configFile(config) {
    parse();
}

Dictionary::~Dictionary() {
    for (size_t i = 0; i < variables.size(); i++) {
        delete variables[i];
    }
}

void Dictionary::parse() {
    string variableDefPath = xmlParser.evaluateToString(configFile, "/Config/Variable_Definition_Files_Path");
    string L1CPath = variableDefPath + "/L1C";
    string L2SynPath = variableDefPath + "/L2_SYN";

    vector<string> L1CFiles = IOUtils::getFiles(L1CPath);
    for (size_t i = 0; i < L1CFiles.size(); i++) {
        parseVariablesFile(L1CPath, L1CFiles[i]);
    }
    vector<string> L2SynFiles = IOUtils::getFiles(L2SynPath);
    for (size_t i = 0; i < L2SynFiles.size(); i++) {
        parseVariablesFile(L2SynPath, L2SynFiles[i]);
    }

    string variablesToBeReadFile = xmlParser.evaluateToString(configFile, "/Config/Variables_To_Be_Read_File");
    ifstream stream(variablesToBeReadFile);
    string temp;
    while (getline(stream, temp)) {
        temp = trim_copy(temp);
        variablesToBeRead.push_back(temp);
    }

}

const vector<string> Dictionary::getVariablesToBeRead() const {
    return variablesToBeRead;
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

const string Dictionary::getNcFileNameForSymbolicName(const string& symbolicName) const {
    for (size_t i = 0; i < variables.size(); i++) {
        if (variables[i]->getSymbolicName().compare(symbolicName) == 0) {
            return variables[i]->getFileName();
        }
    }
    throw std::invalid_argument("No filename for variable " + symbolicName + ".");
}

string Dictionary::getNcVarName(const string& symbolicName) {
    return getVariable(symbolicName).getNcName();
}

const string Dictionary::getSegmentName(const string& symbolicName) {
    return getVariable(symbolicName).getSegmentName();
};

void Dictionary::parseVariablesFile(string& variableDefPath, string& file) {
    string path = variableDefPath + "/" + file;
    const vector<string> variableNames = xmlParser.evaluateToStringList(path, "/dataset/variables/variable/name/child::text()");

    for (size_t i = 0; i < variableNames.size(); i++) {
        string variableName = variableNames[i];
        string query = "/dataset/variables/variable[name=\"" + variableName + "\"]/symbolic_name";
        string symbolicName = xmlParser.evaluateToString(path, query);
        if (symbolicName.empty()) {
            symbolicName = variableName;
        }
        Variable* var = new VariableImpl(variableName, symbolicName);
        var->setFileName(xmlParser.evaluateToString(path, "/dataset/global_attributes/attribute[name=\"dataset_name\"]/value"));
        query = "/dataset/variables/variable[name=\"" + variableName + "\"]/segment_name";
        var->setSegmentName(xmlParser.evaluateToString(path, query));
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