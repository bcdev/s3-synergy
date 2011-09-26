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
 * File:   IpfProcessor.cpp
 * Author: thomass
 * 
 * Created on November 23, 2010, 1:44 PM
 */

#include <iostream>

#include "Boost.h"
#include "IpfProcessor.h"
#include "../logging/Logging.h"

using std::cout;

IpfProcessor::IpfProcessor(const string& taskName,
		const string& taskVersion,
		const vector<BreakpointFile>& breakpointFiles,
		const vector<Input>& inputList, const vector<Output>& outputList) {
	this->taskVersion = taskVersion;
	this->taskName = taskName;
	this->breakpointFiles = breakpointFiles;
	this->inputList = inputList;
	this->outputList = outputList;
}

IpfProcessor::~IpfProcessor() {
}

const vector<Output>& IpfProcessor::getOutputList() const {
	return outputList;
}

const vector<Input>& IpfProcessor::getInputList() const {
	return inputList;
}

const vector<BreakpointFile>& IpfProcessor::getBreakpointFiles() const {
	return breakpointFiles;
}

string IpfProcessor::getTaskVersion() const {
	return taskVersion;
}

string IpfProcessor::getTaskName() const {
	return taskName;
}

void IpfProcessor::log(Logging& logging) const {
	logging.debug("taskName = " + taskName, "JobOrder");
	logging.debug("taskVersion = " + taskVersion, "JobOrder");
	for (size_t i = 0; i < breakpointFiles.size(); i++) {
		logging.debug(
				"parsing breakpointFile " + boost::lexical_cast<string>(i + 1)
						+ ": ", "JobOrder");
		breakpointFiles[i].log(logging);
	}
	for (size_t i = 0; i < inputList.size(); i++) {
		logging.debug(
				"parsing input " + boost::lexical_cast<string>(i + 1) + ": ",
				"JobOrder");
		inputList[i].log(logging);
	}
	for (size_t i = 0; i < outputList.size(); i++) {
		logging.debug(
				"parsing output " + boost::lexical_cast<string>(i + 1) + ": ",
				"JobOrder");
		outputList[i].log(logging);
	}
}

BreakpointFile::BreakpointFile(const string& enable, const string& fileType,
		const string& fileNameType, const string& fileName) {
	this->enable = enable;
	this->fileType = fileType;
	this->fileNameType = fileNameType;
	this->fileName = fileName;
}

BreakpointFile::~BreakpointFile() {
}

void BreakpointFile::log(Logging& logging) const {
	logging.debug("enable = " + enable, "JobOrder");
	logging.debug("fileType = " + fileType, "JobOrder");
	logging.debug("fileNameType = " + fileNameType, "JobOrder");
	logging.debug("fileName = " + fileName, "JobOrder");
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

Input::Input(const string& fileType, const string& fileNameType,
		const vector<string>& fileNames,
		const vector<TimeInterval>& timeIntervals) {
	this->fileType = fileType;
	this->fileNameType = fileNameType;
	this->fileNames = fileNames;
	this->timeIntervals = timeIntervals;
}

Input::~Input() {
}

void Input::log(Logging& logging) const {
	logging.debug("fileType = " + fileType, "JobOrder");
	logging.debug("fileNameType = " + fileNameType, "JobOrder");
	for (size_t i = 0; i < fileNames.size(); i++) {
		logging.debug(
				"fileName " + boost::lexical_cast<string>(i + 1) + ": "
						+ fileNames[i], "JobOrder");
	}
	for (size_t i = 0; i < timeIntervals.size(); i++) {
		timeIntervals[i].log(logging);
	}
}

const vector<TimeInterval>& Input::getTimeIntervals() const {
	return timeIntervals;
}

const vector<string>& Input::getFileNames() const {
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

Output::Output(const string& fileType, const string& fileNameType,
		const string& fileName) {
	this->fileType = fileType;
	this->fileNameType = fileNameType;
	this->fileName = fileName;
}

Output::~Output() {
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

void Output::log(Logging& logging) const {
	logging.debug("fileType = " + fileType, "JobOrder");
	logging.debug("fileNameType = " + fileNameType, "JobOrder");
	logging.debug("fileName = " + fileName, "JobOrder");
}

TimeInterval::TimeInterval(const string& start, const string& stop) {
	this->start = start;
	this->stop = stop;
}

TimeInterval::~TimeInterval() {
}

string TimeInterval::getStop() const {
	return stop;
}

string TimeInterval::getStart() const {
	return start;
}

void TimeInterval::log(Logging& logging) const {
	logging.debug("start = " + start, "JobOrder");
	logging.debug("stop = " + stop, "JobOrder");
}
