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
 * File:   Dictionary.h
 * Author: thomass
 *
 * Created on December 21, 2010, 1:55 PM
 */

#ifndef DICTIONARY_H
#define	DICTIONARY_H

#include <boost/filesystem.hpp>
#include <map>
#include <set>
#include <vector>

#include "Module.h"
#include "Variable.h"
#include "XmlParser.h"

using std::map;
using std::set;
using std::vector;

class Dictionary {
public:
    Dictionary(string configFile);
    virtual ~Dictionary();

    /**
     * Parses the input files and thus initialises the dictionary.
     */
    void parseInputFiles();

    /**
     * Returns the subset of variables, which are to be written.
     * @return
     */
    set<Variable*> getVariablesToBeWritten() const;

    /**
     * Returns a variable for a given symbolic name. To be used by modules in
     * order to get needed attributes for the variable.
     * @param varId The symbolic name of the variable to return.
     * @return The variable with the given symbolic name.
     */
    Variable& getVariable(const string& varId);

    /**
     * Returns a variable for a given netCDF-name. To be used by the reader in
     * order to fill the variable with attributes, dimensions, type and symbolic
     * name.
     * @param ncVarName The netCDF-name of the variable to return.
     * @return The variable with the given netCDF-name.
     */
    Variable& getVariableForNcVarName(const string& ncVarName);

    /**
     * Returns the netCDF-name for a given symbolic name. To be used by the
     * writer.
     * @param varId The symbolic name to get the netCDF-name for.
     * @return The netCDF-name.
     */
    const string& getNcVarName(const string& varId) const;

protected:
    string filePath;
private:
    vector<string> getChildFolders(string& p);

    XmlParser delegate;
    string configFile;
    map<string*, Variable*> variables;
};

#endif	/* DICTIONARY_H */

