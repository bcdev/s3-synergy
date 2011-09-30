/*
 * DummyDataWriterTest.cpp
 *
 *  Created on: Sep 29, 2011
 *      Author: thomasstorm
 */

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"

#include "DummyDataWriterTest.h"
#include "DummyDataSegmentProvider.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(DummyDataWriterTest);

DummyDataWriterTest::DummyDataWriterTest() {
}

DummyDataWriterTest::~DummyDataWriterTest() {
}

shared_ptr<Module> DummyDataWriterTest::getSegmentProvider() {
    return shared_ptr<Module>(new DummyDataSegmentProvider());
}
