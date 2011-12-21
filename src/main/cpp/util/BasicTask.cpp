/*
 * BasicTask.cpp
 *
 *  Created on: Nov 23, 2011
 *      Author: ralf
 */

#include "../core/ExitCode.h"
#include "../core/Processor.h"
#include "DictionaryParser.h"
#include "JobOrderParser.h"

#include "BasicTask.h"

BasicTask::BasicTask(const string& name) : name(name) {
}

BasicTask::~BasicTask() {
}

int BasicTask::execute(int argc, char** argv) {
	if (argc < 2) {
		std::cerr << "Error: missing job order file\n";
		std::cerr << "Usage: " << argv[0] << " JOB_ORDER_FILE" << std::endl;

		return ExitCode::FAILURE;
	}
	return execute(argv[1]);
}

int BasicTask::execute(const string& jobOrderPath) {
    JobOrderParser jobOrderParser;
	try {
		shared_ptr<JobOrder> jobOrder = jobOrderParser.parse(jobOrderPath);
		context.setJobOrder(jobOrder);
	} catch (exception& e) {
		std::cerr << "Error while parsing the job order file:\n";
		std::cerr << e.what() << std::endl;

		return ExitCode::FAILURE;
	}
	try {
		string logFileName = "LOG.";
		logFileName.append(context.getJobOrder().getIpfConfiguration().getOrderId());
	    shared_ptr<Logging> logging = jobOrderParser.createLogging(logFileName);
	    context.setLogging(logging);
	} catch (exception& e) {
		std::cerr << "Error while creating the log file:\n";
		std::cerr << e.what() << std::endl;

		return ExitCode::FAILURE;
	}
	logIoParameters(context.getJobOrder(), context.getLogging());

	shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context.setErrorHandler(errorHandler);

    DictionaryParser dictionaryParser;
    shared_ptr<Dictionary> dictionary = dictionaryParser.parse(Constants::S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context.setDictionary(dictionary);

	Processor processor;
	processor.process(context);

	return ExitCode::OK;
}

void BasicTask::logIoParameters(JobOrder& jobOrder, Logging& logging) const {
	const vector<IpfProcessor>& processorConfigurations =
			jobOrder.getIpfProcessors();
	for (size_t k = 0; k < processorConfigurations.size(); k++) {
		const vector<Input>& inputList =
				processorConfigurations[k].getInputList();
		for (size_t i = 0; i < inputList.size(); i++) {
			const vector<string>& inputFileNames = inputList[i].getFileNames();
			for (size_t j = 0; j < inputFileNames.size(); j++) {
				const string message = "Input file: " + inputFileNames[j];
				logging.info(message, name);
			}
		}
		const vector<Output>& outputList =
				processorConfigurations[k].getOutputList();
		for (size_t i = 0; i < outputList.size(); i++) {
			const string message = "Output file: " + outputList[i].getFileName();
			logging.info(message, name);
		}
	}
}

