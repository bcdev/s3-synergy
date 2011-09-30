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
 * File:   JobOrderParser.cpp
 * Author: thomass
 *
 * Created on November 15, 2010, 4:31 PM
 */

#include <stdexcept>

#include "../core/Boost.h"
#include "../core/Constants.h"

#include "JobOrderParser.h"
#include "DebugLogging.h"

using std::string;
using std::bad_cast;
using std::logic_error;

JobOrderParser::JobOrderParser() :
        parser() {
}

JobOrderParser::~JobOrderParser() {
}

shared_ptr<JobOrder> JobOrderParser::parse(const string& path) {
	IpfConfiguration config = parseIpfConfiguration(path);
	vector<IpfProcessor> ipfProcessors =
			parseIpfProcessors(path);
	return shared_ptr<JobOrder>(new JobOrder(config, ipfProcessors));
}

IpfConfiguration JobOrderParser::parseIpfConfiguration(const string& path) {
	IpfConfiguration configuration;
	string value = parser.evaluateToString(path, "/Ipf_Job_Order/Ipf_Conf/Stdout_Log_Level");
	if (value.empty() || (value.compare(Constants::LOG_LEVEL_INFO) != 0
	                && value.compare(Constants::LOG_LEVEL_DEBUG) != 0
					&& value.compare(Constants::LOG_LEVEL_WARNING) != 0
					&& value.compare(Constants::LOG_LEVEL_PROGRESS) != 0
					&& value.compare(Constants::LOG_LEVEL_ERROR) != 0)) {
		value = Constants::LOG_LEVEL_INFO; // default value
	}
	standardLogLevel = value;
	configuration.setStandardLogLevel(value);

	value = parser.evaluateToString(path,
			"/Ipf_Job_Order/Ipf_Conf/Stderr_Log_Level");
	if (value.empty()
			|| (value.compare(Constants::LOG_LEVEL_INFO) != 0 && value.compare(Constants::LOG_LEVEL_DEBUG) != 0
					&& value.compare(Constants::LOG_LEVEL_WARNING) != 0
					&& value.compare(Constants::LOG_LEVEL_PROGRESS) != 0
					&& value.compare(Constants::LOG_LEVEL_ERROR) != 0)) {
		value = Constants::LOG_LEVEL_INFO; // default value
	}
	configuration.setErrorLogLevel(value);

	const size_t firstDotIndex = path.find_first_of(".");
	const string orderIdWithExt = path.substr(firstDotIndex + 1);
	configuration.setOrderId(orderIdWithExt.substr(0, orderIdWithExt.length() - 4));

	value = parser.evaluateToString(path,
			"/Ipf_Job_Order/Ipf_Conf/Processor_Name");
	configuration.setProcessorName(value);

	value = parser.evaluateToString(path, "/Ipf_Job_Order/Ipf_Conf/Version");
	configuration.setVersion(value);

	value = parser.evaluateToString(path, "/Ipf_Job_Order/Ipf_Conf/Test");
	try {
		configuration.setTest(lexical_cast<bool>(value));
	} catch (bad_cast& e) {
		configuration.setTest(value.compare("true") == 0 || value.compare("True") == 0 || value.compare("TRUE") == 0);
	}

	value = parser.evaluateToString(path,
			"/Ipf_Job_Order/Ipf_Conf/Acquisition_Station");
	configuration.setAcquisitionStation(value);

	value = parser.evaluateToString(path,
			"/Ipf_Job_Order/Ipf_Conf/Processing_Station");
	configuration.setProcessingStation(value);

	value = parser.evaluateToString(path,
			"/Ipf_Job_Order/Ipf_Conf/Sensing_Time/Start");
	configuration.setSensingTimeStart(value);

	value = parser.evaluateToString(path,
			"/Ipf_Job_Order/Ipf_Conf/Sensing_Time/Stop");
	configuration.setSensingTimeStop(value);

    value = parser.evaluateToString(path, "/Ipf_Job_Order/Ipf_Conf/Breakpoint_Enable");
    to_lower(value);
    bool enableBreakpoint = value.compare("true") == 0 ? true : false;
	configuration.setBreakpointEnable(enableBreakpoint);

	vector<string> keys = parser.evaluateToStringList(path, "/Ipf_Job_Order/Ipf_Conf/Dynamic_Processing_Parameters/Processing_Parameter/Name/child::text()");
	vector<string> values = parser.evaluateToStringList(path, "/Ipf_Job_Order/Ipf_Conf/Dynamic_Processing_Parameters/Processing_Parameter/Value/child::text()");
	vector<ProcessingParameter> parameters;
	for (size_t i = 0; i < keys.size(); i++) {
		parameters.push_back(ProcessingParameter(keys.at(i), values.at(i)));
	}
	configuration.setDynamicProcessingParameters(parameters);

	vector<string> configFileNames = parser.evaluateToStringList(path, "/Ipf_Job_Order/Ipf_Conf/Config_Files/Conf_File_Name/child::text()");
	configuration.setConfigFileNames(configFileNames);

	return configuration;
}

vector<IpfProcessor> JobOrderParser::parseIpfProcessors(
		const string& path) const {
	vector<string> values = parser.evaluateToStringList(path,
			"/Ipf_Job_Order/List_of_Ipf_Procs/Ipf_Proc");
	vector<IpfProcessor> result;
	for (size_t i = 0; i < values.size(); i++) {
		result.push_back(parseIpfProcessor(path, i + 1));
	}
	return result;
}

IpfProcessor JobOrderParser::parseIpfProcessor(
		const string& path, int index) const {
	string baseQuery = "/Ipf_Job_Order/List_of_Ipf_Procs/Ipf_Proc[";
	baseQuery.append(lexical_cast<string>(index));
	baseQuery.append("]");

	const string taskNameQuery = baseQuery + "/Task_Name";
	const string taskName = parser.evaluateToString(path, taskNameQuery);

	const string taskVersionQuery = baseQuery + "/Task_Version";
	const string taskVersion = parser.evaluateToString(path, taskVersionQuery);

	vector<BreakpointFile> breakpointFiles = parseBreakpointFiles(path,
			baseQuery);
	vector<Input> inputList = parseInputs(path, baseQuery);
	vector<Output> outputList = parseOutputs(path, baseQuery);
	IpfProcessor processorConfigurartion(taskName, taskVersion,
			breakpointFiles, inputList, outputList);

	return processorConfigurartion;
}

vector<BreakpointFile> JobOrderParser::parseBreakpointFiles(const string& path,
		const string& baseQuery) const {
	const string breakPointFilesQuery = baseQuery
			+ "/BreakPoint/List_of_Brk_Files/Brk_File";
	const size_t breakPointFileCount = parser.evaluateToStringList(path,
			breakPointFilesQuery).size();
	vector<BreakpointFile> breakpointFiles;
	for (size_t i = 0; i < breakPointFileCount; i++) {
		const string query = breakPointFilesQuery + "["
				+ lexical_cast<string>(i + 1) + "]";
		breakpointFiles.push_back(parseBreakpointFile(path, query));
	}
	return breakpointFiles;
}

BreakpointFile JobOrderParser::parseBreakpointFile(const string& path,
		const string& baseQuery) const {
	const string enableQuery = baseQuery + "/Enable";
	const string enable = parser.evaluateToString(path, enableQuery);

	const string fileTypeQuery = baseQuery + "/File_Type";
	const string fileType = parser.evaluateToString(path, fileTypeQuery);

	const string fileNameTypeQuery = baseQuery + "/File_Name_Type";
	string fileNameType = parser.evaluateToString(path, fileNameTypeQuery);
	if (fileNameType.compare("") == 0) {
		fileNameType = "Physical";
	}

	const string fileNameQuery = baseQuery + "/File_Name";
	const string fileName = parser.evaluateToString(path, fileNameQuery);

	BreakpointFile breakpointFile(enable, fileType, fileNameType, fileName);

	return breakpointFile;
}

vector<Input> JobOrderParser::parseInputs(const string& path,
		const string& baseQuery) const {
	const string inputQuery = baseQuery + "/List_of_Inputs/Input";
	const size_t inputCount =
			parser.evaluateToStringList(path, inputQuery).size();
	vector<Input> inputs;
	for (size_t i = 0; i < inputCount; i++) {
		const string query = inputQuery + "[" + lexical_cast<string>(i + 1)
				+ "]";
		inputs.push_back(parseInput(path, query));
	}
	return inputs;
}

Input JobOrderParser::parseInput(const string& path,
		const string& baseQuery) const {
	const string fileTypeQuery = baseQuery + "/File_Type";
	const string fileType = parser.evaluateToString(path, fileTypeQuery);

	const string fileNameTypeQuery = baseQuery + "/File_Name_Type";
	string fileNameType = parser.evaluateToString(path, fileNameTypeQuery);
	if (fileNameType.compare("") == 0) {
		fileNameType = "Physical";
	}

	const string fileNamesQuery = baseQuery + "/List_of_File_Names/File_Name";
	const size_t fileNameCount = parser.evaluateToStringList(path,
			fileNamesQuery).size();
	vector<string> fileNames;
	for (size_t i = 0; i < fileNameCount; i++) {
		const string fileNameQuery = fileNamesQuery + "["
				+ lexical_cast<string>(i + 1) + "]";
		fileNames.push_back(parser.evaluateToString(path, fileNameQuery));
	}

	const string timeIntervalsQuery = baseQuery
			+ "/List_of_Time_Intervals/Time_Interval";
	const size_t timeIntervalsCount = parser.evaluateToStringList(path,
			timeIntervalsQuery).size();
	vector<TimeInterval> timeIntervals;
	for (size_t i = 0; i < timeIntervalsCount; i++) {
		const string timeIntervalsStartQuery = timeIntervalsQuery + "["
				+ lexical_cast<string>(i + 1) + "]/Start";
		const string timeIntervalsStopQuery = timeIntervalsQuery + "["
				+ lexical_cast<string>(i + 1) + "]/Stop";
		const string start = parser.evaluateToString(path,
				timeIntervalsStartQuery);
		const string stop = parser.evaluateToString(path,
				timeIntervalsStopQuery);
		TimeInterval timeInterval(start, stop);
		timeIntervals.push_back(timeInterval);
	}
	Input input(fileType, fileNameType, fileNames, timeIntervals);

	return input;
}

vector<Output> JobOrderParser::parseOutputs(const string& path,
		const string& baseQuery) const {
	const string outputQuery = baseQuery + "/List_of_Outputs/Output";
	const size_t outputCount =
			parser.evaluateToStringList(path, outputQuery).size();
	vector<Output> outputs;
	for (size_t i = 0; i < outputCount; i++) {
		const string query = outputQuery + "[" + lexical_cast<string>(i + 1)
				+ "]";
		outputs.push_back(parseOutput(path, query));
	}
	return outputs;
}

Output JobOrderParser::parseOutput(const string& path,
		const string& baseQuery) const {
	const string fileTypeQuery = baseQuery + "/File_Type";
	const string fileType = parser.evaluateToString(path, fileTypeQuery);

	const string fileNameTypeQuery = baseQuery + "/File_Name_Type";
	string fileNameType = parser.evaluateToString(path, fileNameTypeQuery);
	if (fileNameType.compare("") == 0) {
		fileNameType = "Physical";
	}

	const string fileNameQuery = baseQuery + "/File_Name";
	const string fileName = parser.evaluateToString(path, fileNameQuery);

	Output output(fileType, fileNameType, fileName);
	return output;
}

const shared_ptr<Logging> JobOrderParser::createLogging(const string& logFileName) const {
    if(standardLogLevel.empty()) {
        BOOST_THROW_EXCEPTION(logic_error("The ipf configuration needs to be parsed first."));
    }
    if(standardLogLevel.compare("DEBUG") == 0) {
        return shared_ptr<Logging>(new DebugLogging(logFileName));
    } else if(standardLogLevel.compare("INFO") == 0) {
        return shared_ptr<Logging>(new InfoLogging(logFileName));
    }  else if(standardLogLevel.compare("PROGRESS") == 0) {
        return shared_ptr<Logging>(new ProgressLogging(logFileName));
    } else if(standardLogLevel.compare("WARNING") == 0) {
        return shared_ptr<Logging>(new WarningLogging(logFileName));
    } else if(standardLogLevel.compare("ERROR") == 0) {
        return shared_ptr<Logging>(new ErrorLogging(logFileName));
    } else if(standardLogLevel.compare("NULL") == 0) {
        return shared_ptr<Logging>(new NullLogging());
    }
    BOOST_THROW_EXCEPTION(logic_error("invalid standard log level '" + standardLogLevel + "'."));
}
