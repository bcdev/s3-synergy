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
 * File:   Pcl.h
 * Author: Thomas Storm, Ralf Quast
 *
 * Created on Sep 14, 2011
 */

#ifndef PCL_H_
#define PCL_H_

#include "../modules/BasicModule.h"

class Pcl: public BasicModule {
public:
	Pcl();
	virtual ~Pcl();

	void start(Context& context);
	void process(Context& context);

private:
	friend class PclTest;

	static uint16_t computeOlcSummaryFlags(uint32_t sourceFlags);
	static uint32_t computeSlsSummaryFlags(uint8_t sourceFlags, const vector<const Accessor*>& exceptionAccessors, size_t index);
	static uint16_t computeSynFlags(uint32_t sourceOlcFlags, uint8_t sourceSlnFlags, uint8_t sourceSloFlags, const vector<const Accessor*>& radianceAccessors, size_t index);
};

#endif /* PCL_H_ */
