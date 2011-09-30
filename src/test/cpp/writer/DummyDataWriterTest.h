/*
 * DummyDataWriterTest.h
 *
 *  Created on: Sep 29, 2011
 *      Author: thomasstorm
 */

#ifndef DUMMYDATAWRITERTEST_H_
#define DUMMYDATAWRITERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"
#include "SynL2WriterTest.h"

class DummyDataWriterTest : public SynL2WriterTest {
    CPPUNIT_TEST_SUITE(DummyDataWriterTest);
    CPPUNIT_TEST(testWriter);
    CPPUNIT_TEST_SUITE_END();

public:
    DummyDataWriterTest();
    virtual ~DummyDataWriterTest();

protected:
    shared_ptr<Module> getSegmentProvider();
};

#endif /* DUMMYDATAWRITERTEST_H_ */
