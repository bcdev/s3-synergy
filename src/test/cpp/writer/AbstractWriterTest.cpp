/*
 * SyL2WriterTest.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: ralf
 */

#include "../../../../src/main/cpp/writer/AbstractWriter.h"

#include "AbstractWriterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(AbstractWriterTest);

AbstractWriterTest::AbstractWriterTest() {
}

AbstractWriterTest::~AbstractWriterTest() {
}

void AbstractWriterTest::setUp() {
}

void AbstractWriterTest::tearDown() {
}

void AbstractWriterTest::testReplacing() {
    string first = "abcdefg";
    string toReplace = "b";
    AbstractWriter::replaceString(toReplace, "31415", first);
    CPPUNIT_ASSERT(first.compare("a31415cdefg") == 0);

    string second = "<checksum checksumName=\"MD5\">${checksum-tiepoints_meteo.nc}</checksum>";
    AbstractWriter::replaceString("\\$\\{checksum-tiepoints_meteo\\.nc\\}", "someChecksum", second);
    CPPUNIT_ASSERT(second.compare("<checksum checksumName=\"MD5\">someChecksum</checksum>") == 0);
}

