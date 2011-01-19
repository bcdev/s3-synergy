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
 * File:   JobOrderParser.h
 * Author: thomass
 *
 * Created on November 15, 2010, 4:31 PM
 */

#ifndef JOBORDERPARSER_H
#define	JOBORDERPARSER_H

#include "../core/Configuration.h"
#include "../core/JobOrder.h"
#include "../core/ProcessorConfiguration.h"
#include "XmlParser.h"

using std::string;

class JobOrderParser : public XmlParser {
public:
    JobOrderParser(string path);
    virtual ~JobOrderParser();
    JobOrder* parseJobOrder();
protected:
    string path;
private:
    Configuration parseConfiguration();
    vector<ProcessorConfiguration*> parseProcessorConfigurations();
    ProcessorConfiguration* parseProcessorConfiguration(int index);
    void parseId(Configuration* config, DOMElement* node);
    void parseDebugLevel(Configuration* config, DOMElement* node);
    void parseInputLocation(DOMElement* childNode, Configuration* config);
    bool stringToBool(string in);
    string getStringFromNode(const XObject* result);
    vector<BreakpointFile*> parseBreakpointFiles(string baseQuery);
    BreakpointFile* parseBreakpointFile(string baseQuery);
    vector<Input*> parseInputs(string baseQuery);
    Input* parseInput(string baseQuery);
    vector<Output*> parseOutputs(string baseQuery);
    Output* parseOutput(string baseQuery);
    string intToString(int toConvert);
};

#endif	/* JOBORDERPARSER_H */

