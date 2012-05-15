/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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
 */

/**
 * @file A file defining global constants.
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

#include <limits>
#include <string>

#include "Types.h"

using std::numeric_limits;
using std::string;

/**
 * The namespace for all global constants.
 */
namespace Constants {

#include "Config.h"

/**
 * The Synergy Level 1 product type.
 */
const string PRODUCT_SY1 = "SY1";

/**
 * The Synergy Level 2 product type.
 */
const string PRODUCT_SY2 = "SY2";

/**
 * The VGT-P product type.
 */
const string PRODUCT_VGP = "VGP";

/**
 * The VGT-S product type.
 */
const string PRODUCT_VGS = "VGS";

/**
 * The name of the geo-location data segment.
 */
const string SEGMENT_GEO = "GEO";

/**
 * The name of the OLCI data segment.
 */
const string SEGMENT_OLC = "OLC";

/**
 * The name of the OLCI tie-point data segment.
 */
const string SEGMENT_OLC_TP = "OLC_TP";

/**
 * The name of the OLCI general information data segment.
 */
const string SEGMENT_OLC_INFO = "OLC_INFO";

/**
 * The name of the OLCI time data segment.
 */
const string SEGMENT_OLC_TIME = "OLC_TIME";

/**
 * The name of the SLSTR nadir-view data segment.
 */
const string SEGMENT_SLN = "SLN";

/**
 * The name of the SLSTR oblique-view data segment.
 */
const string SEGMENT_SLO = "SLO";

/**
 * The name of the SLSTR nadir-view tie-pint data segment.
 */
const string SEGMENT_SLN_TP = "SLN_TP";

/**
 * The name of the SLSTR oblique-view tie-point data segment.
 */
const string SEGMENT_SLO_TP = "SLO_TP";

/**
 * The name of the SLSTR nadir-view general information data segment.
 */
const string SEGMENT_SLN_INFO = "SLN_INFO";

/**
 * The name of the SLSTR oblique-view general information data segment.
 */
const string SEGMENT_SLO_INFO = "SLO_INFO";

/**
 * The name of the SYN collocated data segment.
 */
const string SEGMENT_SYN_COLLOCATED = "SYN_COLLOCATED";

/**
 * The name of the SYN averaged data segment.
 */
const string SEGMENT_SYN_AVERAGED = "SYN_AVERAGED";

/**
 * The name of the VGT data segment.
 */
const string SEGMENT_VGT = "VGT";

/**
 * The name of the VGT tie-point data segment.
 */
const string SEGMENT_VGT_TP = "VGT_TP";

/**
 * The name of the VGT latitude data segment.
 */
const string SEGMENT_VGT_LAT = "VGT_LAT";

/**
 * The name of the VGT longitude data segment.
 */
const string SEGMENT_VGT_LON = "VGT_LON";

/**
 * The name of the VGT latitude tie-point data segment.
 */
const string SEGMENT_VGT_LAT_TP = "VGT_LAT_TP";

/**
 * The name of the VGT longitude tie-point data segment.
 */
const string SEGMENT_VGT_LON_TP = "VGT_LON_TP";

/**
 * The name of the VGT latitude bounds data segment.
 */
const string SEGMENT_VGT_LAT_BNDS = "VGT_LAT_BNDS";

/**
 * The name of the VGT longitude bounds data segment.
 */
const string SEGMENT_VGT_LON_BNDS = "VGT_LON_BNDS";

/**
 * The name of the VGT latitude tie-point bounds data segment.
 */
const string SEGMENT_VGT_LAT_TP_BNDS = "VGT_LAT_TP_BNDS";

/**
 * The name of the VGT longitude tie-point bounds data segment.
 */
const string SEGMENT_VGT_LON_TP_BNDS = "VGT_LON_TP_BNDS";

/**
 * The name of the OLCI camera dimension.
 */
const string DIMENSION_N_CAM = "N_CAM";

/**
 * The name of the OLCI column dimension.
 */
const string DIMENSION_N_DET_CAM = "N_DET_CAM";

/**
 * The name of the OLCI row dimension.
 */
const string DIMENSION_N_LINE_OLC = "N_LINE_OLC";

/**
 * The size of the OLCI camera dimension.
 */
const long N_CAM = 5;

/**
 * The size of the OLCI column dimension.
 */
const long N_DET_CAM = 740;

/**
 * The code for the 'byte' data type.
 */
const int TYPE_BYTE = 1;

/**
 * The code for the 'character' data type.
 */
const int TYPE_CHAR = 2;

/**
 * The code for the 'short' data type.
 */
const int TYPE_SHORT = 3;

/**
 * The code for the 'int' data type.
 */
const int TYPE_INT = 4;

/**
 * The code for the 'float' data type.
 */
const int TYPE_FLOAT = 5;

/**
 * The code for the 'double' data type.
 */
const int TYPE_DOUBLE = 6;

/**
 * The code for the unsigned 'byte' data type.
 */
const int TYPE_UBYTE = 7;

/**
 * The code for the unsigned 'short' data type.
 */
const int TYPE_USHORT = 8;

/**
 * The code for the unsigned 'int' data type.
 */
const int TYPE_UINT = 9;

/**
 * The code for the 'long' data type.
 */
const int TYPE_LONG = 10;

/**
 * The code for the unsigned 'long' data type.
 */
const int TYPE_ULONG = 11;

/**
 * The code for the 'string' data type.
 */
const int TYPE_STRING = 12;

/**
 * The name of the 'info' log level.
 */
const string LOG_LEVEL_INFO = "INFO";

/**
 * The name of the 'progress' log level.
 */
const string LOG_LEVEL_PROGRESS = "PROGRESS";

/**
 * The name of the 'debug' log level.
 */
const string LOG_LEVEL_DEBUG = "DEBUG";

/**
 * The name of the 'warning' log level.
 */
const string LOG_LEVEL_WARNING = "WARNING";

/**
 * The name of the 'error' log level.
 */
const string LOG_LEVEL_ERROR = "ERROR";

/**
 * The ID of the SYN L2 configuration auxiliary dataset.
 */
const string AUX_ID_SYCPAX = "SYCPAX";

/**
 * The ID of the SYN L2 radiative transfer auxiliary dataset.
 */
const string AUX_ID_SYRTAX = "SYRTAX";

/**
 * The ID of the VGT-P configuration auxiliary dataset.
 */
const string AUX_ID_VPCPAX = "VPCPAX";

/**
 * The ID of the VGT-P spectral response auxiliary dataset.
 */
const string AUX_ID_VPSRAX = "VPSRAX";

/**
 * The ID of the VGT-P radiative transfer auxiliary dataset.
 */
const string AUX_ID_VPRTAX = "VPRTAX";

/**
 * The ID of the VGT-S configuration auxiliary dataset.
 */
const string AUX_ID_VSCPAX = "VSCPAX";

/**
 * The ID of the VGT-S radiative transfer auxiliary dataset.
 */
const string AUX_ID_VSRTAX = "VSRTAX";

/**
 * The mask for the SY1 OLCI land flag.
 */
const uint32_t SY1_OLCI_LAND_FLAG = 2147483648U;

/**
 * The mask for the SY1 SLSTR cloud summary flag.
 */
const uint8_t SY1_SLSTR_CLOUD_FLAG = 64U;

/**
 * The mask for the SY2 cloud flag.
 */
const uint16_t SY2_CLOUD_FLAG = 1U;

/**
 * The mask for the SY2 snow-risk flag.
 */
const uint16_t SY2_SNOW_RISK_FLAG = 2U;

/**
 * The mask for the SY2 shadow-risk flag.
 */
const uint16_t SY2_SHADOW_RISK_FLAG = 4U;

/**
 * The mask for the SY2 cloud-filled flag.
 */
const uint16_t SY2_CLOUD_FILLED_FLAG = 8U;

/**
 * The mask for the SY2 land flag.
 */
const uint16_t SY2_LAND_FLAG = 16U;

/**
 * The mask for the SY2 no-OLC flag.
 */
const uint16_t SY2_NO_OLC_FLAG = 32U;

/**
 * The mask for the SY2 no-SLN flag.
 */
const uint16_t SY2_NO_SLN_FLAG = 64U;

/**
 * The mask for the SY2 no-SLO flag.
 */
const uint16_t SY2_NO_SLO_FLAG = 128U;

/**
 * The mask for the SY2 partly-cloudy flag.
 */
const uint16_t SY2_PARTLY_CLOUDY_FLAG = 256U;

/**
 * The mask for the SY2 partly-water flag.
 */
const uint16_t SY2_PARTLY_WATER_FLAG = 512U;

/**
 * The mask for the SY2 border flag.
 */
const uint16_t SY2_BORDER_FLAG = 1024U;

/**
 * The mask for the SY2 aerosol-filled flag.
 */
const uint16_t SY2_AEROSOL_FILLED_FLAG = 2048U;

/**
 * The mask for the SY2 aerosol-success flag.
 */
const uint16_t SY2_AEROSOL_SUCCESS_FLAG = 4096U;

/**
 * The mask for the SY2 aerosol-negative-curvature flag.
 */
const uint16_t SY2_AEROSOL_NEGATIVE_CURVATURE_FLAG = 8192U;

/**
 * The mask for the SY2 aerosol-too-low flag.
 */
const uint16_t SY2_AEROSOL_TOO_LOW_FLAG = 16384U;

/**
 * The mask for the SY2 aerosol-high-error flag.
 */
const uint16_t SY2_AEROSOL_HIGH_ERROR_FLAG = 32768U;

/**
 * The mask for any flags set by the AVE module.
 */
const uint16_t SY2_AVERAGE_FLAGS_MASK = SY2_PARTLY_CLOUDY_FLAG | SY2_PARTLY_WATER_FLAG | SY2_BORDER_FLAG | SY2_AEROSOL_FILLED_FLAG | SY2_AEROSOL_HIGH_ERROR_FLAG | SY2_AEROSOL_NEGATIVE_CURVATURE_FLAG | SY2_AEROSOL_SUCCESS_FLAG | SY2_AEROSOL_TOO_LOW_FLAG;

/**
 * The 'clear sky' value of the VGT cloud flags.
 */
const uint8_t VGT_CLEAR_VALUE = 252U;

/**
 * The 'cloud shadow' value of the VGT cloud flags.
 */
const uint8_t VGT_SHADOW_VALUE = 1U;

/**
 * The 'uncertain' value of the VGT cloud flags.
 */
const uint8_t VGT_UNCERTAIN_VALUE = 2U;

/**
 * The 'cloudy' value of the VGT cloud flags.
 */
const uint8_t VGT_CLOUD_VALUE = 3U;

/**
 * The mask for the VGT ice or snow flag.
 */
const uint8_t VGT_ICE_OR_SNOW_FLAG = 4U;

/**
 * The mask for the VGT land flag.
 */
const uint8_t VGT_LAND_FLAG = 8U;

/**
 * The mask for the VGT MIR-good flag.
 */
const uint8_t VGT_MIR_GOOD_FLAG = 16U;

/**
 * The mask for the VGT B3-good flag.
 */
const uint8_t VGT_B3_GOOD_FLAG = 32U;

/**
 * The mask for the VGT B2-good flag.
 */
const uint8_t VGT_B2_GOOD_FLAG = 64U;

/**
 * The mask for the VGT B0-good flag.
 */
const uint8_t VGT_B0_GOOD_FLAG = 128U;

/**
 * The fill value used for 'double' type variables.
 */
const double FILL_VALUE_DOUBLE = -numeric_limits<double>::max();
}

#endif	/* CONSTANTS_H */

