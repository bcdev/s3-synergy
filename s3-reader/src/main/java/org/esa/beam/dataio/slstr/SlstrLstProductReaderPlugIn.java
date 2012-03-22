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

import org.esa.beam.dataio.manifest.ManifestProductReaderPlugIn;
import org.esa.beam.framework.dataio.ProductReader;

/**
 * PlugIn class which provides a Sentinel 3 SLSTR LST product reader to the framework.
 *
 * @author Olaf Danne
 * @author Ralf Quast
 * @since 1.0
 */
public class SlstrLstProductReaderPlugIn extends ManifestProductReaderPlugIn {

    static final String FORMAT_NAME_SLSTR_LST = "SLSTR-L2-LST";

    public SlstrLstProductReaderPlugIn() {
        super(FORMAT_NAME_SLSTR_LST, "S3.?_SL_2_LST..._.*.SAFE", "Sentinel-3 SLSTR Level-2 LST product");
    }

    @Override
    public ProductReader createReaderInstance() {
        return new SlstrLstProductReader(this);
    }
}

