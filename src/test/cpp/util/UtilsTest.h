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
 * File:   UtilsTest.h
 * Author: thomass
 *
 * Created on January 6, 2011, 12:14 PM
 */

#ifndef UTILSTEST_H
#define	UTILSTEST_H

#include <cppunit/extensions/HelperMacros.h>

class UtilsTest: public CPPUNIT_NS::TestFixture {
CPPUNIT_TEST_SUITE(UtilsTest);
		CPPUNIT_TEST(testCreateCountVector);
		CPPUNIT_TEST(testCreateStartVector);
		CPPUNIT_TEST(testReplacing);
	CPPUNIT_TEST_SUITE_END();

public:
	UtilsTest();
	virtual ~UtilsTest();
	void setUp();
	void tearDown();
private:
	void testCreateCountVector();
	void testCreateStartVector();
	void testReplacing();
};

#endif	/* UTILSTEST_H */

