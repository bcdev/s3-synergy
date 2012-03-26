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

package org.esa.beam.dataio.slstr;

import org.esa.beam.framework.datamodel.FlagCoding;
import org.esa.beam.util.BitSetter;

/**
 * Class providing the flag codings in an SLSTR L2 LND_LST or SST_WCT data product.
 * TODO - delete this class, it shall not be used because flags shall be read from input data.
 *
 * @author Olaf Danne
 * @since 1.0
 */
class SlstrFlagCodings {

    protected static final String CLOUD_FLAG_BAND_NAME = "cloud";
    protected static final String BAYES_FLAG_BAND_NAME = "bayes";
    protected static final String POINTING_FLAG_BAND_NAME = "pointing";
    protected static final String CONFIDENCE_FLAG_BAND_NAME = "confidence";

    private static final int CLOUD_TBC_0 = 0;
    private static final int CLOUD_TBC_1 = 1;
    private static final int CLOUD_TBC_2 = 2;
    private static final int CLOUD_TBC_3 = 3;
    private static final int CLOUD_TBC_4 = 4;
    private static final int CLOUD_TBC_5 = 5;
    private static final int CLOUD_TBC_6 = 6;
    private static final int CLOUD_TBC_7 = 7;
    private static final int CLOUD_TBC_8 = 8;
    private static final int CLOUD_TBC_9 = 9;
    private static final int CLOUD_TBC_10 = 10;
    private static final int CLOUD_TBC_11 = 11;
    private static final int CLOUD_TBC_12 = 12;

    private static final int BAYES_TBC_0 = 0;
    private static final int BAYES_TBC_1 = 1;
    private static final int BAYES_TBC_2 = 2;
    private static final int BAYES_TBC_3 = 3;

    private static final int POINTING_FLIP_MIRROR_ABSOLUTE_ERROR = 0;
    private static final int POINTING_FLIP_MIRROR_INTEGRATED_ERROR = 1;
    private static final int POINTING_FLIP_MIRROR_RMS_ERROR = 2;
    private static final int POINTING_SCAN_MIRROR_ABSOLUTE_ERROR = 3;
    private static final int POINTING_SCAN_MIRROR_INTEGRATED_ERROR = 4;
    private static final int POINTING_SCAN_MIRROR_RMS_ERROR = 5;
    private static final int POINTING_SCAN_TIME_ERROR = 6;
    private static final int POINTING_PLATFORM_MODE = 7;

    private static final int CONFIDENCE_COASTLINE = 0;
    private static final int CONFIDENCE_OCEAN = 1;
    private static final int CONFIDENCE_TIDAL = 2;
    private static final int CONFIDENCE_LAND = 3;
    private static final int CONFIDENCE_INLAND_WATER = 4;
    private static final int CONFIDENCE_COSMETIC = 5;
    private static final int CONFIDENCE_DUPLICATE = 6;
    private static final int CONFIDENCE_DAY = 7;
    private static final int CONFIDENCE_TWILIGHT = 8;
    private static final int CONFIDENCE_SUN_GLINT = 9;
    private static final int CONFIDENCE_SNOW = 10;
    private static final int CONFIDENCE_SUMMARY_CLOUD = 11;
    private static final int CONFIDENCE_SUMMARY_POINTING = 12;


    protected static FlagCoding createCloudFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(CLOUD_FLAG_BAND_NAME);
        flagCoding.addFlag("CLOUD_TBC_0", BitSetter.setFlag(0, CLOUD_TBC_0), null);
        flagCoding.addFlag("CLOUD_TBC_1", BitSetter.setFlag(0, CLOUD_TBC_1), null);
        flagCoding.addFlag("CLOUD_TBC_2", BitSetter.setFlag(0, CLOUD_TBC_2), null);
        flagCoding.addFlag("CLOUD_TBC_3", BitSetter.setFlag(0, CLOUD_TBC_3), null);
        flagCoding.addFlag("CLOUD_TBC_4", BitSetter.setFlag(0, CLOUD_TBC_4), null);
        flagCoding.addFlag("CLOUD_TBC_5", BitSetter.setFlag(0, CLOUD_TBC_5), null);
        flagCoding.addFlag("CLOUD_TBC_6", BitSetter.setFlag(0, CLOUD_TBC_6), null);
        flagCoding.addFlag("CLOUD_TBC_7", BitSetter.setFlag(0, CLOUD_TBC_7), null);
        flagCoding.addFlag("CLOUD_TBC_8", BitSetter.setFlag(0, CLOUD_TBC_8), null);
        flagCoding.addFlag("CLOUD_TBC_9", BitSetter.setFlag(0, CLOUD_TBC_9), null);
        flagCoding.addFlag("CLOUD_TBC_10", BitSetter.setFlag(0, CLOUD_TBC_10), null);
        flagCoding.addFlag("CLOUD_TBC_11", BitSetter.setFlag(0, CLOUD_TBC_11), null);
        flagCoding.addFlag("CLOUD_TBC_12", BitSetter.setFlag(0, CLOUD_TBC_12), null);

        return flagCoding;
    }

    protected static FlagCoding createBayesFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(BAYES_FLAG_BAND_NAME);
        flagCoding.addFlag("BAYES_TBC_0", BitSetter.setFlag(0, BAYES_TBC_0), null);
        flagCoding.addFlag("BAYES_TBC_1", BitSetter.setFlag(0, BAYES_TBC_1), null);
        flagCoding.addFlag("BAYES_TBC_2", BitSetter.setFlag(0, BAYES_TBC_2), null);
        flagCoding.addFlag("BAYES_TBC_3", BitSetter.setFlag(0, BAYES_TBC_3), null);

        return flagCoding;
    }

    protected static FlagCoding createPointingFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(POINTING_FLAG_BAND_NAME);
        flagCoding.addFlag("POINTING_FLIP_MIRROR_ABSOLUTE_ERROR",
                           BitSetter.setFlag(0, POINTING_FLIP_MIRROR_ABSOLUTE_ERROR), null);
        flagCoding.addFlag("POINTING_FLIP_MIRROR_INTEGRATED_ERROR",
                           BitSetter.setFlag(0, POINTING_FLIP_MIRROR_INTEGRATED_ERROR), null);
        flagCoding.addFlag("POINTING_FLIP_MIRROR_RMS_ERROR", BitSetter.setFlag(0, POINTING_FLIP_MIRROR_RMS_ERROR),
                           null);
        flagCoding.addFlag("POINTING_SCAN_MIRROR_ABSOLUTE_ERROR",
                           BitSetter.setFlag(0, POINTING_SCAN_MIRROR_ABSOLUTE_ERROR), null);
        flagCoding.addFlag("POINTING_SCAN_MIRROR_INTEGRATED_ERROR",
                           BitSetter.setFlag(0, POINTING_SCAN_MIRROR_INTEGRATED_ERROR), null);
        flagCoding.addFlag("POINTING_SCAN_MIRROR_RMS_ERROR", BitSetter.setFlag(0, POINTING_SCAN_MIRROR_RMS_ERROR),
                           null);
        flagCoding.addFlag("POINTING_SCAN_TIME_ERROR", BitSetter.setFlag(0, POINTING_SCAN_TIME_ERROR), null);
        flagCoding.addFlag("POINTING_PLATFORM_MODE", BitSetter.setFlag(0, POINTING_PLATFORM_MODE), null);

        return flagCoding;
    }

    protected static FlagCoding createConfidenceFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(CONFIDENCE_FLAG_BAND_NAME);
        flagCoding.addFlag("CONFIDENCE_COASTLINE", BitSetter.setFlag(0, CONFIDENCE_COASTLINE), null);
        flagCoding.addFlag("CONFIDENCE_OCEAN", BitSetter.setFlag(0, CONFIDENCE_OCEAN), null);
        flagCoding.addFlag("CONFIDENCE_TIDAL", BitSetter.setFlag(0, CONFIDENCE_TIDAL), null);
        flagCoding.addFlag("CONFIDENCE_LAND", BitSetter.setFlag(0, CONFIDENCE_LAND), null);
        flagCoding.addFlag("CONFIDENCE_INLAND_WATER", BitSetter.setFlag(0, CONFIDENCE_INLAND_WATER), null);
        flagCoding.addFlag("CONFIDENCE_COSMETIC", BitSetter.setFlag(0, CONFIDENCE_COSMETIC), null);
        flagCoding.addFlag("CONFIDENCE_DUPLICATE", BitSetter.setFlag(0, CONFIDENCE_DUPLICATE), null);
        flagCoding.addFlag("CONFIDENCE_DAY", BitSetter.setFlag(0, CONFIDENCE_DAY), null);
        flagCoding.addFlag("CONFIDENCE_TWILIGHT", BitSetter.setFlag(0, CONFIDENCE_TWILIGHT), null);
        flagCoding.addFlag("CONFIDENCE_SUN_GLINT", BitSetter.setFlag(0, CONFIDENCE_SUN_GLINT), null);
        flagCoding.addFlag("CONFIDENCE_SNOW", BitSetter.setFlag(0, CONFIDENCE_SNOW), null);
        flagCoding.addFlag("CONFIDENCE_SUMMARY_CLOUD", BitSetter.setFlag(0, CONFIDENCE_SUMMARY_CLOUD), null);
        flagCoding.addFlag("CONFIDENCE_SUMMARY_POINTING", BitSetter.setFlag(0, CONFIDENCE_SUMMARY_POINTING), null);

        return flagCoding;
    }
}
