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
#include "../util/IOUtils.h"

using std::ifstream;
using boost::algorithm::trim_copy;
using boost::starts_with;

Dictionary::Dictionary() {
}

Dictionary::Dictionary(string config) : configFile(config) {
    init();
}

Dictionary::~Dictionary() {
    for (size_t i = 0; i < l1cVariables.size(); i++) {
        delete l1cVariables[i];
    }
    for (size_t i = 0; i < l2Variables.size(); i++) {
        delete l2Variables[i];
    }
}

void Dictionary::init() {
    string variableDefPath = xmlParser.evaluateToString(configFile, "/Config/Variable_Definition_Files_Path");
    string L1CPath = variableDefPath + "/L1C";
    string L2SynPath = variableDefPath + "/L2_SYN";

    vector<string> L1CFiles = IOUtils::getFiles(L1CPath);
    for (size_t i = 0; i < L1CFiles.size(); i++) {
        parseVariablesFile(L1CPath, L1CFiles[i], true);
    }
    vector<string> L2SynFiles = IOUtils::getFiles(L2SynPath);
    for (size_t i = 0; i < L2SynFiles.size(); i++) {
        parseVariablesFile(L2SynPath, L2SynFiles[i], false);
    }
}

bool Dictionary::hasVariable(const string& sectionId, const string& symbolicVarName) {
    if (sectionId.compare("SYL2") == 0) {
        for (size_t i = 0; i < l2Variables.size(); i++) {
            if (l2Variables[i]->getSymbolicName().compare(symbolicVarName) == 0) {
                return true;
            }
        }
    }
    return false;
}

Variable& Dictionary::addVariable(const string& sectionId, const string& symbolicVarName, const string& ncVarName) {
    if (sectionId.compare("SYL2") == 0 && !hasVariable(sectionId, symbolicVarName)) {
        Variable* variable = new VariableImpl(ncVarName, symbolicVarName);
        l2Variables.push_back(variable);
        return *variable;
    }
    throw std::invalid_argument("Invalid section ID: " + sectionId);
}

vector<string> Dictionary::getVariables(bool l1c) const {
    vector<string> result;
    if (l1c) {
        for (size_t i = 0; i < l1cVariables.size(); i++) {
            result.push_back(l1cVariables[i]->getSymbolicName());
        }
    } else {
        for (size_t i = 0; i < l2Variables.size(); i++) {
            result.push_back(l2Variables[i]->getSymbolicName());
        }
    }
    return result;
}

Variable& Dictionary::getL1cVariable(const string& symbolicName) {
    for (size_t i = 0; i < l1cVariables.size(); i++) {
        if (l1cVariables[i]->getSymbolicName().compare(symbolicName) == 0) {
            return *(l1cVariables[i]);
        }
    }
    throw std::invalid_argument("No L1C-variable with symbolic name " + symbolicName + ".");
}

Variable& Dictionary::getL2Variable(const string& symbolicName) {
    for (size_t i = 0; i < l2Variables.size(); i++) {
        if (l2Variables[i]->getSymbolicName().compare(symbolicName) == 0) {
            return *(l2Variables[i]);
        }
    }
    throw std::invalid_argument("No L2-variable with symbolic name " + symbolicName + ".");
}

const string Dictionary::getL2NcFileName(const string& ncName) const {
    for (size_t i = 0; i < l2Variables.size(); i++) {
        if (l2Variables[i]->getNcName().compare(ncName) == 0) {
            return l2Variables[i]->getFileName();
        }
    }
    throw std::invalid_argument("No filename for variable " + ncName + ".");
}

const string Dictionary::getL1cNcFileNameForSymbolicName(const string& symbolicName) const {
    for (size_t i = 0; i < l1cVariables.size(); i++) {
        if (l1cVariables[i]->getSymbolicName().compare(symbolicName) == 0) {
            return l1cVariables[i]->getFileName();
        }
    }
    throw std::invalid_argument("No filename for variable " + symbolicName + ".");
}

const string Dictionary::getL2NcFileNameForSymbolicName(const string& symbolicName) const {
    for (size_t i = 0; i < l2Variables.size(); i++) {
        if (l2Variables[i]->getSymbolicName().compare(symbolicName) == 0) {
            return l2Variables[i]->getFileName();
        }
    }
    throw std::invalid_argument("No filename for variable " + symbolicName + ".");
}

string Dictionary::getL1cNcVarName(const string& symbolicName) {
    return getL1cVariable(symbolicName).getNcName();
}

const string Dictionary::getSegmentNameForL1c(const string& symbolicName) {
    return getL1cVariable(symbolicName).getSegmentName();
}

const string Dictionary::getSegmentNameForL2(const string& symbolicName) {
    return getL2Variable(symbolicName).getSegmentName();
}

void Dictionary::parseVariablesFile(string& variableDefPath, string& file, bool l1c) {
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
        if (l1c) {
            l1cVariables.push_back(var);
        } else {
            l2Variables.push_back(var);
        }
    }
}

vector<Attribute*> Dictionary::parseAttributes(string& file, string& variableName) {
    string query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute/name/child::text()";
    const vector<string> attributeNames = xmlParser.evaluateToStringList(file, query);
    vector<Attribute*> attributes;
    for (size_t i = 0; i < attributeNames.size(); i++) {
        string attributeName = attributeNames[i];
        query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute[name=\"" + attributeName + "\"]/type";
        int type = mapToNcType(xmlParser.evaluateToString(file, query));
        query = "/dataset/variables/variable[name=\"" + variableName + "\"]/attributes/attribute[name=\"" + attributeName + "\"]/value";
        string value = xmlParser.evaluateToString(file, query);

        attributes.push_back(new Attribute(type, attributeName, value));
    }
    return attributes;
}

int Dictionary::mapToNcType(const string& typeString) {
    // TODO - implement
    return 0;
}