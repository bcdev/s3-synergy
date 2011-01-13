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
#include "../util/XmlParser.h"

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
    Dictionary();
    Dictionary(string configFile);
    virtual ~Dictionary();

    /**
     * Returns the list of symbolic names of the variables, which are to be read
     * from the input L1C-product. To be used by the reader.
     * @return The list of variables to be read.
     */
    vector<string> getVariables(bool l1c) const;

    /**
     * Returns a L1C-variable for a given symbolic name.
     * @param varId The symbolic name of the variable to return.
     * @return The variable with the given symbolic name.
     */
    Variable& getL1cVariable(const string& symbolicName);

    /**
     * Returns a L2-variable for a given symbolic name. To be used by modules in
     * order to get needed attributes for the variable.
     * @param varId The symbolic name of the variable to return.
     * @return The variable with the given symbolic name.
     */
    Variable& getL2Variable(const string& symbolicName);

    /**
     * Returns the name of the given L1C-variable as it appears in the source
     * netcdf-file. To be used by the reader in order to read data from the
     * variable from a file and to store the data in structures addressed by
     * the (unique) symbolic name.
     * @param symbolicName The symbolic name to get the netcdf-name for.
     * @return The netcdf-name.
     */
    string getL1cNcVarName(const string& symbolicName);

    /**
     * Returns the name of the netCDF-file for a given L2 variable name.
     * Important note: this method is only to be used for output variables,
     * since the input variable file names are ambiguous! In search for the
     * file name of an input variable, use getL1CNcFileNameForSymbolicName().
     * @param ncName The output L2-variable name to get the name of the
     * netCDF-file for.
     * @return The netCDF-filename.
     */
    const string getL2NcFileName(const string& ncName) const;

    /**
     * Returns the name of the netCDF-file for a given symbolic L1C-variable name.
     * @param ncName The symbolic variable name to get the name of the
     * netCDF-file for.
     * @return The netCDF-filename.
     */
    const string getL1cNcFileNameForSymbolicName(const string& symbolicName) const;

    /**
     * Returns the name of the netCDF-file for a given symbolic L2-variable name.
     * @param ncName The symbolic variable name to get the name of the
     * netCDF-file for.
     * @return The netCDF-filename.
     */
    const string getL2NcFileNameForSymbolicName(const string& symbolicName) const;

    /**
     * Returns the name of the segment the (L1c-)variable uses during the
     * processing.
     * @return The segment's name.
     */
    const string getSegmentNameForL1c(const string& symbolicName);

    /**
     * Returns the name of the segment the (L2-)variable uses during the
     * processing.
     * @return The segment's name.
     */
    const string getSegmentNameForL2(const string& symbolicName);

    Variable& addVariable(const string& sectionId, const string& symbolicVarName, const string& ncVarName);

    bool hasVariable(const string& sectionId, const string& symbolicVarName);

private:
    void init();
    string filePath;
    void parseVariablesFile(string& variableDefPath, string& file, bool l1c);
    vector<Attribute*> parseAttributes(string& file, string& variableName);
    int mapToNcType(const string& typeString);

    XmlParser xmlParser;
    const string configFile;
    vector<Variable*> l1cVariables;
    vector<Variable*> l2Variables;
};

#endif	/* DICTIONARY_H */

