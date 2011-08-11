#include <time.h>
#include <unistd.h>
#include <vector>

#include "../util/ErrorHandler.h"
#include "../core/Dictionary.h"
#include "../util/JobOrderParser.h"
#include "../util/Logger.h"
#include "../core/Processor.h"
#include "../modules/L1cReader.h"
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

        JobOrderParser parser = JobOrderParser(jobOrderXml);
        JobOrder* jobOrder = parser.parseJobOrder();

        // set up logger
        // TODO - use argument for log file name/path
        string logFileName = "LOG.";
        logFileName.append(jobOrder->getConfig().getOrderId());
        Logger logger(logFileName);
        logger.setProcessorVersion(jobOrder->getConfig().getVersion());
        logger.setOutLogLevel(jobOrder->getConfig().getStandardLogLevel());
        logger.setErrLogLevel(jobOrder->getConfig().getErrorLogLevel());
        logger.info("Main process started.", "Main");

        Dictionary* dict = new Dictionary();

        // configure modules
        // TODO - replace "SYL2" by 'argv[0]'
        L1cReader reader(jobOrder->getProcessorConfiguration("SYL2").getInputList()[0]->getFileNames()[0]);
        TestModule test;
        SynL2Writer writer(jobOrder->getProcessorConfiguration("SYL2").getOutputList()[0]->getFileName());

        Processor processor;

        Context context(logger);
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
        errorHandler.handleInitError(e);
    }
}

void logIOParameters(JobOrder& jobOrder, Logger* logger) {
    vector<ProcessorConfiguration*> processorList = jobOrder.getProcessorList();
    for (size_t h = 0; h < processorList.size(); h++) {
        vector<Input*> inputList = processorList[h]->getInputList();
        for (size_t i = 0; i < inputList.size(); i++) {
            for (size_t j = 0; j < inputList[i]->getFileNames().size(); j++) {
                string message = "Input file: " + inputList[i]->getFileNames()[j];
                logger->info(message, "JobOrder");
            }
        }
        vector<Output*> outputList = processorList[h]->getOutputList();
        for (size_t i = 0; i < outputList.size(); i++) {
            string message = "Output file: " + outputList[i]->getFileName();
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
    message << "Processing took " << hours << ":" << minutes << ":" << seconds << " (HH:MM:SS.mm).";
    return message.str();
}
