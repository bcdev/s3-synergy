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

    // this line ensures that errors in netcdf don't cause a system exit.
    NcError ncError = NcError(NcError::verbose_nonfatal);

    // TODO - needed as argument
    string jobOrderXml = "/mnt/hgfs/S3L2PP/src/test/resources/syn/JobOrder.Test_1.xml";

    // TODO - error handler needed
    JobOrderParser parser = JobOrderParser(jobOrderXml);
    JobOrder jobOrder = parser.parseJobOrder();
//    jobOrder.print();

    // set up logger
    // TODO - use argument for log file name/path
    Logger* logger = Logger::get();
    logger->setErrLogLevel(jobOrder.getConfig().getErrorLogLevel());
    logger->setOutLogLevel(jobOrder.getConfig().getStandardLogLevel());
    // TODO - set processor version from job order to logger
    //    logger->init(jobOrder.getConfig().getOrderId());

    // configure modules
    // TODO - use job order for configuration
    MockReader reader;
    PixelClassification pixelClassification;
    TestModule test;
    SynL2Writer writer;

    Processor processor;
    processor.addModule(reader);
    processor.addModule(pixelClassification);
//    processor.addModule(test);
//    processor.addModule(writer);

    ProcessorContext context = ProcessorContext(jobOrder);
    processor.process(context);

    return 0;
}