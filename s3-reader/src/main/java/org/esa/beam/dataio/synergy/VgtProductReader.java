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

import com.bc.ceres.core.ProgressMonitor;
import com.bc.ceres.glevel.MultiLevelImage;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductIO;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.FlagCoding;
import org.esa.beam.framework.datamodel.MetadataElement;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.ProductData;
import org.esa.beam.framework.datamodel.TiePointGrid;
import org.esa.beam.util.ProductUtils;
import org.esa.beam.util.io.FileUtils;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Product reader responsible for reading VGT P data products in SAFE format.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class VgtProductReader extends SynProductReader {

    public VgtProductReader(VgtProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
    }

    @Override
    protected void attachAnnotationData(Manifest manifest, Product product) throws IOException {
        if (isVGSProduct()) {

            measurementFileNames.addAll(manifest.getGeometryFileNames());
            measurementFileNames.add(manifest.getTimeFileName());
        }
        measurementProducts = loadDataProducts(measurementFileNames);
        final List<String> tiepointFileNames = manifest.getTiePointFileNames();
        if (isVGPProduct()) {
            tiepointFileNames.addAll(manifest.getGeometryFileNames());
        }
        tiepointProducts = loadDataProducts(tiepointFileNames);

        attachAnnotationData(tiepointProducts, product);
    }

    private boolean isMeasurementBandElement(MetadataElement element) {
        return (!element.getName().startsWith("lat") &&
                !element.getName().startsWith("lon") &&
                !element.getName().equals("crs"));
    }

    private void attachAnnotationData(List<Product> tiepointsProducts, Product product) {
        attachTiePointsToProduct(tiepointsProducts, product);
    }

    private void attachTiePointsToProduct(List<Product> tiePointsProducts, Product product) {
        for (Product tpProduct : tiePointsProducts) {
            final int subsampling = 8;
            final Band tpBand = tpProduct.getBandAt(0);
            final MultiLevelImage sourceImage = tpBand.getGeophysicalImage();
            final int width = sourceImage.getWidth();
            final int height = sourceImage.getHeight();
            final float[] tiePointData = new float[width * height];
            sourceImage.getData().getSamples(0, 0, width, height, 0, tiePointData);
            TiePointGrid tiePointGrid = new TiePointGrid(tpBand.getName(), tpBand.getRasterWidth(),
                                                         tpBand.getRasterHeight(), 0, 0, subsampling,
                                                         subsampling, tiePointData, true);
            setTiePointScalingFactor(tiePointGrid);
            product.addTiePointGrid(tiePointGrid);
        }
    }

    private boolean isVGSProduct() {
        return getManifest().getDescription().contains("VGT S");
    }

    private boolean isVGPProduct() {
        return getManifest().getDescription().contains("VGT P");
    }

    private void setMeasurementBandScalingFactor(Band band) {
        if (band.getName().equals("ndvi")) {
            band.setScalingFactor(0.004);
        } else if (isSpectralBand(band)) {
            band.setScalingFactor(0.004);
        }
    }

    private boolean isSpectralBand(Band band) {
        return band.getName().equals("b0") ||
               band.getName().equals("b2") ||
               band.getName().equals("b3") ||
               band.getName().equals("mir");
    }

    private void setTiePointScalingFactor(TiePointGrid tpg) {
        if (tpg.getName().equals("ag")) {
            tpg.setScalingFactor(0.004);
        } else if (tpg.getName().equals("og")) {
            tpg.setScalingFactor(0.004);
        } else if (tpg.getName().equals("wvg")) {
            tpg.setScalingFactor(0.04);
        }
    }
}
