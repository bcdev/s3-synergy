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
 * File:   SynL2WriterTest.cpp
 * Author: ralf
 * 
 * Created on January 12, 2011, 11:48 AM
 */

#include "SynL2WriterTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SynL2WriterTest);

SynL2WriterTest::SynL2WriterTest() {
}

SynL2WriterTest::~SynL2WriterTest() {
}

void SynL2WriterTest::setUp() {
    writer = new SynL2Writer();
}

void SynL2WriterTest::tearDown() {
    delete writer;
}

void SynL2WriterTest::testWriter() {
    CPPUNIT_ASSERT(writer != 0);

    Dictionary dictionary;
    Variable& a = dictionary.addVariable("SYL2", "SDR_1", "SDR_1");
    a.setFileName("test");
    a.setType(NC_BYTE);
    a.setSegmentName(Constants::SYMBOLIC_NAME_SEGMENT_SYN_COLLOCATED);
    Variable& b = dictionary.addVariable("SYL2", "SDR_1_er", "SDR_1_er");
    b.setFileName("test");
    b.setType(NC_FLOAT);
    b.setSegmentName(Constants::SYMBOLIC_NAME_SEGMENT_SYN_COLLOCATED);

    Context context;
    context.setDictionary(&dictionary);
    Segment& segment = context.addSegment(Constants::SYMBOLIC_NAME_SEGMENT_SYN_COLLOCATED, 1, 2, 3, 0, 4);
    segment.addVariableByte(a.getSymbolicName());
    segment.addVariableFloat(b.getSymbolicName());
    Accessor& accessorA = segment.getAccessor(a.getSymbolicName());
    Accessor& accessorB = segment.getAccessor(b.getSymbolicName());
    valarray<int8_t>& bufferA = accessorA.getByteData();
    valarray<float>& bufferB = accessorB.getFloatData();

    bufferA[segment.getGrid().getIndex(0, 0, 0)] = 10;
    bufferA[segment.getGrid().getIndex(0, 0, 1)] = 11;
    bufferA[segment.getGrid().getIndex(1, 0, 0)] = 20;
    bufferA[segment.getGrid().getIndex(1, 0, 1)] = 21;
    bufferA[segment.getGrid().getIndex(2, 0, 0)] = 30;
    bufferA[segment.getGrid().getIndex(2, 0, 1)] = 31;

    bufferB[segment.getGrid().getIndex(0, 0, 0)] = 10;
    bufferB[segment.getGrid().getIndex(0, 0, 1)] = 11;
    bufferB[segment.getGrid().getIndex(1, 0, 0)] = 20;
    bufferB[segment.getGrid().getIndex(1, 0, 1)] = 21;
    bufferB[segment.getGrid().getIndex(2, 0, 0)] = 30;
    bufferB[segment.getGrid().getIndex(2, 0, 1)] = 31;

    writer->start(context);
    writer->process(context);
    writer->stop(context);
}