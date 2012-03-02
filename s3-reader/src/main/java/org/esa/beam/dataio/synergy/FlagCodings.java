/*
 * Copyright (c) 2012. Brockmann Consult (info@brockmann-consult.de)
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

package org.esa.beam.dataio.synergy;

import org.esa.beam.framework.datamodel.FlagCoding;
import org.esa.beam.util.BitSetter;

/**
 * Class providing the flag codings in Sentinel-3 Synergy data products.
 *
 * @author Olaf Danne
 * @author Ralf Quast
 * @since 1.0
 */
class FlagCodings {

    private FlagCodings() {
    }

    static final String SY2_FLAG_BAND_NAME = "SYN_flags";
    static final String OLCI_FLAG_BAND_NAME = "OLC_flags";
    static final String SLSTR_NADIR_FLAG_BAND_NAME = "SLN_flags";
    static final String SLSTR_OBLIQUE_FLAG_BAND_NAME = "SLO_flags";
    static final String VGT_STATUS_MASK_BAND_NAME = "SM";


    private static final int SY2_CLOUD = 0;
    private static final int SY2_SNOW_RISK = 1;
    private static final int SY2_SHADOW_RISK = 2;
    private static final int SY2_CLOUD_FILLED = 3;
    private static final int SY2_LAND = 4;
    private static final int SY2_NO_OLC = 5;
    private static final int SY2_NO_SLN = 6;
    private static final int SY2_NO_SLO = 7;
    private static final int SY2_PARTLY_CLOUDY = 8;
    private static final int SY2_PARTLY_WATER = 9;
    private static final int SY2_BORDER = 10;
    private static final int SY2_AEROSOL_FILLED = 11;
    private static final int SY2_SUCCESS = 12;
    private static final int SY2_NEGATIVE_CURVATURE = 13;
    private static final int SY2_TOO_LOW = 14;
    private static final int SY2_HIGH_ERROR = 15;

    private static final int OLC_SUMMARY_SATURATED = 0;
    private static final int OLC_DUBIOUS = 1;
    private static final int OLC_SUN_GLINT_RISK = 2;
    private static final int OLC_COSMETIC = 3;
    private static final int OLC_INVALID = 4;
    private static final int OLC_BRIGHT = 5;
    private static final int OLC_TIDAL_REGION = 6;
    private static final int OLC_FRESH_INLAND_WATER = 7;
    private static final int OLC_COASTLINE = 8;
    private static final int OLC_LAND = 9;

    private static final int SLN_SUMMARY_ISP_ABSENT = 0;
    private static final int SLN_SUMMARY_PIXEL_ABSENT = 1;
    private static final int SLN_SUMMARY_NOT_DECOMPRESSED = 2;
    private static final int SLN_SUMMARY_NO_SIGNAL = 3;
    private static final int SLN_SUMMARY_SATURATION = 4;
    private static final int SLN_SUMMARY_INVALID_RADIANCE = 5;
    private static final int SLN_SUMMARY_NO_PARAMETERS = 6;
    private static final int SLN_SUMMARY_UNFILLED_PIXEL = 7;
    private static final int SLN_COASTLINE = 8;
    private static final int SLN_OCEAN = 9;
    private static final int SLN_TIDAL = 10;
    private static final int SLN_LAND = 11;
    private static final int SLN_INLAND_WATER = 12;
    private static final int SLN_COSMETIC = 13;
    private static final int SLN_DAY = 14;
    private static final int SLN_TWILIGHT = 15;
    private static final int SLN_SUN_GLINT = 16;
    private static final int SLN_SNOW = 17;
    private static final int SLN_SUMMARY_CLOUD = 18;
    private static final int SLN_SUMMARY_POINTING = 19;

    private static final int SLO_SUMMARY_ISP_ABSENT = 0;
    private static final int SLO_SUMMARY_PIXEL_ABSENT = 1;
    private static final int SLO_SUMMARY_NOT_DECOMPRESSED = 2;
    private static final int SLO_SUMMARY_NO_SIGNAL = 3;
    private static final int SLO_SUMMARY_SATURATION = 4;
    private static final int SLO_SUMMARY_INVALID_RADIANCE = 5;
    private static final int SLO_SUMMARY_NO_PARAMETERS = 6;
    private static final int SLO_SUMMARY_UNFILLED_PIXEL = 7;
    private static final int SLO_COASTLINE = 8;
    private static final int SLO_OCEAN = 9;
    private static final int SLO_TIDAL = 10;
    private static final int SLO_LAND = 11;
    private static final int SLO_INLAND_WATER = 12;
    private static final int SLO_COSMETIC = 13;
    private static final int SLO_DAY = 14;
    private static final int SLO_TWILIGHT = 15;
    private static final int SLO_SUN_GLINT = 16;
    private static final int SLO_SNOW = 17;
    private static final int SLO_SUMMARY_CLOUD = 18;
    private static final int SLO_SUMMARY_POINTING = 19;

    private static final int VGT_CLOUD_1 = 0;
    private static final int VGT_CLOUD_2 = 1;
    private static final int VGT_ICE_SNOW = 2;
    private static final int VGT_LAND = 3;
    private static final int VGT_MIR_GOOD = 4;
    private static final int VGT_B3_GOOD = 5;
    private static final int VGT_B2_GOOD = 6;
    private static final int VGT_B0_GOOD = 7;


    static FlagCoding createSynL2FlagCoding() {
        FlagCoding flagCoding = new FlagCoding(SY2_FLAG_BAND_NAME);
        flagCoding.addFlag("SY2_CLOUD", BitSetter.setFlag(0, SY2_CLOUD), null);
        flagCoding.addFlag("SY2_SNOW_RISK", BitSetter.setFlag(0, SY2_SNOW_RISK), null);
        flagCoding.addFlag("SY2_SHADOW_RISK", BitSetter.setFlag(0, SY2_SHADOW_RISK), null);
        flagCoding.addFlag("SY2_CLOUD_FILLED", BitSetter.setFlag(0, SY2_CLOUD_FILLED), null);
        flagCoding.addFlag("SY2_LAND", BitSetter.setFlag(0, SY2_LAND), null);
        flagCoding.addFlag("SY2_NO_OLC", BitSetter.setFlag(0, SY2_NO_OLC), null);
        flagCoding.addFlag("SY2_NO_SLN", BitSetter.setFlag(0, SY2_NO_SLN), null);
        flagCoding.addFlag("SY2_NO_SLO", BitSetter.setFlag(0, SY2_NO_SLO), null);
        flagCoding.addFlag("SY2_PARTLY_CLOUDY", BitSetter.setFlag(0, SY2_PARTLY_CLOUDY), null);
        flagCoding.addFlag("SY2_PARTLY_WATER", BitSetter.setFlag(0, SY2_PARTLY_WATER), null);
        flagCoding.addFlag("SY2_BORDER", BitSetter.setFlag(0, SY2_BORDER), null);
        flagCoding.addFlag("SY2_AEROSOL_FILLED", BitSetter.setFlag(0, SY2_AEROSOL_FILLED), null);
        flagCoding.addFlag("SY2_SUCCESS", BitSetter.setFlag(0, SY2_SUCCESS), null);
        flagCoding.addFlag("SY2_NEGATIVE_CURVATURE", BitSetter.setFlag(0, SY2_NEGATIVE_CURVATURE), null);
        flagCoding.addFlag("SY2_TOO_LOW", BitSetter.setFlag(0, SY2_TOO_LOW), null);
        flagCoding.addFlag("SY2_HIGH_ERROR", BitSetter.setFlag(0, SY2_HIGH_ERROR), null);

        return flagCoding;
    }

    static FlagCoding createOlciFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(OLCI_FLAG_BAND_NAME);
        flagCoding.addFlag("OLC_SUMMARY_SATURATED", BitSetter.setFlag(0, OLC_SUMMARY_SATURATED), null);
        flagCoding.addFlag("OLC_DUBIOUS", BitSetter.setFlag(0, OLC_DUBIOUS), null);
        flagCoding.addFlag("OLC_SUN_GLINT_RISK", BitSetter.setFlag(0, OLC_SUN_GLINT_RISK), null);
        flagCoding.addFlag("OLC_COSMETIC", BitSetter.setFlag(0, OLC_COSMETIC), null);
        flagCoding.addFlag("OLC_INVALID", BitSetter.setFlag(0, OLC_INVALID), null);
        flagCoding.addFlag("OLC_BRIGHT", BitSetter.setFlag(0, OLC_BRIGHT), null);
        flagCoding.addFlag("OLC_TIDAL_REGION", BitSetter.setFlag(0, OLC_TIDAL_REGION), null);
        flagCoding.addFlag("OLC_FRESH_INLAND_WATER", BitSetter.setFlag(0, OLC_FRESH_INLAND_WATER), null);
        flagCoding.addFlag("OLC_COASTLINE", BitSetter.setFlag(0, OLC_COASTLINE), null);
        flagCoding.addFlag("OLC_LAND", BitSetter.setFlag(0, OLC_LAND), null);

        return flagCoding;
    }

    static FlagCoding createSlstrNadirFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(SLSTR_NADIR_FLAG_BAND_NAME);
        flagCoding.addFlag("SLN_SUMMARY_ISP_ABSENT", BitSetter.setFlag(0, SLN_SUMMARY_ISP_ABSENT), null);
        flagCoding.addFlag("SLN_SUMMARY_PIXEL_ABSENT", BitSetter.setFlag(0, SLN_SUMMARY_PIXEL_ABSENT), null);
        flagCoding.addFlag("SLN_SUMMARY_NOT_DECOMPRESSED", BitSetter.setFlag(0, SLN_SUMMARY_NOT_DECOMPRESSED), null);
        flagCoding.addFlag("SLN_SUMMARY_NO_SIGNAL", BitSetter.setFlag(0, SLN_SUMMARY_NO_SIGNAL), null);
        flagCoding.addFlag("SLN_SUMMARY_SATURATION", BitSetter.setFlag(0, SLN_SUMMARY_SATURATION), null);
        flagCoding.addFlag("SLN_SUMMARY_INVALID_RADIANCE", BitSetter.setFlag(0, SLN_SUMMARY_INVALID_RADIANCE), null);
        flagCoding.addFlag("SLN_SUMMARY_NO_PARAMETERS", BitSetter.setFlag(0, SLN_SUMMARY_NO_PARAMETERS), null);
        flagCoding.addFlag("SLN_SUMMARY_UNFILLED_PIXEL", BitSetter.setFlag(0, SLN_SUMMARY_UNFILLED_PIXEL), null);
        flagCoding.addFlag("SLN_COASTLINE", BitSetter.setFlag(0, SLN_COASTLINE), null);
        flagCoding.addFlag("SLN_OCEAN", BitSetter.setFlag(0, SLN_OCEAN), null);
        flagCoding.addFlag("SLN_TIDAL", BitSetter.setFlag(0, SLN_TIDAL), null);
        flagCoding.addFlag("SLN_LAND", BitSetter.setFlag(0, SLN_LAND), null);
        flagCoding.addFlag("SLN_INLAND_WATER", BitSetter.setFlag(0, SLN_INLAND_WATER), null);
        flagCoding.addFlag("SLN_COSMETIC", BitSetter.setFlag(0, SLN_COSMETIC), null);
        flagCoding.addFlag("SLN_DAY", BitSetter.setFlag(0, SLN_DAY), null);
        flagCoding.addFlag("SLN_TWILIGHT", BitSetter.setFlag(0, SLN_TWILIGHT), null);
        flagCoding.addFlag("SLN_SUN_GLINT", BitSetter.setFlag(0, SLN_SUN_GLINT), null);
        flagCoding.addFlag("SLN_SNOW", BitSetter.setFlag(0, SLN_SNOW), null);
        flagCoding.addFlag("SLN_SUMMARY_CLOUD", BitSetter.setFlag(0, SLN_SUMMARY_CLOUD), null);
        flagCoding.addFlag("SLN_SUMMARY_POINTING", BitSetter.setFlag(0, SLN_SUMMARY_POINTING), null);

        return flagCoding;
    }

    static FlagCoding createSlstrObliqueFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(SLSTR_OBLIQUE_FLAG_BAND_NAME);
        flagCoding.addFlag("SLO_SUMMARY_ISP_ABSENT", BitSetter.setFlag(0, SLO_SUMMARY_ISP_ABSENT), null);
        flagCoding.addFlag("SLO_SUMMARY_PIXEL_ABSENT", BitSetter.setFlag(0, SLO_SUMMARY_PIXEL_ABSENT), null);
        flagCoding.addFlag("SLO_SUMMARY_NOT_DECOMPRESSED", BitSetter.setFlag(0, SLO_SUMMARY_NOT_DECOMPRESSED), null);
        flagCoding.addFlag("SLO_SUMMARY_NO_SIGNAL", BitSetter.setFlag(0, SLO_SUMMARY_NO_SIGNAL), null);
        flagCoding.addFlag("SLO_SUMMARY_SATURATION", BitSetter.setFlag(0, SLO_SUMMARY_SATURATION), null);
        flagCoding.addFlag("SLO_SUMMARY_INVALID_RADIANCE", BitSetter.setFlag(0, SLO_SUMMARY_INVALID_RADIANCE), null);
        flagCoding.addFlag("SLO_SUMMARY_NO_PARAMETERS", BitSetter.setFlag(0, SLO_SUMMARY_NO_PARAMETERS), null);
        flagCoding.addFlag("SLO_SUMMARY_UNFILLED_PIXEL", BitSetter.setFlag(0, SLO_SUMMARY_UNFILLED_PIXEL), null);
        flagCoding.addFlag("SLO_COASTLINE", BitSetter.setFlag(0, SLO_COASTLINE), null);
        flagCoding.addFlag("SLO_OCEAN", BitSetter.setFlag(0, SLO_OCEAN), null);
        flagCoding.addFlag("SLO_TIDAL", BitSetter.setFlag(0, SLO_TIDAL), null);
        flagCoding.addFlag("SLO_LAND", BitSetter.setFlag(0, SLO_LAND), null);
        flagCoding.addFlag("SLO_INLAND_WATER", BitSetter.setFlag(0, SLO_INLAND_WATER), null);
        flagCoding.addFlag("SLO_COSMETIC", BitSetter.setFlag(0, SLO_COSMETIC), null);
        flagCoding.addFlag("SLO_DAY", BitSetter.setFlag(0, SLO_DAY), null);
        flagCoding.addFlag("SLO_TWILIGHT", BitSetter.setFlag(0, SLO_TWILIGHT), null);
        flagCoding.addFlag("SLO_SUN_GLINT", BitSetter.setFlag(0, SLO_SUN_GLINT), null);
        flagCoding.addFlag("SLO_SNOW", BitSetter.setFlag(0, SLO_SNOW), null);
        flagCoding.addFlag("SLO_SUMMARY_CLOUD", BitSetter.setFlag(0, SLO_SUMMARY_CLOUD), null);
        flagCoding.addFlag("SLO_SUMMARY_POINTING", BitSetter.setFlag(0, SLO_SUMMARY_POINTING), null);

        return flagCoding;
    }

    static FlagCoding createVgtFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(VGT_STATUS_MASK_BAND_NAME);
        flagCoding.addFlag("VGT_CLOUD_1", BitSetter.setFlag(0, VGT_CLOUD_1), null);
        flagCoding.addFlag("VGT_CLOUD_2", BitSetter.setFlag(0, VGT_CLOUD_2), null);
        flagCoding.addFlag("VGT_ICE_SNOW", BitSetter.setFlag(0, VGT_ICE_SNOW), null);
        flagCoding.addFlag("VGT_LAND", BitSetter.setFlag(0, VGT_LAND), null);
        flagCoding.addFlag("MIR_GOOD", BitSetter.setFlag(0, VGT_MIR_GOOD), null);
        flagCoding.addFlag("B3_GOOD", BitSetter.setFlag(0, VGT_B3_GOOD), null);
        flagCoding.addFlag("B2_GOOD", BitSetter.setFlag(0, VGT_B2_GOOD), null);
        flagCoding.addFlag("B0_GOOD", BitSetter.setFlag(0, VGT_B0_GOOD), null);

        return flagCoding;
    }
}
