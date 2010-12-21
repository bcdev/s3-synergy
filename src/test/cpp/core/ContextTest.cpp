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
#include "../../../main/cpp/core/DefaultModule.h"
#include "TestObject.h"

using std::auto_ptr;

CPPUNIT_TEST_SUITE_REGISTRATION(ContextTest);

ContextTest::ContextTest() {
}

ContextTest::~ContextTest() {
}

void ContextTest::setUp() {
    context = new Context();
}

void ContextTest::tearDown() {
    delete context;
}

void ContextTest::testInitialState() {
    CPPUNIT_ASSERT(context->getDictionary() == 0);
    CPPUNIT_ASSERT(context->getJobOrder() == 0);
    CPPUNIT_ASSERT(context->getLogging() == 0);
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
    CPPUNIT_ASSERT_THROW(context->addObject(*o), logic_error);
}


