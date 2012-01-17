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
 * File:   VgtSegmentProvider.h
 * Author: ralf
 *
 * Created on January 19, 2011, 5:07 PM
 */

#ifndef VGTSEGMENTPROVIDER_H
#define	VGTSEGMENTPROVIDER_H

#include "../../../main/cpp/core/AbstractModule.h"

class VgtSegmentProvider : public AbstractModule {
public:
    VgtSegmentProvider();
    virtual ~VgtSegmentProvider();

    void start(Context& context);
    void stop(Context& context);
    void process(Context& context);

private:
    void setLastComputedLines(Context& context, const string& identifier);
    vector<SegmentDescriptor*> getSegmentDescriptors(Context& context);
};

#endif	/* VGTSEGMENTPROVIDER_H */

