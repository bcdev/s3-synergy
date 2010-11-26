#include <vector>

#include "../common/JobOrderParser.h"
#include "../common/Segment.h"
#include "../common/MockReader.h"
#include "../common/PixelClassification.h"
#include "../common/Processor.h"
#include "../common/ProcessorContext.h"
#include "../common/SynL2Writer.h"

#include <iostream>

using std::list;

int main() {

    // Initialize Xerces and XPath
    XPathInitializer init;

    string path = "/mnt/hgfs/S3L2PP/src/test/resources/syn/JobOrder.Test_1.xml";

    JobOrderParser parser = JobOrderParser(path);
    JobOrder jobOrder = parser.parseJobOrder();
    //    jobOrder.print();

    MockReader reader;
    PixelClassification pixelClassification;

    Processor processor;
    processor.addModule(reader);
    processor.addModule(pixelClassification);

    Logger* logger = Logger::get();
    logger->setErrLogLevel(jobOrder.getConfig().getErrorLogLevel());
    logger->setOutLogLevel(jobOrder.getConfig().getStandardLogLevel());

    ProcessorContext context = ProcessorContext();
    processor.process(context);

    //logger->writeLogFile(jobOrder.getConfig().getOrderId());
    delete logger;
}