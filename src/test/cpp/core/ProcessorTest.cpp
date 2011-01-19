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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * File:   ProcessorTest.cpp
 * Author: thomass
 * 
 * Created on January 18, 2011, 10:46 AM
 */

#include "../../../main/cpp/core/Context.h"
#include "../../../main/cpp/core/DefaultLogging.h"
#include "../../../main/cpp/util/ErrorHandler.h"
#include "../../../main/cpp/core/JobOrder.h"
#include "../../../main/cpp/util/JobOrderParser.h"
#include "../../../main/cpp/core/Processor.h"
#include "ProcessorTest.h"
#include "../../../main/cpp/modules/L1cReader.h"
#include "../../../main/cpp/util/XPathInitializer.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ProcessorTest);

ProcessorTest::ProcessorTest() {
}

ProcessorTest::~ProcessorTest() {
}

void ProcessorTest::setUp() {
    dict = new Dictionary();
}

void ProcessorTest::tearDown() {
    delete dict;
}

void ProcessorTest::testCatching() {

    XPathInitializer init;
    string jobOrderXml = "/mnt/hgfs/S3L2PP/src/test/resources/syn/JobOrder.1.xml";
    JobOrderParser parser = JobOrderParser(jobOrderXml);
    JobOrder* jobOrder = parser.parseJobOrder();

    L1cReader reader(".");

    Processor processor;
    DefaultLogging logging;
    Context context(logging);
    context.addModule(reader);
    context.setDictionary(dict);

    ErrorHandler handler;
    context.setErrorHandler(&handler);

    CPPUNIT_ASSERT_NO_THROW(processor.process(context));

    context.setJobOrder(jobOrder);

    CPPUNIT_ASSERT_NO_THROW(processor.process(context));

    ProductDescriptor& prodDesc = dict->addProductDescriptor(Constants::SYMBOLIC_NAME_L1C);
    SegmentDescriptor& segDesc = prodDesc.addSegmentDescriptor("segment");
    segDesc.addVariableDescriptor("variable");

    CPPUNIT_ASSERT_NO_THROW(processor.process(context));
}