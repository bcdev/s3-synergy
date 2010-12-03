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
 * File:   TestModule.h
 * Author: thomass
 *
 * Created on December 1, 2010, 11:30 AM
 */

#ifndef TESTMODULE_H
#define	TESTMODULE_H

#include "AbstractModule.h"
#include "Variable.h"

class TestModule : public AbstractModule {
public:
    TestModule();
    virtual ~TestModule();

    Segment* processSegment(ProcessorContext& context);

    size_t getMinLineRequired(size_t line) const;

private:
    static const size_t overlap = 100;
    Variable* createSDR_1Variable();
    Attribute<void*>* createIntAttribute(string key, int value);
    Attribute<void*>* createShortAttribute(string key, int value);
    Attribute<void*>* createStringAttribute(string key, string value);
    Attribute<void*>* createFloatAttribute(string key, float value);
};

#endif	/* TESTMODULE_H */

