/*
 * SyL2WriterTest.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: ralf
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/writer/AbstractWriter.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"
#include "../../../../src/main/cpp/core/NullLogging.h"

#include "SynL2SegmentProvider.h"
#include "AbstractWriterTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(AbstractWriterTest);

AbstractWriterTest::AbstractWriterTest() {
}

AbstractWriterTest::~AbstractWriterTest() {
}

void AbstractWriterTest::setUp() {
	XPathInitializer init;

	prepareContext();
}


void AbstractWriterTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    JobOrderParser jobOrderParser;
    shared_ptr<JobOrder> jobOrder = jobOrderParser.parse(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_GEN.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(Constants::S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context->setDictionary(dictionary);

    shared_ptr<Logging> logging = shared_ptr<Logging>(new NullLogging());
    context->setLogging(logging);
}

void AbstractWriterTest::tearDown() {
}

void AbstractWriterTest::testReplacing() {
    shared_ptr<AbstractWriter> writer = shared_ptr<AbstractWriter>(new SynL2Writer());

    string first = "abcdefg";
    string toReplace = "b";
    writer->replaceString(toReplace, "31415", first);
    CPPUNIT_ASSERT(first.compare("a31415cdefg") == 0);

    string second = "<checksum checksumName=\"MD5\">${checksum-tiepoints_meteo.nc}</checksum>";
    writer->replaceString("\\$\\{checksum-tiepoints_meteo\\.nc\\}", "someChecksum", second);
    CPPUNIT_ASSERT(second.compare("<checksum checksumName=\"MD5\">someChecksum</checksum>") == 0);
}

