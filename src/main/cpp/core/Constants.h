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

#include <boost/cstdint.hpp>
#include <limits>
#include <string>

using std::numeric_limits;
using std::string;

using boost::int8_t;
using boost::int16_t;
using boost::int32_t;
using boost::int64_t;
using boost::uint8_t;
using boost::uint16_t;
using boost::uint32_t;
using boost::uint64_t;

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
const string AUX_ID_SYCPAX = "SYCPAX";
const string AUX_ID_SYRTAX = "SYRTAX";

/**
 * Flags.
 */
const uint32_t SY1_OLCI_LAND_FLAG = 2147483648U;

const uint8_t SY1_SLSTR_CLOUD_FLAG = 64U;

const uint16_t SY2_CLOUD_FLAG = 1U;
const uint16_t SY2_SNOW_RISK_FLAG = 2U;
const uint16_t SY2_SHADOW_RISK_FLAG = 4U;
const uint16_t SY2_CLOUD_FILLED_FLAG = 8U;
const uint16_t SY2_LAND_FLAG = 16U;
const uint16_t SY2_NO_OLC_FLAG = 32U;
const uint16_t SY2_NO_SLN_FLAG = 64U;
const uint16_t SY2_NO_SLO_FLAG = 128U;
const uint16_t SY2_PARTLY_CLOUDY_FLAG = 256U;
const uint16_t SY2_PARTLY_WATER_FLAG = 512U;
const uint16_t SY2_BORDER_FLAG = 1024U;
const uint16_t SY2_AEROSOL_FILLED_FLAG = 2048U;
const uint16_t SY2_AEROSOL_SUCCESS_FLAG = 4096U;
const uint16_t SY2_AEROSOL_NEGATIVE_CURVATURE_FLAG = 8192U;
const uint16_t SY2_AEROSOL_TOO_LOW_FLAG = 16384U;
const uint16_t SY2_AEROSOL_HIGH_ERROR_FLAG = 32768U;
const uint16_t SY2_AVERAGE_FLAG_MASK = SY2_PARTLY_CLOUDY_FLAG | SY2_PARTLY_WATER_FLAG | SY2_BORDER_FLAG | SY2_AEROSOL_FILLED_FLAG | SY2_AEROSOL_HIGH_ERROR_FLAG | SY2_AEROSOL_NEGATIVE_CURVATURE_FLAG | SY2_AEROSOL_SUCCESS_FLAG | SY2_AEROSOL_TOO_LOW_FLAG;

const double FILL_VALUE_DOUBLE = -numeric_limits<double>::max();
}

#endif	/* CONSTANTS_H */

