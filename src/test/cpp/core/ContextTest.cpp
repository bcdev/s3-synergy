/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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
 */

#include "../../../main/cpp/core/AbstractModule.h"

#include "TestObject.h"
#include "ContextTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ContextTest);

ContextTest::ContextTest() {
}

ContextTest::~ContextTest() {
}

void ContextTest::setUp() {
	context = shared_ptr<Context>(new Context());
}

void ContextTest::tearDown() {
}

void ContextTest::testInitialState() {
	CPPUNIT_ASSERT_THROW(context->getDictionary(), logic_error);
	CPPUNIT_ASSERT_THROW(context->getJobOrder(), logic_error);
	CPPUNIT_ASSERT_NO_THROW(context->getLogging());
	CPPUNIT_ASSERT(context->getModules().size() == 0);
}

void ContextTest::testAddModule() {
	const shared_ptr<Module> a(new AbstractModule("A"));
	const shared_ptr<Module> b(new AbstractModule("B"));
	context->addModule(a);
	context->addModule(b);
	const vector<shared_ptr<Module> > modules = context->getModules();
	CPPUNIT_ASSERT(modules.size() == 2);
	CPPUNIT_ASSERT(modules[0] == a);
	CPPUNIT_ASSERT(modules[1] == b);
}

void ContextTest::testAddObject() {
	const shared_ptr<Identifiable> o(new TestObject("O"));
	CPPUNIT_ASSERT(context->hasObject(o->getId()) == false);
	context->addObject(o);
	CPPUNIT_ASSERT(context->hasObject(o->getId()));
	CPPUNIT_ASSERT(context->getObject(o->getId()) == *o);
	CPPUNIT_ASSERT_THROW(context->addObject(o), logic_error);
}

void ContextTest::testAddRemoveSegment() {
	CPPUNIT_ASSERT(context->hasSegment("S") == false);
	Segment& segment = context->addSwathSegment("S", 2000, Constants::N_DET_CAM,
			Constants::N_CAM, 0, 9999);
	CPPUNIT_ASSERT(context->hasSegment("S"));
	CPPUNIT_ASSERT(context->getSegment("S") == segment);
	CPPUNIT_ASSERT_THROW(
			context->addSwathSegment("S", 1000, Constants::N_DET_CAM, Constants::N_CAM, 0, 9999),
			logic_error);

	CPPUNIT_ASSERT(context->removeSegment("S"));
	CPPUNIT_ASSERT(!context->hasSegment("S"));
}

void ContextTest::testSetGetDictionary() {
	CPPUNIT_ASSERT_THROW(context->getDictionary(), logic_error);
	shared_ptr<Dictionary> dictionary = shared_ptr<Dictionary>(
			new Dictionary());
	context->setDictionary(dictionary);
	CPPUNIT_ASSERT(&context->getDictionary() == dictionary.get());
}

void ContextTest::testSetGetJobOrder() {
	CPPUNIT_ASSERT_THROW(context->getJobOrder(), logic_error);
	shared_ptr<JobOrder> jobOrder = shared_ptr<JobOrder>(new JobOrder());
	context->setJobOrder(jobOrder);
	CPPUNIT_ASSERT(&context->getJobOrder() == jobOrder.get());
}

void ContextTest::testGetUnknownObject() {
	CPPUNIT_ASSERT_THROW(context->getObject("O"), logic_error);
}

void ContextTest::testGetUnknownSegment() {
	CPPUNIT_ASSERT_THROW(context->getSegment("S"), logic_error);
}

void ContextTest::testSetGetLastComputedL() {
	Segment& s = context->addSwathSegment("S", 100, 1, 1, 0, 199);
	shared_ptr<Module> m = shared_ptr<Module>(new AbstractModule("M"));
	CPPUNIT_ASSERT_THROW(context->setLastComputedL(s, *m, 10), logic_error);

	context->addModule(m);
	context->setLastComputedL(s, *m, 10);
	CPPUNIT_ASSERT(context->getLastComputedL(s, *m) == 10);

	CPPUNIT_ASSERT(context->removeSegment("S"));
	CPPUNIT_ASSERT(!context->hasLastComputedL(s, *m));
}
