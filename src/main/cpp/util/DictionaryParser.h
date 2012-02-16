/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

#ifndef DICTIONARYPARSER_H
#define	DICTIONARYPARSER_H

#include <set>

#include "../core/Dictionary.h"

#include "XmlParser.h"

using std::set;

class DictionaryParser {
public:
    DictionaryParser();
    virtual ~DictionaryParser();

    shared_ptr<Dictionary> parse(const string& path);

private:
    DictionaryParser(const DictionaryParser& orig);

    bool contains(const set<string>& list, const string& key) {
        return list.find(key) != list.end();
    }

    void parseGlobalAttributes(const string& dirPath, ProductDescriptor& productDescriptor) const;
    void parseVariables(const string& variableDefPath, const string& file, ProductDescriptor& prod) const;
    void parseAttributes(const string& file, const string& variableName, VariableDescriptor& var) const;
    void parseDimensions(const string& file, const string& variableName, VariableDescriptor& var) const;
    int toType(const string& typeString) const;

    XmlParser xmlParser;
    set<string> exclusionSet;
};

#endif	/* DICTIONARYPARSER_H */

