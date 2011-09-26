/* 
 * File:   AcoTest.cpp
 * Author: ralf
 * 
 * Created on August 16, 2011, 1:36 PM
 */

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/reader/SynL1Reader.h"
#include "../../../../src/main/cpp/modules/Col.h"
#include "../../../../src/main/cpp/modules/Aco.h"
#include "../../../../src/main/cpp/writer/SynL2Writer.h"

#include "AcoTest.h"

extern shared_ptr<Context> context;

CPPUNIT_TEST_SUITE_REGISTRATION(AcoTest);

AcoTest::AcoTest() {
}

AcoTest::~AcoTest() {
}

void AcoTest::setUp() {
	shared_ptr<Module> reader = shared_ptr<Module>(new SynL1Reader());
	shared_ptr<Module> col = shared_ptr<Module>(new Col());
	shared_ptr<Module> aco = shared_ptr<Module>(new Aco());
	shared_ptr<Module> writer = shared_ptr<Module>(new SynL2Writer());

	context->addModule(reader);
	context->addModule(col);
	context->addModule(aco);
	context->addModule(writer);
}

void AcoTest::tearDown() {
	foreach (shared_ptr<Module> m, context->getModules())
			{
				context->removeModule(m);
			}
}

void AcoTest::testAco() {
	Processor processor;
	processor.process(*context);
}
