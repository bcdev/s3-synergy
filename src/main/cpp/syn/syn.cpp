#include <vector>

#include "../common/JobOrderParser.h"
#include "../common/MockReader.h"
#include "../common/PixelClassification.h"
#include "../common/Processor.h"
#include "../common/ProcessorContext.h"
#include "../common/SynL2Writer.h"
#include "../common/TestModule.h"

#include <iostream>

int main() {

    // Initialize Xerces and XPath
    XPathInitializer init;

    string path = "/mnt/hgfs/S3L2PP/src/test/resources/syn/JobOrder.Test_1.xml";

    JobOrderParser parser = JobOrderParser(path);
    JobOrder jobOrder = parser.parseJobOrder();
    //    jobOrder.print();

    MockReader reader;
    PixelClassification pixelClassification;
    TestModule test;
    SynL2Writer writer;

    Processor processor;
    processor.addModule(reader);
    processor.addModule(pixelClassification);
    processor.addModule(test);
//    processor.addModule(writer);

    Logger* logger = Logger::get();
    logger->setErrLogLevel(jobOrder.getConfig().getErrorLogLevel());
    logger->setOutLogLevel(jobOrder.getConfig().getStandardLogLevel());
//    logger->init(jobOrder.getConfig().getOrderId());

    ProcessorContext context = ProcessorContext(jobOrder);
    processor.process(context);

    //logger->writeLogFile(jobOrder.getConfig().getOrderId());
    delete logger;
    return 0;
}