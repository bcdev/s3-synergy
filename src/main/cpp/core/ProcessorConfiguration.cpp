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
#include <boost/lexical_cast.hpp>

#include "ProcessorConfiguration.h"
#include "../util/Logger.h"

using std::cout;

/*
 * implementation of ProcessorConfiguration
 */

ProcessorConfiguration::ProcessorConfiguration(string taskName,
        string taskVersion, vector<BreakpointFile*> breakpointFiles,
        vector<Input*> inputList, vector<Output*> outputList) {
    this->taskVersion = taskVersion;
    this->taskName = taskName;
    this->breakpointFiles = breakpointFiles;
    this->inputList = inputList;
    this->outputList = outputList;
}

ProcessorConfiguration::~ProcessorConfiguration() {
}

vector<Output*> ProcessorConfiguration::getOutputList() const {
    return outputList;
}

vector<Input*> ProcessorConfiguration::getInputList() const {
    return inputList;
}

vector<BreakpointFile*> ProcessorConfiguration::getBreakpointFiles() const {
    return breakpointFiles;
}

string ProcessorConfiguration::getTaskVersion() const {
    return taskVersion;
}

string ProcessorConfiguration::getTaskName() const {
    return taskName;
}

// TODO - replace by toString()

void ProcessorConfiguration::log() {
    Logger* logger = Logger::get();
    logger->debug("taskName = " + taskName, "JobOrder");
    logger->debug("taskVersion = " + taskVersion, "JobOrder");
    for (size_t i = 0; i < breakpointFiles.size(); i++) {
        logger->debug("parsing breakpointFile " + boost::lexical_cast<string > (i + 1) + ": ", "JobOrder");
        breakpointFiles.at(i)->log();
    }
    for (size_t i = 0; i < inputList.size(); i++) {
        logger->debug("parsing input " + boost::lexical_cast<string > (i + 1) + ": ", "JobOrder");
        inputList.at(i)->log();
    }
    for (size_t i = 0; i < outputList.size(); i++) {
        logger->debug("parsing output " + boost::lexical_cast<string>(i + 1) + ": ", "JobOrder");
        outputList.at(i)->log();
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

void BreakpointFile::log() const {
    Logger* logger = Logger::get();
    logger->debug("enable = " + enable, "JobOrder");
    logger->debug("fileType = " + fileType, "JobOrder");
    logger->debug("fileNameType = " + fileNameType, "JobOrder");
    logger->debug("fileName = " + fileName, "JobOrder");
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

Input::Input(string fileType, string fileNameType, vector<string> fileNames, vector<TimeInterval*> timeIntervals) {
    this->fileType = fileType;
    this->fileNameType = fileNameType;
    this->fileNames = fileNames;
    this->timeIntervals = timeIntervals;
}

void Input::log() const {
    Logger* logger = Logger::get();
    logger->debug("fileType = " + fileType, "JobOrder");
    logger->debug("fileNameType = " + fileNameType, "JobOrder");
    for (size_t i = 0; i < fileNames.size(); i++) {
        logger->debug("fileName " + boost::lexical_cast<string>(i + 1) + ": " + fileNames.at(i), "JobOrder");
    }
    for (size_t i = 0; i < timeIntervals.size(); i++) {
        timeIntervals.at(i)->log();
    }
}

vector<TimeInterval*> Input::getTimeIntervals() const {
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

void Output::log() const {
    Logger* logger = Logger::get();
    logger->debug("fileType = " + fileType, "JobOrder");
    logger->debug("fileNameType = " + fileNameType, "JobOrder");
    logger->debug("fileName = " + fileName, "JobOrder");
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

void TimeInterval::log() const {
    Logger* logger = Logger::get();
    logger->debug("start = " + start, "JobOrder");
    logger->debug("stop = " + stop, "JobOrder");
}