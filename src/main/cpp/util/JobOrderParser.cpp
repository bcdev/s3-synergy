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
#include "../core/Configuration.h"
#include "JobOrderParser.h"
#include "../core/ProcessorConfiguration.h"
#include "Logger.h"

using std::string;
using std::bad_cast;

JobOrderParser::JobOrderParser(string path) {
    this->path = path;
}

JobOrderParser::~JobOrderParser() {
}

JobOrder JobOrderParser::parseJobOrder() {
    Configuration config = parseConfiguration();
    vector<ProcessorConfiguration*> processorConfigurations = parseProcessorConfigurations();
    JobOrder jobOrder = JobOrder(config, processorConfigurations);
    return jobOrder;
}

Configuration JobOrderParser::parseConfiguration() {
    Configuration config;
    string query = "/Ipf_Job_Order/Ipf_Conf/Stdout_Log_Level";
    string value = evaluateToString(path, query);
    if (value.empty() && value.compare("INFO") != 0 &&
            value.compare("DEBUG") != 0 &&
            value.compare("WARNING") != 0 &&
            value.compare("PROGRESS") != 0 &&
            value.compare("ERROR") != 0) {
        value = "INFO"; // default value
    }
    config.setStandardLogLevel(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Stderr_Log_Level";
    value = evaluateToString(path, query);
    if (value.empty() && value.compare("INFO") != 0 &&
            value.compare("DEBUG") != 0 &&
            value.compare("WARNING") != 0 &&
            value.compare("PROGRESS") != 0 &&
            value.compare("ERROR") != 0) {
        value = "INFO"; // default value
    }
    config.setErrorLogLevel(value);

    size_t firstDotIndex = path.find_first_of(".");
    string orderIdWithExt = path.substr(firstDotIndex + 1);
    config.setOrderId(orderIdWithExt.substr(0, orderIdWithExt.length() - 4));

    query = "/Ipf_Job_Order/Ipf_Conf/Processor_Name";
    value = evaluateToString(path, query);
    config.setProcessorName(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Version";
    value = evaluateToString(path, query);
    config.setVersion(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Test";
    value = evaluateToString(path, query);
    try {
        config.setTest(lexical_cast<bool>(value));
    } catch (bad_cast) {
        config.setTest((value.compare("true") == 0)
                || (value.compare("True") == 0)
                || (value.compare("TRUE") == 0));
    }

    query = "/Ipf_Job_Order/Ipf_Conf/Acquisition_Station";
    value = evaluateToString(path, query);
    config.setAcquisitionStation(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Processing_Station";
    value = evaluateToString(path, query);
    config.setProcessingStation(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Sensing_Time/Start";
    value = evaluateToString(path, query);
    config.setSensingTimeStart(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Sensing_Time/Stop";
    value = evaluateToString(path, query);
    config.setSensingTimeStop(value);

    query = "/Ipf_Job_Order/Ipf_Conf/Dynamic_Processing_Parameters/Processing_Parameter/Name/child::text()";
    string query2 = "/Ipf_Job_Order/Ipf_Conf/Dynamic_Processing_Parameters/Processing_Parameter/Value/child::text()";
    vector<string> keys = evaluateToStringList(path, query);
    vector<string> values = evaluateToStringList(path, query2);
    vector<ProcessingParameter*> parameters;
    for (size_t i = 0; i < keys.size(); i++) {
        parameters.push_back(new ProcessingParameter(keys.at(i), values.at(i)));
    }
    config.setProcessingParameters(parameters);

    query = "/Ipf_Job_Order/Ipf_Conf/Config_Files/Conf_File_Name/child::text()";
    vector<string> strings = evaluateToStringList(path, query);
    config.setConfigFileNames(strings);

    return config;
}

vector<ProcessorConfiguration*> JobOrderParser::parseProcessorConfigurations() {
    vector<ProcessorConfiguration*> result;
    string query = "/Ipf_Job_Order/List_of_Ipf_Procs/Ipf_Proc";
    vector<string> values = evaluateToStringList(path, query);
    int numberOfProcConfigurations = values.size();
    for (int i = 1; i <= numberOfProcConfigurations; i++) {

        result.push_back(parseProcessorConfiguration(i));
    }
    return result;
}

ProcessorConfiguration* JobOrderParser::parseProcessorConfiguration(int index) {
    string baseQuery = "/Ipf_Job_Order/List_of_Ipf_Procs/Ipf_Proc[";
    baseQuery.append(lexical_cast<string > (index));
    baseQuery.append("]");

    string taskNameQuery = baseQuery + "/Task_Name";
    string taskName = evaluateToString(path, taskNameQuery);

    string taskVersionQuery = baseQuery + "/Task_Version";
    string taskVersion = evaluateToString(path, taskVersionQuery);

    vector<BreakpointFile*> breakpointFiles = parseBreakpointFiles(baseQuery);
    vector<Input*> inputList = parseInputs(baseQuery);
    vector<Output*> outputList = parseOutputs(baseQuery);

    ProcessorConfiguration* config = new ProcessorConfiguration(taskName,
            taskVersion, breakpointFiles, inputList, outputList);

    return config;
}

vector<BreakpointFile*> JobOrderParser::parseBreakpointFiles(string baseQuery) {
    vector<BreakpointFile*> breakpointFiles;
    string breakPointFilesQuery = baseQuery + "/BreakPoint/List_of_Brk_Files/Brk_File";
    int breakPointFilesCount = evaluateToStringList(path, breakPointFilesQuery).size();
    for (int i = 1; i <= breakPointFilesCount; i++) {

        string query = breakPointFilesQuery + "[" + lexical_cast<string > (i) + "]";
        breakpointFiles.push_back(parseBreakpointFile(query));
    }
    return breakpointFiles;
}

BreakpointFile* JobOrderParser::parseBreakpointFile(string baseQuery) {
    string enableQuery = baseQuery + "/Enable";
    string enable = evaluateToString(path, enableQuery);

    string fileTypeQuery = baseQuery + "/File_Type";
    string fileType = evaluateToString(path, fileTypeQuery);

    string fileNameTypeQuery = baseQuery + "/File_Name_Type";
    string fileNameType = evaluateToString(path, fileNameTypeQuery);
    if (fileNameType.compare("") == 0) {
        fileNameType = "Physical";
    }

    string fileNameQuery = baseQuery + "/File_Name";
    string fileName = evaluateToString(path, fileNameQuery);

    BreakpointFile* breakpointFile = new BreakpointFile(enable, fileType, fileNameType, fileName);

    return breakpointFile;
}

vector<Input*> JobOrderParser::parseInputs(string baseQuery) {
    vector<Input*> inputs;
    string inputQuery = baseQuery + "/List_of_Inputs/Input";
    int inputCount = evaluateToStringList(path, inputQuery).size();
    for (int i = 1; i <= inputCount; i++) {

        string query = inputQuery + "[" + lexical_cast<string > (i) + "]";
        inputs.push_back(parseInput(query));
    }
    return inputs;
}

Input* JobOrderParser::parseInput(string baseQuery) {
    string fileTypeQuery = baseQuery + "/File_Type";
    string fileType = evaluateToString(path, fileTypeQuery);

    string fileNameTypeQuery = baseQuery + "/File_Name_Type";
    string fileNameType = evaluateToString(path, fileNameTypeQuery);
    if (fileNameType.compare("") == 0) {
        fileNameType = "Physical";
    }

    vector<string> fileNames;
    string fileNamesQuery = baseQuery + "/List_of_File_Names/File_Name";
    int fileNameCount = evaluateToStringList(path, fileNamesQuery).size();
    for (int i = 1; i <= fileNameCount; i++) {
        string fileNameQuery = fileNamesQuery + "[" + lexical_cast<string > (i) + "]";
        fileNames.push_back(evaluateToString(path, fileNameQuery));
    }

    vector<TimeInterval*> timeIntervals;
    string timeIntervalsQuery = baseQuery + "/List_of_Time_Intervals/Time_Interval";
    int timeIntervalsCount = evaluateToStringList(path, timeIntervalsQuery).size();
    for (int i = 1; i <= timeIntervalsCount; i++) {
        string timeIntervalsStartQuery = timeIntervalsQuery + "[" + lexical_cast<string > (i) + "]/Start";
        string timeIntervalsStopQuery = timeIntervalsQuery + "[" + lexical_cast<string > (i) + "]/Stop";
        string start = evaluateToString(path, timeIntervalsStartQuery);
        string stop = evaluateToString(path, timeIntervalsStopQuery);
        TimeInterval* timeInterval = new TimeInterval(start, stop);
        timeIntervals.push_back(timeInterval);
    }

    Input* input = new Input(fileType, fileNameType, fileNames, timeIntervals);

    return input;
}

vector<Output*> JobOrderParser::parseOutputs(string baseQuery) {
    vector<Output*> outputs;
    string outputQuery = baseQuery + "/List_of_Outputs/Output";
    int outputCount = evaluateToStringList(path, outputQuery).size();
    for (int i = 1; i <= outputCount; i++) {

        string query = outputQuery + "[" + lexical_cast<string > (i) + "]";
        outputs.push_back(parseOutput(query));
    }
    return outputs;
}

Output* JobOrderParser::parseOutput(string baseQuery) {
    string fileTypeQuery = baseQuery + "/File_Type";
    string fileType = evaluateToString(path, fileTypeQuery);

    string fileNameTypeQuery = baseQuery + "/File_Name_Type";
    string fileNameType = evaluateToString(path, fileNameTypeQuery);
    if (fileNameType.compare("") == 0) {
        fileNameType = "Physical";
    }

    string fileNameQuery = baseQuery + "/File_Name";
    string fileName = evaluateToString(path, fileNameQuery);

    Output* output = new Output(fileType, fileNameType, fileName);
    return output;
}