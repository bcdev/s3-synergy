/* 
 * File:   ColTest.h
 * Author: thomasstorm
 *
 * Created on Sep 15, 2011, 1:30 PM
 */

#ifndef COLTEST_H
#define	COLTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "../../../../src/main/cpp/core/Context.h"
#include "../../../../src/main/cpp/core/Dictionary.h"

class ColTest : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(ColTest);
    CPPUNIT_TEST(testAddOlciVariables);
    CPPUNIT_TEST(testAddSlstrVariables);
    CPPUNIT_TEST(testRetrievePositionVariableName);
    CPPUNIT_TEST(testCol);
    CPPUNIT_TEST_SUITE_END();

public:
    ColTest();
    virtual ~ColTest();
    void setUp();
    void tearDown();

private:
    void testAddSlstrVariables();
    void testAddOlciVariables();
    void testRetrievePositionVariableName();
    void testCol();
    void prepareContext();
    shared_ptr<Context> context;
};

#endif	/* COLTEST_H */

