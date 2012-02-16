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

#include <cstdlib>

#include "../../../../src/main/cpp/core/Processor.h"
#include "../../../../src/main/cpp/writer/VgtWriter.h"
#include "../../../../src/main/cpp/util/DictionaryParser.h"
#include "../../../../src/main/cpp/util/JobOrderParser.h"

#include "VgtSegmentProvider.h"
#include "VgtWriterTest.h"

using std::getenv;

CPPUNIT_TEST_SUITE_REGISTRATION(VgtWriterTest);

VgtWriterTest::VgtWriterTest() {
}

VgtWriterTest::~VgtWriterTest() {
}

void VgtWriterTest::setUp() {
	XPathInitializer init;

	prepareContext();
}


void VgtWriterTest::prepareContext() {
    context = shared_ptr<Context>(new Context());
    shared_ptr<ErrorHandler> errorHandler = shared_ptr<ErrorHandler>(new ErrorHandler());
    context->setErrorHandler(errorHandler);

    JobOrderParser jobOrderParser;
    shared_ptr<JobOrder> jobOrder = jobOrderParser.parse(Constants::S3_SYNERGY_HOME + "/src/test/resources/jobs/JobOrder.SY_UNT_VWR.xml");
    context->setJobOrder(jobOrder);

    shared_ptr<Dictionary> dictionary = DictionaryParser().parse(Constants::S3_SYNERGY_HOME + "/src/main/resources/dictionary");
    context->setDictionary(dictionary);

    shared_ptr<Logging> logging = jobOrderParser.createLogging("LOG.SY_UNT_VWR");
    context->setLogging(logging);
}

void VgtWriterTest::tearDown() {
}

void VgtWriterTest::testWriter() {
    shared_ptr<Module> module = shared_ptr<Module>(new VgtSegmentProvider());
    shared_ptr<Module> writer = shared_ptr<Module>(new VgtWriter());
    context->addModule(module);
    context->addModule(writer);

    Processor processor;
	processor.process(*context);
}
