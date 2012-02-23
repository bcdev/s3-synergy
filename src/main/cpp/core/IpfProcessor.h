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

#ifndef IPFPROCESSOR_H
#define	IPFPROCESSOR_H

#include <string>
#include <vector>

#include "Logging.h"

using std::string;
using std::vector;

class BreakpointFile;
class Input;
class Output;
class TimeInterval;

/**
 * Represents the processor element in an IPF Job Order.
 */
class IpfProcessor {
public:
	IpfProcessor(const string& taskName, const string& taskVersion, const vector<BreakpointFile>& breakpointFiles, const vector<Input>& inputList, const vector<Output>& outputList);
	~IpfProcessor();
	string getTaskName() const;
	string getTaskVersion() const;
	const vector<BreakpointFile>& getBreakpointFiles() const;
	const vector<Input>& getInputList() const;
	const vector<Output>& getOutputList() const;
	void log(Logging& logging) const;
private:
	string taskName;
	string taskVersion;
	vector<BreakpointFile> breakpointFiles;
	vector<Input> inputList;
	vector<Output> outputList;
};

class BreakpointFile {
public:
	BreakpointFile(const string& enable, const string& fileType, const string& fileNameType, const string& fileName);
	~BreakpointFile();
	string getFileName() const;
	string getFileNameType() const;
	string getFileType() const;
	string getEnable() const;
	void log(Logging& logging) const;
private:
	string enable;
	string fileType;
	string fileNameType;
	string fileName;
};

class Input {
public:
	Input(const string& fileType, const string& fileNameType, const vector<string>& fileNames, const vector<TimeInterval>& timeIntervals);
	~Input();
	const vector<TimeInterval>& getTimeIntervals() const;
	const vector<string>& getFileNames() const;
	string getFileNameType() const;
	string getFileType() const;
	void log(Logging& logging) const;
private:
	string fileType;
	string fileNameType;
	vector<string> fileNames;
	vector<TimeInterval> timeIntervals;
};

class Output {
public:
	Output(const string& fileType, const string& fileNameType, const string& fileName);
	~Output();
	void log(Logging& logging) const;
	string getFileName() const;
	string getFileNameType() const;
	string getFileType() const;
private:
	string fileType;
	string fileNameType;
	string fileName;
};

class TimeInterval {
public:
	TimeInterval(const string& start, const string& stop);
	~TimeInterval();
	void log(Logging& logging) const;
	string getStop() const;
	string getStart() const;
private:
	string start;
	string stop;
};

#endif	/* IPFPROCESSOR_H */

