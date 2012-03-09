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

import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.GeoCoding;
import org.esa.beam.framework.datamodel.PixelGeoCoding;
import org.esa.beam.framework.datamodel.Product;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * Product reader responsible for reading OLCI/SLSTR L2 SYN data products in SAFE format.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class SynL2ProductReader extends SynProductReader {

    public SynL2ProductReader(SynL2ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
    }

    @Override
    protected List<String> getTiePointFileNames(Manifest manifest) {
        // the tie points are provided on a different grid, so we currently don't use them
        return Collections.emptyList();
    }

    @Override
    protected List<String> getTimeFileNames(Manifest manifest) {
        // the time data are provided on a different grid, so we currently don't use them
        return Collections.emptyList();
    }

    @Override
    protected void attachGeoCoding(Manifest manifest, Product targetProduct) throws IOException {
        final List<GeoCoding> geoCodingList = new ArrayList<GeoCoding>();
        for (int i = 1; i <= 5; i++) {
            final String latBandName = "latitude_CAM" + i;
            final String lonBandName = "longitude_CAM" + i;
            final Band latBand = targetProduct.getBand(latBandName);
            final Band lonBand = targetProduct.getBand(lonBandName);
            final GeoCoding geoCoding = new PixelGeoCoding(latBand, lonBand, null, 5);

            geoCodingList.add(geoCoding);
        }
        for (final Band targetBand : targetProduct.getBands()) {
            for (int i = 1; i <= 5; i++) {
                if (targetBand.getName().contains("CAM" + i)) {
                    targetBand.setGeoCoding(geoCodingList.get(i - 1));
                    break;
                }
            }
        }
    }

    @Override
    protected void attachTiepointData(Band sourceBand, Product targetProduct) {
        // the tie points are provided on a different grid, so we currently don't use them
    }
}
