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
 * File:   PixelClassification.cpp
 * Author: thomass
 *
 * Created on November 10, 2010, 4:26 PM
 */

#include "PixelClassification.h"
#include "ProcessorContext.h"

PixelClassification::PixelClassification() : AbstractModule("PCL") {
}

PixelClassification::~PixelClassification() {
}

Segment* PixelClassification::processSegment(ProcessorContext& context) {
    Segment& segment = context.getSegment("SYN_COLLOCATED");
    if (!segment.hasVariable("SYN_flags")) {
        segment.addVariableInt("SYN_flags");
    }
    Logger::get()->progress("Starting to process segment [" + segment.toString() + "]", getModuleId(), getVersion());
    const Grid& grid = segment.getGrid();
    for (size_t cam = grid.getStartK(); cam <= grid.getSizeK(); cam++) {
        for (size_t line = getMinLineNotComputed(segment, context); line <= grid.getSizeL(); line++) {
            for (size_t col = grid.getStartM(); col <= grid.getSizeM(); col++) {
                const size_t p = grid.getIndex(cam, line, col);
                const int olcFlags = segment.getAccessor("F_OLC").getInt(p);
                const int slnFlags = segment.getAccessor("F_SLN").getInt(p);
                const int sloFlags = segment.getAccessor("F_SLO").getInt(p);
                const bool olcLand = (olcFlags & 0x1000) != 0;
                const bool slnLand = (slnFlags & 0x0800) != 0;
                const bool sloLand = (sloFlags & 0x0800) != 0;
                const bool slnCloud = (slnFlags & 0x00400000) != 0;
                const bool sloCloud = (sloFlags & 0x00400000) != 0;

                // TODO - OLCI L2 Pixel classification

                int synFlags = 0;
                if (olcLand && slnLand && sloLand) {
                    synFlags |= 0x0020;
                }
                if (slnCloud || sloCloud) {
                    synFlags |= 0x0001;
                }
                segment.getAccessor("SYN_flags").setInt(p, synFlags);
            }
        }
    }
    // TODO - check if needed here; don't want to set this explicitly for a pixel processor
    context.setMaxLineComputed(segment, *this, grid.getSizeL());

    return &segment;
}

Variable* PixelClassification::createSYN_flagsVariable() {
    Variable* var = new VariableImpl("SYN_flags", ncInt);
    var->addDimension(new Dimension("N_CAM", 5)); // Number of OLCI camera modules
    var->addDimension(new Dimension("N_LINE_OLC", 10000)); // Number of lines in OLCI camera image - TODO - replace with correct value
    var->addDimension(new Dimension("N_DET_CAM", 760)); // Number of pixels per line in OLCI camera image - TODO - replace with correct value
    var->addAttribute(Variable::createStringAttribute("standard_name", "surface_directional_reflectance"));
    var->addAttribute(Variable::createStringAttribute("long_name", "Surface directional reflectance for SYN channel 1"));
    var->addAttribute(Variable::createFloatAttribute("_FillValue", -10.000));
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