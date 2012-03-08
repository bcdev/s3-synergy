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
import org.esa.beam.framework.datamodel.FlagCoding;
import org.esa.beam.framework.datamodel.PixelGeoCoding;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.ProductNodeGroup;

import java.io.IOException;
import java.util.List;

/**
 * Product reader responsible for reading OLCI/SLSTR L2 SYN data products in SAFE format.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class SynL2ProductReader extends SynProductReader {

    private List<Product> measurementProducts;
    private Product geoCoordinatesProduct;

    public SynL2ProductReader(SynL2ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
    }

    @Override
    protected void attachAnnotationData(Manifest manifest, Product product) throws IOException {
        attachGeoCoodinatesToProduct(manifest.getGeoCoordinatesFileName(), product);
        attachTiePointsToProduct(manifest.getTiePointFileNames(), product);
    }

    private void attachTiePointsToProduct(List<String> tiePointsFileNames, Product product) {
        // the tie points are provided on a different grid, so we currently don't use them for the SYN product
        // todo: at least attach TPGs for lat/lon for each camera
    }

    private void attachGeoCoodinatesToProduct(String geoCoordinatesFileName, Product product) throws IOException {
        geoCoordinatesProduct = readProduct(geoCoordinatesFileName);

        for (final Band band : product.getBands()) {
            for (int i = 1; i <= 5; i++) {
                if (band.getName().endsWith("CAM" + i)) {
                    final String latBandName = "latitude_CAM" + i;
                    final String lonBandName = "longitude_CAM" + i;
                    if (geoCoordinatesProduct.containsBand(latBandName) &&
                        geoCoordinatesProduct.containsBand(lonBandName)) {
                        band.setGeoCoding(new PixelGeoCoding(geoCoordinatesProduct.getBand(latBandName),
                                                             geoCoordinatesProduct.getBand(lonBandName),
                                                             null, 5));
                    }
                }
            }
        }
    }
}
