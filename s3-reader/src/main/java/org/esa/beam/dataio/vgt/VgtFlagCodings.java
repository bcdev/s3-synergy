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

package org.esa.beam.dataio.vgt;

import org.esa.beam.framework.datamodel.FlagCoding;
import org.esa.beam.util.BitSetter;

/**
 * Class providing the flag codings in a VGT P data product.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class VgtFlagCodings {

    public static final String VGTP_SM_FLAG_BAND_NAME = "SM";

    private static final int VGTP_CLOUD_1 = 0;
    private static final int VGTP_CLOUD_2 = 1;
    private static final int VGTP_ICE_SNOW = 2;
    private static final int VGTP_LAND = 3;
    private static final int VGTP_MIR_GOOD = 4;
    private static final int VGTP_B3_GOOD = 5;
    private static final int VGTP_B2_GOOD = 6;
    private static final int VGTP_B0_GOOD = 7;

    public static FlagCoding createSmFlagCoding() {
        FlagCoding flagCoding = new FlagCoding(VGTP_SM_FLAG_BAND_NAME);
        flagCoding.addFlag("VGTP_CLOUD_1", BitSetter.setFlag(0, VGTP_CLOUD_1), null);
        flagCoding.addFlag("VGTP_CLOUD_2", BitSetter.setFlag(0, VGTP_CLOUD_2), null);
        flagCoding.addFlag("VGTP_ICE_SNOW", BitSetter.setFlag(0, VGTP_ICE_SNOW), null);
        flagCoding.addFlag("VGTP_LAND", BitSetter.setFlag(0, VGTP_LAND), null);
        flagCoding.addFlag("MIR_GOOD", BitSetter.setFlag(0, VGTP_MIR_GOOD), null);
        flagCoding.addFlag("B3_GOOD", BitSetter.setFlag(0, VGTP_B3_GOOD), null);
        flagCoding.addFlag("B2_GOOD", BitSetter.setFlag(0, VGTP_B2_GOOD), null);
        flagCoding.addFlag("B0_GOOD", BitSetter.setFlag(0, VGTP_B0_GOOD), null);

        return flagCoding;
    }
}
