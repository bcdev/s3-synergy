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
 * File:   Constants.h
 * Author: ralf
 *
 * Created on December 15, 2010, 5:39 PM
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#include <cstddef>
#include <string>

using std::string;

class Constants {
public:
    static const size_t N_CAM = 5;
    static const size_t N_DET_CAM = 760;
    static const size_t N_LINE_OLC = 60000;
    static const char PROCESSOR_VERSION[];

    static const char SYMBOLIC_NAME_SEGMENT_SYN_COLLOCATED[];
    static const char SYMBOLIC_NAME_DIMENSION_N_CAM[];
    static const char SYMBOLIC_NAME_DIMENSION_N_DET_CAM[];
    static const char SYMBOLIC_NAME_DIMENSION_N_LINE_OLC[];

private:
    Constants() {
    }

    ~Constants() {
    }
};

#endif	/* CONSTANTS_H */

