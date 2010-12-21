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

#include "Module.h"
#include "Variable.h"
#include "XmlParser.h"

using std::set;

class Dictionary : XmlParser {
public:
    Dictionary(string configFile);
    virtual ~Dictionary();
    void parseInputFiles();
    set<Variable*> getVariablesToBeWritten() const;
    set<Variable*> getVariablesToBeComputed(Module& module) const;
    set<Variable*> getNeededVariables(Module& module) const;
protected:
    string path;
private:
    string configFile;
    set<Variable*> variables;
};

#endif	/* DICTIONARY_H */

