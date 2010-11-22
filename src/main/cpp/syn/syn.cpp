extern "C" {
#include <netcdf.h>
}
#include <vector>
#include <netcdfcpp.h>
#include <Safe.h>

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
    Configuration configuration = parser.parseConfiguration();
    configuration.print();

    SynL2Writer writer;
    PixelClassification pixelClassification;

    Processor processor;

    ProcessorContext context;
    context.addModule(pixelClassification);
    context.addModule(writer);

    MockReader reader(4, 1, 8, 8);

    Segment* segment = 0;
    do {
        segment = reader.getNextSegment();
        processor.process(context);
    } while(segment != 0);

    //    	// create a netCDF file using the C++ API
    //    	const NcFile dataFile("hello1.nc", NcFile::Replace, 0, 0, NcFile::Netcdf4);
    //    	// create a netCDF file using the C API
    //    	int ncid;
    //    	nc_create((char*) "hello2.nc", NC_CLOBBER | NC_NETCDF4, &ncid);
    //    	nc_close(ncid);
    //
    //    	// create a SAFE archive using the C++ API
    //    	Safe safeFile((char*) "hello3.safe",
    //    			(char*) "esa/safe/1.2/envisat/meris/fr/level-2");
    //    	safeFile.save();
}