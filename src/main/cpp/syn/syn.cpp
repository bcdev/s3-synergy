#include <time.h>
#include <unistd.h>
#include <vector>

#include "../core/Dictionary.h"
#include "../util/JobOrderParser.h"
#include "../util/Logger.h"
#include "../core/Processor.h"
#include "../modules/Reader.h"
#include "../modules/SynL2Writer.h"

#include <iostream>

static string createProcessingTimeMessage(time_t start);
void logIOParameters(JobOrder& jobOrder, Logger* logger);

int main() {


    clock_t start = clock();
    XPathInitializer init;

    // TODO - needed as argument
    string jobOrderXml = "/mnt/hgfs/S3L2PP/src/test/resources/syn/JobOrder.1.xml";

    // TODO - error handler needed
    JobOrderParser parser = JobOrderParser(jobOrderXml);
    JobOrder jobOrder = parser.parseJobOrder();

    // set up logger
    // TODO - use argument for log file name/path
    Logger* logger = Logger::get();
    // TODO - set processor version from job order to logger
    logger->init(jobOrder.getConfig().getOrderId());
    logger->setOutLogLevel(jobOrder.getConfig().getStandardLogLevel());
    logger->setErrLogLevel(jobOrder.getConfig().getErrorLogLevel());
    logger->info("Main process started.", "Main");
    //    jobOrder.log();
    //    logIOParameters(jobOrder, logger);

    // TODO - get config file correct for current processor, not simply the first
    Dictionary dict(jobOrder.getConfig().getConfigFileNames()[0]);

    // configure modules
    // TODO - use job order for configuration
    Reader reader;
    //TestModule test;
    SynL2Writer writer(".");

    Processor processor;

    Context context;
    context.setLogging(logger);
    context.setJobOrder(&jobOrder);
    context.addModule(reader);
    //    context.addModule(writer);
    context.setDictionary(&dict);

    processor.process(context);

    logger->info(createProcessingTimeMessage(start), "Main");
    logger->info("Processing complete. Exit code: 0", "Main");
    logger->close();
    return 0;
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