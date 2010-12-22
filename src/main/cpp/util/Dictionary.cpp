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


//#include <boost/filesystem/operations.hpp>
//#include <boost/filesystem/fstream.hpp>

#include "Dictionary.h"

//using namespace boost::filesystem;

Dictionary::Dictionary(string config) : configFile(config) {
}

Dictionary::~Dictionary() {
}

void Dictionary::parseInputFiles() {
    string query = "/Config/Variable_Definition_Files_Path";
    string variableDefPath = evaluateToString(configFile, query);

//    if (exists(variableDefPath)) {
//        directory_iterator end;
//        for (directory_iterator iter(variableDefPath); iter != end; ++iter) {
//            iter.basic_directory_iterator()->
//            if (is_directory(*iter)) {
//                std::cout << iter->native_directory_string() << " (directory)\n";
//            } else {
//                std::cout << iter->native_file_string() << " (file)\n";
//            }
//        }
//    }

    //    for all input files do:

    //    query = "/variables/name";
    //    string path = "/variables/name";
    //    string value = evaluateToString(path, query);
}

set<Variable*> Dictionary::getVariablesToBeWritten() const {
    // TODO - implement
    return variables;
}

set<Variable*> Dictionary::getNeededVariables(Module& module) const {
    // TODO - implement
    return variables;
}

set<Variable*> Dictionary::getVariablesToBeComputed(Module& module) const {
    // TODO - implement
    return variables;
}