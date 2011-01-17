/* 
 * Copyright (C) 2011 by Brockmann Consult (info@brockmann-consult.de)
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
 * File:   DictionaryTest.cpp
 * Author: ralf
 * 
 * Created on January 14, 2011, 10:03 AM
 */

#include "DictionaryTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(DictionaryTest);

DictionaryTest::DictionaryTest() {
}

DictionaryTest::~DictionaryTest() {
}

void DictionaryTest::setUp() {
    dictionary = new Dictionary();
}

void DictionaryTest::tearDown() {
    delete dictionary;
}

void DictionaryTest::testContainers() {
    ProductDescriptor& productDescriptor = dictionary->addProductDescriptor("L1C");
    CPPUNIT_ASSERT(productDescriptor.getName().compare("L1C") == 0);

    vector<ProductDescriptor*> volumes = dictionary->getProductDescriptors();
    CPPUNIT_ASSERT(volumes.size() == 1);
    CPPUNIT_ASSERT(volumes[0] == &productDescriptor);

    CPPUNIT_ASSERT(&dictionary->getProductDescriptor("L1C") == &productDescriptor);
    CPPUNIT_ASSERT_THROW(dictionary->getProductDescriptor("L2"), std::out_of_range);

    CPPUNIT_ASSERT(dictionary->hasProductDescriptor("L1C"));
    CPPUNIT_ASSERT(!dictionary->hasProductDescriptor("L2"));

    SegmentDescriptor& segmentDescriptor = productDescriptor.addSegmentDescriptor("SYN_COLLOCATED");

    CPPUNIT_ASSERT(productDescriptor.hasSegmentDescriptor(segmentDescriptor.getName()));
    CPPUNIT_ASSERT(&productDescriptor.getSegmentDescriptor("SYN_COLLOCATED") == &segmentDescriptor);

    Attribute& volAttribute = productDescriptor.addAttribute(1, "Version", "37");

    CPPUNIT_ASSERT(productDescriptor.hasAttribute(volAttribute.getName()));
    CPPUNIT_ASSERT(&productDescriptor.getAttribute("Version") == &volAttribute);

    Attribute& sectAttribute = segmentDescriptor.addAttribute(0, "Vendor", "BC");

    CPPUNIT_ASSERT(segmentDescriptor.hasAttribute(sectAttribute.getName()));
    CPPUNIT_ASSERT(&segmentDescriptor.getAttribute("Vendor") == &sectAttribute);

    VariableDescriptor& variableDescriptor = segmentDescriptor.addVariableDescriptor("reflec_1");

    CPPUNIT_ASSERT(dictionary->getProductDescriptor("L1C").getVariables().size() == 1);
}