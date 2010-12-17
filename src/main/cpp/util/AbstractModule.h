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
 * File:   AbstractModule.h
 * Author: ralf
 *
 * Created on November 24, 2010, 8:53 AM
 */

#ifndef ABSTRACTMODULE_H
#define	ABSTRACTMODULE_H

#include <algorithm>

#include "Module.h"
#include "ProcessorContext.h"
#include "VariableImpl.h"

using std::max;

class AbstractModule : public Module {
public:

    AbstractModule(const string& moduleId) : id(moduleId), version("1.0-SNAPSHOT") {
    };

    AbstractModule(const string& moduleId, string version) : id(moduleId), version(version) {
    };

    virtual ~AbstractModule() {
    };

    const string& getModuleId() const {
        return id;
    };

    const string& getVersion() const {
        return version;
    }

    virtual void start() {

    };

    virtual void stop() {
    };

    /**
     * Returns the minimum line of the input product, which is needed for the
     * module to compute the line given as parameter. Method may be subclassed.
     * 
     * @param line The index of a line to be computed.
     * @return The corresponding input line needed for the computation.
     */
    virtual size_t getMinLineRequired(size_t line) const {
        return line;
    }

    virtual size_t getMinLineNotComputed(Segment& segment, ProcessorContext& context) const {
        if (context.hasMaxLineComputed(segment, *this)) {
            return max(segment.getL(), context.getMaxLineComputed(segment, *this) + 1);
        } else {
            return segment.getL();
        }
    }

protected:
    // TODO - move to Segment.h
    Variable* createVariable(string id) {
        Variable* var = new VariableImpl("SDR_1", ncInt);
        var->addDimension(new Dimension("N_CAM", 5)); // Number of OLCI camera modules
        var->addDimension(new Dimension("N_LINE_OLC", 10000)); // Number of lines in OLCI camera image - TODO - replace with correct value
        var->addDimension(new Dimension("N_DET_OLC", 760)); // Number of pixels per line in OLCI camera image - TODO - replace with correct value
        var->addAttribute(Variable::createStringAttribute("standard_name", "surface_directional_reflectance"));
        var->addAttribute(Variable::createStringAttribute("long_name", "Surface directional reflectance for SYN channel 1"));
        var->addAttribute(Variable::createFloatAttribute("_FillValue", -10000));
        var->addAttribute(Variable::createFloatAttribute("scale_factor", 0.0001));
        var->addAttribute(Variable::createShortAttribute("valid_min", 0));
        var->addAttribute(Variable::createShortAttribute("valid_max", 10000));
        var->addAttribute(Variable::createStringAttribute("ancillary_variables", "SDR_1_er"));
        var->addAttribute(Variable::createShortAttribute("channel", 1));
        var->addAttribute(Variable::createFloatAttribute("central_wavelength", 400));
        var->addAttribute(Variable::createFloatAttribute("min_wavelength", 100));
        var->addAttribute(Variable::createFloatAttribute("max_wavelength", 700));
        return var;
    }

private:
    const string id;
    const string version;
};

#endif	/* ABSTRACTMODULE_H */

