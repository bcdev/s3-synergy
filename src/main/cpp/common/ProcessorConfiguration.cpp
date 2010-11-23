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
 * File:   ProcessorConfiguration.cpp
 * Author: thomass
 * 
 * Created on November 23, 2010, 1:44 PM
 */

#include <iostream>

#include "ProcessorConfiguration.h"

using std::cout;

/*
 * implementation of ProcessorConfiguration
 */

ProcessorConfiguration::ProcessorConfiguration(string taskName,
        string taskVersion, vector<BreakpointFile> breakpointFiles,
        vector<Input> inputList, vector<Output> outputList) {
    this->taskVersion = taskVersion;
    this->taskName = taskName;
    this->breakpointFiles = breakpointFiles;
    this->inputList = inputList;
    this->outputList = outputList;
}

ProcessorConfiguration::~ProcessorConfiguration() {
}

vector<Output> ProcessorConfiguration::getOutputList() const {
    return outputList;
}

vector<Input> ProcessorConfiguration::getInputList() const {
    return inputList;
}

vector<BreakpointFile> ProcessorConfiguration::getBreakpointFiles() const {
    return breakpointFiles;
}

string ProcessorConfiguration::getTaskVersion() const {
    return taskVersion;
}

string ProcessorConfiguration::getTaskName() const {
    return taskName;
}

void ProcessorConfiguration::print() {
    cout << "taskName = " << taskName << "\n";
    cout << "taskVersion = " << taskVersion << "\n";
    for (size_t i = 0; i < breakpointFiles.size(); i++) {
        cout << "breakpointFile " << i + 1 << ":\n";
        breakpointFiles.at(i).print();
    }
    for (size_t i = 0; i < inputList.size(); i++) {
        cout << "input " << i + 1 << ":\n";
        inputList.at(i).print();
    }
    for (size_t i = 0; i < outputList.size(); i++) {
        cout << "output " << i + 1 << ":\n";
        outputList.at(i).print();
    }
}

/*
 * implementation of BreakpointFile
 */

BreakpointFile::BreakpointFile(string enable, string fileType, string fileNameType, string fileName) {
    this->enable = enable;
    this->fileType = fileType;
    this->fileNameType = fileNameType;
    this->fileName = fileName;
}

BreakpointFile::~BreakpointFile() {
}

void BreakpointFile::print() const {
    cout << "enable = " << enable << "\n";
    cout << "fileType = " << fileType << "\n";
    cout << "fileNameType = " << fileNameType << "\n";
    cout << "fileName = " << fileName << "\n";
}

string BreakpointFile::getFileName() const {
    return fileName;
}

string BreakpointFile::getFileNameType() const {
    return fileNameType;
}

string BreakpointFile::getFileType() const {
    return fileType;
}

string BreakpointFile::getEnable() const {
    return enable;
}

/*
 * implementation of Input
 */

Input::Input(string fileType, string fileNameType, vector<string> fileNames, vector<TimeInterval> timeIntervals) {
    this->fileType = fileType;
    this->fileNameType = fileNameType;
    this->fileNames = fileNames;
    this->timeIntervals = timeIntervals;
}

void Input::print() const {
    cout << "fileType = " << fileType << "\n";
    cout << "fileNameType = " << fileNameType << "\n";
    for (size_t i = 0; i < fileNames.size(); i++) {
        cout << "fileName " << i + 1 << ": " << fileNames.at(i) << "\n";
    }
    for (size_t i = 0; i < timeIntervals.size(); i++) {
        timeIntervals.at(i).print();
    }
}

vector<TimeInterval> Input::getTimeIntervals() const {
    return timeIntervals;
}

vector<string> Input::getFileNames() const {
    return fileNames;
}

string Input::getFileNameType() const {
    return fileNameType;
}

string Input::getFileType() const {
    return fileType;
}

/*
 * implementation of Output
 */

Output::Output(string fileType, string fileNameType, string fileName) {
    this->fileType = fileType;
    this->fileNameType = fileNameType;
    this->fileName = fileName;
}
string Output::getFileName() const {
    return fileName;
}
string Output::getFileNameType() const {
    return fileNameType;
}
string Output::getFileType() const {
    return fileType;
}

void Output::print() const {
    cout << "fileType = " << fileType << "\n";
    cout << "fileNameType = " << fileNameType << "\n";
    cout << "fileName = " << fileName << "\n";
}

/*
 * implementation of TimeInterval
 */

TimeInterval::TimeInterval(string start, string stop) {
    this->start = start;
    this->stop = stop;
}

string TimeInterval::getStop() const {
    return stop;
}

string TimeInterval::getStart() const {
    return start;
}

void TimeInterval::print() const {
    cout << "start = " << start << "\n";
    cout << "stop = " << stop << "\n";
}