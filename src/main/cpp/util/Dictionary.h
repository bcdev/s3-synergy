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

#include <set>
#include <vector>

#include "Variable.h"
#include "XmlParser.h"

using std::set;
using std::vector;

/**
 * A dictionary serves as source for static information about variables, i.e. 
 * the name, type, dimensions, and attributes of variables both to be written
 * and read.
 *
 * @param configFile The file comprising the path to the output variable
 * definitions.
 */
class Dictionary {
public:
    Dictionary(string configFile);
    virtual ~Dictionary();

    /**
     * Parses the input files and thus initialises the dictionary.
     */
    void parse();

    /**
     * Returns all variables.
     * @return All variables.
     */
    vector<Variable*> getVariables() const;

    /**
     * Returns a variable for a given symbolic name. To be used by modules in
     * order to get needed attributes for the variable.
     * @param varId The symbolic name of the variable to return.
     * @return The variable with the given symbolic name.
     */
    Variable& getVariable(const string& symbolicName);

    /**
     * Returns the name of the given variable as it appears in the source
     * netcdf-file. To be used by the reader in order to read data from the
     * variable from a file and to store the data in structures addressed by
     * the (unique) symbolic name.
     * @param symbolicName The symbolic name to get the netcdf-name for.
     * @return The netcdf-name.
     */
    string getNcVarName(const string& symbolicName);

    /**
     * Returns the name of the netCDF-file for a given (output) variable name.
     * Important note: this method is only to be used for output variables,
     * since the input variable file names are ambiguous!
     * @param ncName The output (netcdf-) variable name to get the name of the
     * netCDF-file for.
     * @return The netCDF-filename.
     */
    const string getNcFileName(const string& ncName) const;

    /**
     * Returns the name of the netCDF-file for a given symbolic variable name.
     * @param ncName The symbolic variable name to get the name of the
     * netCDF-file for.
     * @return The netCDF-filename.
     */
    const string getNcFileNameForSymbolicName(const string& symbolicName) const;

private:
    string filePath;
    void parseVariablesFile(string& variableDefPath, string& file);
    vector<Attribute*> parseAttributes(string& file, string& variableName);

    XmlParser xmlParser;
    string configFile;
    vector<Variable*> variables;
};

#endif	/* DICTIONARY_H */

