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

#include "Dictionary.h"

using namespace boost::filesystem;

Dictionary::Dictionary(string config) : configFile(config) {
}

Dictionary::~Dictionary() {
}

void Dictionary::parseInputFiles() {
    string query = "/Config/Variable_Definition_Files_Path";
    string variableDefPath = delegate.evaluateToString(configFile, query);

    vector<string> childFolders = getChildFolders(variableDefPath);
    query = "/variable/name";
    for (size_t i = 0; i < childFolders.size(); i++) {
        string path = variableDefPath + "/" + (childFolders[i]);
        string value = delegate.evaluateToString(path, query);
        std::cout << value << "\n";
    }

}

vector<string> Dictionary::getChildFolders(string& path) {
    vector<string> files;
    if (is_directory(path)) {
        for (directory_iterator iter(path); iter != directory_iterator(); ++iter) {
            string fileName = iter->path().filename();
            if (!is_directory(fileName)) {
//                files.push_back(fileName);
            }
        }
    }

    return files;
}

set<Variable*> Dictionary::getVariablesToBeWritten() const {
    // TODO - implement
    return *(new set<Variable*>());
}

Variable& Dictionary::getVariable(const string& varId) {
    map<string*, Variable*>::iterator iter;
    for (iter = variables.begin(); iter != variables.end(); iter++) {
        if (iter->second->getId() == varId) {
            return *(iter->second);
        }
    }
    throw std::invalid_argument("No variable with id " + varId);
}

Variable& Dictionary::getVariableForNcVarName(const string& ncVarName) {

}

const string& Dictionary::getNcVarName(const string& varId) const {

}