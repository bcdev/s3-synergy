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
 * File:   ProcessorConfiguration.h
 * Author: thomass
 *
 * Created on November 23, 2010, 1:44 PM
 */

#ifndef PROCESSORCONFIGURATION_H
#define	PROCESSORCONFIGURATION_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class BreakpointFile;
class Input;
class Output;
class TimeInterval;

class ProcessorConfiguration {
public:
    ProcessorConfiguration(string taskName, string taskVersion,
            vector<BreakpointFile> breakpointFiles,
            vector<Input> inputList, vector<Output> outputList);
    virtual ~ProcessorConfiguration();
    string getTaskName() const;
    string getTaskVersion() const;
    vector<BreakpointFile> getBreakpointFiles() const;
    vector<Input> getInputList() const;
    vector<Output> getOutputList() const;
    void print();
private:
    string taskName;
    string taskVersion;
    vector<BreakpointFile> breakpointFiles;
    vector<Input> inputList;
    vector<Output> outputList;
};

class BreakpointFile {
public:
    BreakpointFile(string enable, string fileType, string fileNameType, string fileName);
    ~BreakpointFile();
    string getFileName() const;
    string getFileNameType() const;
    string getFileType() const;
    string getEnable() const;
    void print() const;
private:
    string enable;
    string fileType;
    string fileNameType;
    string fileName;
};

class Input {
public:
    Input(string fileType, string fileNameType, vector<string> fileNames, vector<TimeInterval> timeIntervals);
    vector<TimeInterval> getTimeIntervals() const;
    vector<string> getFileNames() const;
    string getFileNameType() const;
    string getFileType() const;
    void print() const;
private:
    string fileType;
    string fileNameType;
    vector<string> fileNames;
    vector<TimeInterval> timeIntervals;
};

class Output {
public:
    Output(string fileType, string fileNameType, string fileName);
    void print() const;
    string getFileName() const;
    string getFileNameType() const;
    string getFileType() const;
private:
    string fileType;
    string fileNameType;
    string fileName;
};

class TimeInterval {
private:
    string start;
    string stop;
public:
    void print() const;
    TimeInterval(string start, string stop);
    string getStop() const;
    string getStart() const;
};

#endif	/* PROCESSORCONFIGURATION_H */

