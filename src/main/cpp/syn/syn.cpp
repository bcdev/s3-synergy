#include <vector>

#include "../common/JobOrderParser.h"
#include "../common/Segment.h"
#include "../common/MockReader.h"
#include "../common/PixelClassification.h"
#include "../common/Processor.h"
#include "../common/ProcessorContext.h"
#include "../common/Writer.h"
#include "../common/SynL2Writer.h"

using std::list;

int main() {

    // Initialize Xerces and XPath
    XPathInitializer init;

    string path = "/mnt/hgfs/S3L2PP/src/test/resources/syn/JobOrder.Test_1.xml";

    JobOrderParser parser = JobOrderParser(path);
    JobOrder jobOrder = parser.parseJobOrder();
    jobOrder.print();

    SynL2Writer writer;
    PixelClassification pixelClassification;

    Processor processor;
    processor.addModule(pixelClassification);
    ProcessorContext context;
    MockReader reader(60000);
    size_t lineCount = 2000;

//    do {
//        size_t minL = context.getNextMinL(segment);
//        size_t maxL = minL + lineCount - 1;
//        Segment segment = reader.readSegment(minL, maxL);
//        processor.process(context);
//    } while (segment != 0);
}