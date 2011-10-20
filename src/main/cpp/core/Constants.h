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

#include <string>

using std::string;

namespace Constants {

#include "Config.h"

/**
 * Product types.
 */
const string PRODUCT_SY1 = "SY1";
const string PRODUCT_SY2 = "SY2";
const string PRODUCT_VGTP = "VGTP";
const string PRODUCT_VGTS = "VGTS";

/**
 * Segment names.
 */
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
const string SEGMENT_SYN_AVERAGED = "SYN_AVERAGED";

/**
 * Dimension names.
 */
const string DIMENSION_N_CAM = "N_CAM";
const string DIMENSION_N_DET_CAM = "N_DET_CAM";
const string DIMENSION_N_LINE_OLC = "N_LINE_OLC";

/**
 * Dimension sizes.
 */
const long N_CAM = 5;
const long N_DET_CAM = 740;

/**
 * Data types.
 */
const int TYPE_BYTE = 1;
const int TYPE_CHAR = 2;
const int TYPE_SHORT = 3;
const int TYPE_INT = 4;
const int TYPE_FLOAT = 5;
const int TYPE_DOUBLE = 6;
const int TYPE_UBYTE = 7;
const int TYPE_USHORT = 8;
const int TYPE_UINT = 9;
const int TYPE_LONG = 10;
const int TYPE_ULONG = 11;
const int TYPE_STRING = 12;

/**
 * Log information
 */
const string LOG_LEVEL_INFO = "INFO";
const string LOG_LEVEL_PROGRESS = "PROGRESS";
const string LOG_LEVEL_DEBUG = "DEBUG";
const string LOG_LEVEL_WARNING = "WARNING";
const string LOG_LEVEL_ERROR = "ERROR";

/**
 * Auxdata information.
 */

const string AUXDATA_CONFIGURATION_ID = "SYCPAX";
const string AUXDATA_RADIOMETRIC_ID = "SYRTAX";

}

#endif	/* CONSTANTS_H */

