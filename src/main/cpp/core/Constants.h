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

namespace Constants {
    const size_t N_CAM = 5;
    const size_t N_DET_CAM = 760;
    const size_t N_LINE_OLC = 60000;

    const string PROCESSOR_VERSION = "1.0-SNAPSHOT";

    const string SEGMENT_SYN_COLLOCATED = "SYN_COLLOCATED";
    const string SEGMENT_OLC_TIE_POINTS = "OLC_TIE_POINTS";
    const string SEGMENT_SLN_TIE_POINTS = "SLN_TIE_POINTS";
    const string SEGMENT_SLO_TIE_POINTS = "SLO_TIE_POINTS";
    const string SEGMENT_OLC_TIME_STAMPS = "OLC_TIME_STAMPS";
    const string SEGMENT_OLC = "OLC";
    const string SEGMENT_SLO_GEN_INFO = "SLO_GEN_INFO";
    const string SEGMENT_SLN_GEN_INFO = "SLN_GEN_INFO";
    const string SEGMENT_SLO = "SLO";
    const string SEGMENT_SLN = "SLN";

    const string DIMENSION_N_CAM = "N_CAM";
    const string DIMENSION_N_DET_CAM = "N_DET_CAM";
    const string DIMENSION_N_LINE_OLC = "N_LINE_OLC";

    const string PRODUCT_SYN_L2 = "L2";
    const string PRODUCT_L1C = "L1C";
}

#endif	/* CONSTANTS_H */

