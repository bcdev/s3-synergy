extern "C" {
#include <netcdf.h>
}
#include <netcdfcpp.h>
#include <Safe.h>
#include "ConfigParser.h"
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace esa::safe;
using namespace std;
using namespace xercesc_3_1;

void initializeXerces() {
    try {
        XMLPlatformUtils::Initialize();
    } catch (const XMLException& toCatch) {
        // Do your failure processing here
    }
}

void terminateXerces() {
    XMLPlatformUtils::Terminate();
}

/**
 * Generates three empty files: two netCDF files and a SAFE archive.
 */
int main() {

    initializeXerces();
    string path = "/home/thomass/Desktop/test.xml";

    ConfigParser parser = ConfigParser(path);
    parser.readXml();
    //	DOMNode * perfExp = (DOMNode *)parser.getNodeForExpression( "syn/exportSyn/performExport" );
    Configuration configuration = parser.parseConfiguration();
    configuration.print();

    //	// create a netCDF file using the C++ API
    //	const NcFile dataFile("hello1.nc", NcFile::Replace, 0, 0, NcFile::Netcdf4);
    //	// create a netCDF file using the C API
    //	int ncid;
    //	nc_create((char*) "hello2.nc", NC_CLOBBER | NC_NETCDF4, &ncid);
    //	nc_close(ncid);
    //
    //	// create a SAFE archive using the C++ API
    //	Safe safeFile((char*) "hello3.safe",
    //			(char*) "esa/safe/1.2/envisat/meris/fr/level-2");
    //	safeFile.save();

    terminateXerces();
}
