/* 
 * File:   AerTest.h
 * Author: thomasstorm
 *
 * Created on Sep 22, 2011
 */

#ifndef AERTEST_H
#define	AERTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class AerTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(AerTest);
//    CPPUNIT_TEST(testReadAuxdata);
//    CPPUNIT_TEST(testErrorMetric);
//    CPPUNIT_TEST(testAotStandardError);
//    CPPUNIT_TEST(testNdv);
    CPPUNIT_TEST(testAer);
    CPPUNIT_TEST_SUITE_END();

public:
    AerTest();
    virtual ~AerTest();
    void setUp();
    void tearDown();

private:
    static const double EPSILON;
    void prepareContext();
    void testReadAuxdata();
    void testAer();
    void testNdv();
    void testAotStandardError();
    void testErrorMetric();
    shared_ptr<Context> context;
    shared_ptr<Aer> aer;
};

class AerTestPixel : public AerPixel {
public:

    AerTestPixel(Segment& segment, long k, long l, long m) : AerPixel(segment, k, l, m) {

    }

    double getRadiance(int16_t index) {
        if(index == 9) {
            return 5;
        } else if(index == 17) {
            return 10;
        } else return 0.0;
    }
};

#endif	/* AERTEST_H */

