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
#include <netcdf.h>

using std::string;

namespace Constants {
    const size_t N_CAM = 5;
    const size_t N_DET_CAM = 760;
    const size_t N_LINE_OLC = 60000;

    const string PROCESSOR_VERSION = "1.0-SNAPSHOT";

    const string PRODUCT_SY1 = "SY1";
    const string PRODUCT_SY2 = "SY2";
    const string PRODUCT_VGTP = "VGTP";
    const string PRODUCT_VGTS = "VGTS";

    const string SEGMENT_OLC = "OLC";
    const string SEGMENT_OLC_TP = "OLC_TP";
    const string SEGMENT_OLC_INFO = "OLC_INFO";
    const string SEGMENT_OLC_TIME = "OLC_TIME";
    const string SEGMENT_SLN = "SLN";
    const string SEGMENT_SLO = "SLO";
    const string SEGMENT_SLN_TP = "SLN_TP";
    const string SEGMENT_SLO_TP = "SLO_TP";
    const string SEGMENT_SLN_INFO = "SLN_INFO";
    const string SEGMENT_SLO_INFO = "SLO_INFO";
    const string SEGMENT_SYN_COLLOCATED = "SYN_COLLOCATED";

    const string DIMENSION_N_CAM = "N_CAM";
    const string DIMENSION_N_DET_CAM = "N_DET_CAM";
    const string DIMENSION_N_LINE_OLC = "N_LINE_OLC";

    const int TYPE_BYTE = NC_BYTE;
    const int TYPE_INT = NC_INT;
    const int TYPE_LONG = NC_INT64;
    const int TYPE_SHORT = NC_SHORT;
    const int TYPE_FLOAT = NC_FLOAT;
    const int TYPE_DOUBLE = NC_DOUBLE;
    const int TYPE_UBYTE = NC_UBYTE;
    const int TYPE_UINT = NC_UINT;
    const int TYPE_ULONG = NC_UINT64;
    const int TYPE_USHORT = NC_USHORT;
    const int TYPE_CHAR = NC_CHAR;
    const int TYPE_STRING = NC_STRING;
}

#endif	/* CONSTANTS_H */

