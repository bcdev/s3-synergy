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

import org.esa.beam.dataio.manifest.ManifestProductReaderPlugIn;
import org.esa.beam.framework.dataio.ProductReader;

/**
 * PlugIn class which provides an Sentinel-3 Synergy Level-2 product reader to the framework.
 *
 * @author Olaf Danne
 * @author Ralf Quast
 * @since 1.0
 */
public class SynL2ProductReaderPlugIn extends ManifestProductReaderPlugIn {

    static final String FORMAT_NAME = "S3-SY2";

    public SynL2ProductReaderPlugIn() {
        super(FORMAT_NAME, "S3.?_SY_2_SYN_.*.SAFE", "Sentinel-3 Synergy Level-2 products");
    }

    @Override
    public ProductReader createReaderInstance() {
        return new SynL2ProductReader(this);
    }
}
