/*
 * NetcdfSegmentTest.cpp
 *
 *  Created on: 01.02.2012
 *      Author: thomasstorm
 */

#include "../../../main/cpp/core/Dictionary.h"
#include "../../../main/cpp/core/NetcdfSegment.h"

#include "NetcdfSegmentTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(NetcdfSegmentTest);

NetcdfSegmentTest::NetcdfSegmentTest() {
	// TODO Auto-generated constructor stub

}

NetcdfSegmentTest::~NetcdfSegmentTest() {
	// TODO Auto-generated destructor stub
}

void NetcdfSegmentTest::tearDown() {
	boost::filesystem::remove(path(Constants::S3_SYNERGY_HOME) / "deleteme.nc");
}

void NetcdfSegmentTest::testSomething() {
	VariableDescriptor vd("testVariable");
	vd.setNcFileName("deleteme");
	vd.setNcName("testVariableNc");
	vd.setSegmentName("netcdfSegment");
	vd.setType(Constants::TYPE_BYTE);
	vd.addAttribute(Constants::TYPE_BYTE, "_FillValue", "-128");
	vd.addDimension("dim1");
	vd.addDimension("dim2");

	NetcdfSegment segment("netcdfSegment", Constants::S3_SYNERGY_HOME, 10, 10);
	segment.addVariable(vd);

	Accessor& acc = segment.getAccessor("testVariable");
	const Grid& grid = segment.getGrid();
	int8_t counter = 0;
	for (int l = 0; l < 10; l++) {
		for (int m = 0; m < 10; m++) {
			const size_t index = grid.getIndex(0, l, m);
			CPPUNIT_ASSERT(acc.isFillValue(index));
			acc.setByte(index, counter);
			CPPUNIT_ASSERT_EQUAL(counter, acc.getByte(index));
			counter++;
		}
	}
}

