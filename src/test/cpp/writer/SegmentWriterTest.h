/*
 * SegmentWriterTest.h
 *
 *  Created on: 23.01.2012
 *      Author: thomasstorm
 */

#ifndef SEGMENTWRITERTEST_H_
#define SEGMENTWRITERTEST_H_

#include <cppunit/extensions/HelperMacros.h>

class SegmentWriterTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(SegmentWriterTest);
    CPPUNIT_TEST(testWriter);
    CPPUNIT_TEST_SUITE_END();

public:
    SegmentWriterTest();
    virtual ~SegmentWriterTest();
    void setUp();
    void tearDown();

private:
    void testWriter();
};

#endif /* SEGMENTWRITERTEST_H_ */
