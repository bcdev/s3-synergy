/*
 * Copyright (C) 2010 by Brockmann Consult (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 *
 * File:   ContextTest.cpp
 * Author: ralf
 *
 * Created on December 21, 2010, 1:08 PM
 */

#include <memory>

#include "ContextTest.h"
#include "../../../main/cpp/core/NullLogging.h"
#include "../../../main/cpp/core/DefaultModule.h"
#include "../../../main/cpp/core/Writer.h"
#include "TestObject.h"

using std::auto_ptr;

CPPUNIT_TEST_SUITE_REGISTRATION(ContextTest);

ContextTest::ContextTest() {
}

ContextTest::~ContextTest() {
}

void ContextTest::setUp() {
    context = new Context(NullLogging::getInstance());
}

void ContextTest::tearDown() {
    delete context;
}

void ContextTest::testInitialState() {
    CPPUNIT_ASSERT(context->getDictionary() == 0);
    CPPUNIT_ASSERT(context->getJobOrder() == 0);
    CPPUNIT_ASSERT(&context->getLogging() != 0);
    CPPUNIT_ASSERT(context->getModules().size() == 0);
}

void ContextTest::testAddModule() {
    const auto_ptr<Module> a(new DefaultModule("A"));
    const auto_ptr<Module> b(new DefaultModule("B"));
    context->addModule(*a);
    context->addModule(*b);
    const vector<Module*> modules = context->getModules();
    CPPUNIT_ASSERT(modules.size() == 2);
    CPPUNIT_ASSERT(modules[0] == a.get());
    CPPUNIT_ASSERT(modules[1] == b.get());
}

void ContextTest::testAddObject() {
    const auto_ptr<Object> o(new TestObject("O"));
    CPPUNIT_ASSERT(context->hasObject(o->getId()) == false);
    context->addObject(*o);
    CPPUNIT_ASSERT(context->hasObject(o->getId()));
    CPPUNIT_ASSERT(context->getObject(o->getId()) == *o);
    CPPUNIT_ASSERT_THROW(context->addObject(*o), logic_error);
}

void ContextTest::testAddSegment() {
    CPPUNIT_ASSERT(context->hasSegment("S") == false);
    const Segment& segment = context->addSegment("S", 2000);
    CPPUNIT_ASSERT(context->hasSegment("S"));
    CPPUNIT_ASSERT(context->getSegment("S") == segment);
    CPPUNIT_ASSERT_THROW(context->addSegment("S", 1000), logic_error);
}

void ContextTest::testSetGetDictionary() {
    CPPUNIT_ASSERT(context->getDictionary() == 0);
    Dictionary* dictionary = (Dictionary*) 1;
    context->setDictionary(dictionary);
    CPPUNIT_ASSERT(context->getDictionary() == dictionary);
}

void ContextTest::testSetGetJobOrder() {
    CPPUNIT_ASSERT(context->getJobOrder() == 0);
    JobOrder* jobOrder = (JobOrder*) 1;
    context->setJobOrder(jobOrder);
    CPPUNIT_ASSERT(context->getJobOrder() == jobOrder);
}

void ContextTest::testGetUnknownObject() {
    CPPUNIT_ASSERT_THROW(context->getObject("O"), logic_error);
}

void ContextTest::testGetUnknownSegment() {
    CPPUNIT_ASSERT_THROW(context->getSegment("S"), logic_error);
}

void ContextTest::testSetGetLastLComputed() {
    Segment& segment = context->addSegment("S", 100, 1, 1, 0, 200);
    DefaultModule module("M");
    CPPUNIT_ASSERT_THROW(context->setLastLComputed(segment, module, 10), logic_error);

    context->addModule(module);
    context->setLastLComputed(segment, module, 10);
    CPPUNIT_ASSERT(context->getLastLComputed(segment, module) == 10);
}

void ContextTest::testGetLastLWritable() {
    const Segment& segment = context->addSegment("A", 100, 1, 1, 0, 200);

    DefaultModule module1("M1");
    DefaultModule module2("M2");
    Writer writer("W");
    context->addModule(module1);
    context->setLastLComputed(segment, module1, 5);
    CPPUNIT_ASSERT_THROW(context->getLastLWritable(segment, writer), logic_error);

    context->addModule(writer);
    CPPUNIT_ASSERT(context->getLastLWritable(segment, writer) == 5);

    context->addModule(module2);
    context->setLastLComputed(segment, module2, 7);
    CPPUNIT_ASSERT(context->getLastLWritable(segment, writer) == 5);

    context->setLastLComputed(segment, module1, 9);
    CPPUNIT_ASSERT(context->getLastLWritable(segment, writer) == 7);

    context->setLastLComputed(segment, writer, 3);
    CPPUNIT_ASSERT(context->getLastLWritable(segment, writer) == 7);
}
