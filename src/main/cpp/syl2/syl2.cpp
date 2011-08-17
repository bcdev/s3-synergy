#include <time.h>
#include <unistd.h>
#include <vector>

#include "../util/ErrorHandler.h"
#include "../core/Dictionary.h"
#include "../util/JobOrderParser.h"
#include "../util/Logger.h"
#include "../core/Processor.h"
#include "../reader/L1cReader.h"
#include "../modules/SynL2Writer.h"
#include "../modules/TestModule.h"

#include <iostream>

static string createProcessingTimeMessage(time_t start);
void logIOParameters(JobOrder& jobOrder, Logger* logger);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Missing job order file." << std::endl;
		std::cerr << "Usage: " << argv[0] << " <jobOrderFile>." << std::endl;
		exit(128);
	}
	ErrorHandler errorHandler;
	try {
		clock_t start = clock();
		XPathInitializer init;

		string jobOrderXml = string(argv[1]);

		JobOrderParser parser;
		JobOrder* jobOrder = parser.parse(jobOrderXml);

		// set up logger
		// TODO - use argument for log file name/path
		string logFileName = "LOG.";
		logFileName.append(jobOrder->getIpfConfiguration().getOrderId());
		Logger logger(logFileName);
		logger.setProcessorVersion(jobOrder->getIpfConfiguration().getVersion());
		logger.setOutLogLevel(jobOrder->getIpfConfiguration().getStandardLogLevel());
		logger.setErrLogLevel(jobOrder->getIpfConfiguration().getErrorLogLevel());
		logger.info("Main process started.", "Main");

		Dictionary* dict = new Dictionary();

		// configure modules
		// TODO - replace "SYL2" by 'argv[0]'
		L1cReader reader;
//		L1cReader reader(
//				jobOrder.getProcessorConfiguration("SYL2").getInputList()[0].getFileNames()[0]);
		TestModule test;
		SynL2Writer writer(
				jobOrder->getIpfProcessor("SYL2").getOutputList()[0].getFileName());

		Processor processor;

		Context context;
		context.setLogging(&logger);
		context.setErrorHandler(&errorHandler);
		context.setJobOrder(jobOrder);
		context.addModule(reader);
		context.addModule(test);
		context.addModule(writer);
		context.setDictionary(dict);
		processor.process(context);

		logger.info(createProcessingTimeMessage(start), "Main");
		logger.info("Processing complete. Exit code: 0.", "Main");
		return 0;
	} catch (exception& e) {
		errorHandler.handleInitializationError(e);
	}
	return 0;
}

void logIOParameters(JobOrder& jobOrder, Logger* logger) {
	const vector<IpfProcessor>& processorConfigurations =
			jobOrder.getIpfProcessors();
	for (size_t k = 0; k < processorConfigurations.size(); k++) {
		const vector<Input>& inputList =
				processorConfigurations[k].getInputList();
		for (size_t i = 0; i < inputList.size(); i++) {
			const vector<string>& inputFileNames = inputList[i].getFileNames();
			for (size_t j = 0; j < inputFileNames.size(); j++) {
				const string message = "Input file: " + inputFileNames[j];
				logger->info(message, "JobOrder");
			}
		}
		const vector<Output>& outputList =
				processorConfigurations[k].getOutputList();
		for (size_t i = 0; i < outputList.size(); i++) {
			const string message = "Output file: " + outputList[i].getFileName();
			logger->info(message, "JobOrder");
		}
	}
}

static string createProcessingTimeMessage(time_t start) {
	clock_t finish = clock();
	double seconds = ((double) (finish - start)) / (CLOCKS_PER_SEC);
	int minutes = 0;
	int hours = 0;
	if (seconds > 3600) {
		hours = seconds / 3600;
		seconds = ((int) seconds) % 3600;
	}
	if (seconds > 60) {
		minutes = seconds / 60;
		seconds = ((int) seconds) % 60;
	}
	std::stringstream message;
	message << "Processing took " << hours << ":" << minutes << ":" << seconds
			<< " (HH:MM:SS.mm).";
	return message.str();
}
