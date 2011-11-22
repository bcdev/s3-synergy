/*
 * syl2.cpp
 *
 *  Created on: Nov 22, 2011
 *      Author: ralf
 */

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/ExitCode.h"
#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/modules/Aer.h"
#include "../../../../src/main/cpp/modules/Aei.h"
#include "../../../../src/main/cpp/modules/Ave.h"
#include "../../../../src/main/cpp/modules/Aco.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Pcl.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"

void logIOParameters(JobOrder& jobOrder, Logging& logging);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Missing job order file." << std::endl;
		std::cerr << "Usage: " << argv[0] << " <jobOrderPath>" << std::endl;
		return ExitCode::FAILURE;
	}
	if (getenv("S3_SYNERGY_HOME") == NULL) {
		std::cerr << "The test runner cannot be executed because the" << std::endl;
		std::cerr << "'S3_SYNERGY_HOME' environment variable has not" << std::endl;
		std::cerr << "been set." << std::endl;
		return ExitCode::FAILURE;
	}

	Context context;
    try {
		XPathInitializer init;

		shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
	    context.setErrorHandler(errorHandler);

	    shared_ptr<JobOrderParser> jobOrderParser = shared_ptr<JobOrderParser>(new JobOrderParser());
		const string jobOrderPath = string(argv[1]);

	    shared_ptr<JobOrder> jobOrder = jobOrderParser->parse(jobOrderPath);
	    context.setJobOrder(jobOrder);

		string logFileName = "LOG.";
		logFileName.append(jobOrder->getIpfConfiguration().getOrderId());
	    shared_ptr<Logging> logging = jobOrderParser->createLogging(logFileName);
	    context.setLogging(logging);

	    const string S3_SYNERGY_HOME = getenv("S3_SYNERGY_HOME");
	    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(S3_SYNERGY_HOME + "/src/main/resources/dictionary");
	    context.setDictionary(dictionary);

		shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
		shared_ptr<Module> col = shared_ptr<Module>(new Col());
		shared_ptr<Module> pcl = shared_ptr<Module>(new Pcl());
		shared_ptr<Module> ave = shared_ptr<Module>(new Ave());
		shared_ptr<Module> aer = shared_ptr<Module>(new Aer());
		shared_ptr<Module> aei = shared_ptr<Module>(new Aei());
		shared_ptr<Module> aco = shared_ptr<Module>(new Aco());
		shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

		context.addModule(reader);
		context.addModule(col);
		context.addModule(pcl);
		context.addModule(ave);
		context.addModule(aer);
		context.addModule(aei);
		context.addModule(aco);
		context.addModule(writer);

		Processor processor;
		processor.process(context);
		return ExitCode::OK;
	} catch (exception& e) {
		std::cerr << "An error has occurred while initializing the processor. Error Message:\n";
		std::cerr << e.what() << std::endl;
		std::cerr << "The system will exit.";
		return ExitCode::FAILURE;
	}
}

void logIOParameters(JobOrder& jobOrder, Logging& logging) {
	const vector<IpfProcessor>& processorConfigurations =
			jobOrder.getIpfProcessors();
	for (size_t k = 0; k < processorConfigurations.size(); k++) {
		const vector<Input>& inputList =
				processorConfigurations[k].getInputList();
		for (size_t i = 0; i < inputList.size(); i++) {
			const vector<string>& inputFileNames = inputList[i].getFileNames();
			for (size_t j = 0; j < inputFileNames.size(); j++) {
				const string message = "Input file: " + inputFileNames[j];
				logging.info(message, "JobOrder");
			}
		}
		const vector<Output>& outputList =
				processorConfigurations[k].getOutputList();
		for (size_t i = 0; i < outputList.size(); i++) {
			const string message = "Output file: " + outputList[i].getFileName();
			logging.info(message, "JobOrder");
		}
	}
}
