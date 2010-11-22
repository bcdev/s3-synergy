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
 * File:   ProcessorContext.h
 * Author: ralf
 *
 * Created on November 22, 2010, 10:58 AM
 */

#ifndef PROCESSORCONTEXT_H
#define	PROCESSORCONTEXT_H

#include <string>
#include <vector>

//#include "Module.h"
#include "Segment.h"

using std::vector;
using std::string;

class Module;

class ProcessorContext {
public:
    ProcessorContext();
    virtual ~ProcessorContext();
    Module& getModule(string moduleId) const;
    const vector<Module*>& getModules() const;
    Segment& getSegment(string segmentId) const;
    void addModule(Module& module);
    void setMaxComputedLine( Segment& segment, Module& module, size_t line );
    size_t getMaxComputedLine( Segment& segment, Module& module ) const;
    size_t getOverlap( Segment& segment ) const;
    void setOverlap( Segment& segment, size_t overlap );

private:
    vector<Module*> modules;
};

#endif	/* PROCESSORCONTEXT_H */

