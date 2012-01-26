package org.esa.beam.dataio.syn;

import org.esa.beam.framework.datamodel.FlagCoding;
import org.esa.beam.util.BitSetter;

/**
 * Created by IntelliJ IDEA.
 * User: olafd
 * Date: 26.01.12
 * Time: 17:35
 * To change this template use File | Settings | File Templates.
 */
public class SynFlagCodings {

    protected static final String SYN_FLAG_BAND_NAME = "SYN_flags";
    protected static final String OLCI_FLAG_BAND_NAME = "OLC_flags";
    protected static final String SLSTR_NADIR_FLAG_BAND_NAME = "SLN_flags";
    protected static final String SLSTR_OBLIQUE_FLAG_BAND_NAME = "SLO_flags";


    private static final int SYN_CLOUD = 0;
    private static final int SYN_SNOW_RISK = 1;
    private static final int SYN_SHADOW_RISK = 2;
    private static final int SYN_CLOUD_FILLED = 3;
    private static final int SYN_LAND = 4;
    private static final int SYN_NO_OLC = 5;
    private static final int SYN_NO_SLN = 6;
    private static final int SYN_NO_SLO = 7;
    private static final int SYN_PARTLY_CLOUDY = 8;
    private static final int SYN_PARTLY_WATER = 9;
    private static final int SYN_BORDER = 10;
    private static final int SYN_AEROSOL_FILLED = 11;
    private static final int SYN_SUCCESS = 12;
    private static final int SYN_NEGATIVE_CURVATURE = 13;
    private static final int SYN_TOO_LOW = 14;
    private static final int SYN_HIGH_ERROR = 15;

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


    protected static FlagCoding createSynFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(SYN_FLAG_BAND_NAME);
        flagCoding.addFlag("SYN_CLOUD", BitSetter.setFlag(0, SYN_CLOUD), null);
        flagCoding.addFlag("SYN_SNOW_RISK", BitSetter.setFlag(0, SYN_SNOW_RISK), null);
        flagCoding.addFlag("SYN_SHADOW_RISK", BitSetter.setFlag(0, SYN_SHADOW_RISK), null);
        flagCoding.addFlag("SYN_CLOUD_FILLED", BitSetter.setFlag(0, SYN_CLOUD_FILLED), null);
        flagCoding.addFlag("SYN_LAND", BitSetter.setFlag(0, SYN_LAND), null);
        flagCoding.addFlag("SYN_NO_OLC", BitSetter.setFlag(0, SYN_NO_OLC), null);
        flagCoding.addFlag("SYN_NO_SLN", BitSetter.setFlag(0, SYN_NO_SLN), null);
        flagCoding.addFlag("SYN_NO_SLO", BitSetter.setFlag(0, SYN_NO_SLO), null);
        flagCoding.addFlag("SYN_PARTLY_CLOUDY", BitSetter.setFlag(0, SYN_PARTLY_CLOUDY), null);
        flagCoding.addFlag("SYN_PARTLY_WATER", BitSetter.setFlag(0, SYN_PARTLY_WATER), null);
        flagCoding.addFlag("SYN_BORDER", BitSetter.setFlag(0, SYN_BORDER), null);
        flagCoding.addFlag("SYN_AEROSOL_FILLED", BitSetter.setFlag(0, SYN_AEROSOL_FILLED), null);
        flagCoding.addFlag("SYN_SUCCESS", BitSetter.setFlag(0, SYN_SUCCESS), null);
        flagCoding.addFlag("SYN_NEGATIVE_CURVATURE", BitSetter.setFlag(0, SYN_NEGATIVE_CURVATURE), null);
        flagCoding.addFlag("SYN_TOO_LOW", BitSetter.setFlag(0, SYN_TOO_LOW), null);
        flagCoding.addFlag("SYN_HIGH_ERROR", BitSetter.setFlag(0, SYN_HIGH_ERROR), null);

        return flagCoding;
    }

    protected static FlagCoding createOlciFlagCoding() {
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

    protected static FlagCoding createSlstrNadirFlagCoding() {
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

    protected static FlagCoding createSlstrObliqueCoding() {
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

}
