/*
 * SynL2WriterTest.h
 *
 *  Created on: Sep 2, 2011
 *      Author: ralf
 */

#ifndef SYNL2WRITERTEST_H_
#define SYNL2WRITERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class SynL2WriterTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SynL2WriterTest);
    CPPUNIT_TEST(testWriter);
    CPPUNIT_TEST_SUITE_END();

public:
    SynL2WriterTest();
    virtual ~SynL2WriterTest();
    void setUp();
    void tearDown();

protected:
    virtual shared_ptr<Module> getSegmentProvider();
    void testWriter();
    void prepareContext();
    shared_ptr<Context> context;
};

#endif /* SYNL2WRITERTEST_H_ */
