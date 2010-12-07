#include <time.h>
#include <unistd.h>
#include <vector>

#include "../common/JobOrderParser.h"
#include "../common/MockReader.h"
#include "../common/PixelClassification.h"
#include "../common/Processor.h"
#include "../common/ProcessorContext.h"
#include "../common/StringUtils.h"
#include "../common/SynL2Writer.h"
#include "../common/TestModule.h"

#include <iostream>

static string createProcessingTimeMessage(time_t start);

int main() {


    // take the current time;
    clock_t start = clock();

    // Initialize Xerces and XPath
    XPathInitializer init;

    // this line ensures that errors in netcdf don't cause a system exit.
    NcError ncError = NcError(NcError::verbose_nonfatal);

    // TODO - needed as argument
    string jobOrderXml = "/mnt/hgfs/S3L2PP/src/test/resources/syn/JobOrder.Test_1.xml";

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
    jobOrder.log();

    vector<ProcessorConfiguration*> processorList = jobOrder.getProcessorList();
    for (size_t h = 0; h < processorList.size(); h++) {
        vector<Input*> inputList = processorList[h]->getInputList();
        for (size_t i = 0; i < inputList.size(); i++) {
            for (size_t j = 0; j < inputList[i]->getFileNames().size(); j++) {
                string message = "Input file: " + inputList[i]->getFileNames()[j];
                logger->info(message, "JobOrderParser");
            }
        }
        vector<Output*> outputList = processorList[h]->getOutputList();
        for (size_t i = 0; i < outputList.size(); i++) {
            string message = "Output file: " + outputList[i]->getFileName();
            logger->info(message, "JobOrderParser");
        }
    }

    // configure modules
    // TODO - use job order for configuration
    MockReader reader;
    PixelClassification pixelClassification;
    TestModule test;
    SynL2Writer writer;

    Processor processor;
    processor.addModule(reader);
    processor.addModule(pixelClassification);
    processor.addModule(test);
    processor.addModule(writer);

    ProcessorContext context = ProcessorContext(jobOrder);
    processor.process(context);

    logger->info(createProcessingTimeMessage(start), "Main");
    logger->info("Processing complete. Exit code: 0", "Main");
    delete logger;
    return 0;
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