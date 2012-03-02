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

package org.esa.beam.dataio.syn;

import com.bc.ceres.core.ProgressMonitor;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductIO;
import org.esa.beam.framework.datamodel.*;
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
 * Product reader responsible for reading OLCI/SLSTR L2 SYN data products in SAFE format.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class SynL2ProductReader extends AbstractProductReader {

    private final Logger logger;
    private List<Product> measurementProducts;
    private Product geoCoordinatesProduct;

    public SynL2ProductReader(SynL2ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
        logger = Logger.getLogger(getClass().getSimpleName());
    }

    @Override
    protected Product readProductNodesImpl() throws IOException {
        File inputFile = getInputFile();
        SynL2Manifest manifest = createManifestFile(inputFile);
        return createProduct(manifest);
    }


    @Override
    protected void readBandRasterDataImpl(int sourceOffsetX, int sourceOffsetY, int sourceWidth, int sourceHeight,
                                          int sourceStepX, int sourceStepY, Band destBand, int destOffsetX,
                                          int destOffsetY, int destWidth, int destHeight, ProductData destBuffer,
                                          ProgressMonitor pm) throws IOException {
        throw new IllegalStateException(String.format("No source to read from for band '%s'.", destBand.getName()));
    }

    @Override
    public void close() throws IOException {
        for (Product product : measurementProducts) {
            product.dispose();
        }
        if (geoCoordinatesProduct != null) {
            geoCoordinatesProduct.dispose();
        }
        super.close();
    }

    private Product createProduct(SynL2Manifest manifest) {
        measurementProducts = loadMeasurementProducts(manifest.getMeasurementFileNames());
        int width = measurementProducts.get(0).getSceneRasterWidth();
        int height = measurementProducts.get(0).getSceneRasterHeight();
        Product product = new Product(getProductName(), SynL2ProductReaderPlugIn.FORMAT_NAME_SYN, width,
                                      height, this);
        product.setStartTime(manifest.getStartTime());
        product.setEndTime(manifest.getStopTime());
        product.setFileLocation(getInputFile());
        attachMeasurementBands(measurementProducts, product);
        attachAnnotationData(manifest, product);
        return product;
    }

    private void attachAnnotationData(SynL2Manifest manifest, Product product) {
        attachGeoCoodinatesToProduct(manifest.getGeoCoordinatesFileName(), product);
        attachTiePointsToProduct(manifest.getTiepointsFileNames(), product);
    }

    private void attachTiePointsToProduct(List<String> tiePointsFileNames, Product product) {
        // the tie points are provided on a different grid, so we currently don't use them for the SYN product
        // todo: at least attach TPGs for lat/lon for each camera
    }

    private void attachGeoCoodinatesToProduct(String geoCoordinatesFileName, Product product) {
        try {
            geoCoordinatesProduct = readProduct(geoCoordinatesFileName);
            // todo: is it ok just to take the lats/lons from the 'middle' camera??
            if (geoCoordinatesProduct.getBand("latitude_CAM3") != null &&
                    geoCoordinatesProduct.getBand("longitude_CAM3") != null) {
                product.setGeoCoding(new PixelGeoCoding(geoCoordinatesProduct.getBand("latitude_CAM3"),
                                                        geoCoordinatesProduct.getBand("longitude_CAM3"), null, 5));
            }
        } catch (IOException e) {
            String msg = String.format("Not able to read file '%s.", geoCoordinatesFileName);
            logger.log(Level.WARNING, msg, e);
        }
    }

    private void attachMeasurementBands(List<Product> measurementProducts, Product product) {
        for (final Product bandProduct : measurementProducts) {
            for (final Band sourceBand : bandProduct.getBands()) {
                final String bandName = sourceBand.getName();
                final Band targetBand = ProductUtils.copyBand(bandName, bandProduct, product);
                if (sourceBand.getName().endsWith(SynL2FlagCodings.SYN_FLAG_BAND_NAME)) {
                    final FlagCoding synFlagCoding = SynL2FlagCodings.createSynFlagCoding();
                    targetBand.setSampleCoding(synFlagCoding);
                    product.getFlagCodingGroup().add(synFlagCoding);
                } else if (sourceBand.getName().endsWith(SynL2FlagCodings.OLCI_FLAG_BAND_NAME)) {
                    final FlagCoding olciFlagCoding = SynL2FlagCodings.createOlciFlagCoding();
                    targetBand.setSampleCoding(olciFlagCoding);
                    product.getFlagCodingGroup().add(olciFlagCoding);
                } else if (sourceBand.getName().endsWith(SynL2FlagCodings.SLSTR_NADIR_FLAG_BAND_NAME)) {
                    final FlagCoding slnFlagCoding = SynL2FlagCodings.createSlstrNadirFlagCoding();
                    targetBand.setSampleCoding(slnFlagCoding);
                    product.getFlagCodingGroup().add(slnFlagCoding);
                } else if (sourceBand.getName().endsWith(SynL2FlagCodings.SLSTR_OBLIQUE_FLAG_BAND_NAME)) {
                    final FlagCoding sloFlagCoding = SynL2FlagCodings.createSlstrObliqueFlagCoding();
                    targetBand.setSampleCoding(sloFlagCoding);
                    product.getFlagCodingGroup().add(sloFlagCoding);
                }
                targetBand.setSourceImage(sourceBand.getSourceImage());
            }
        }
    }

    private String getProductName() {
        return FileUtils.getFilenameWithoutExtension(getParentInputDirectory());
    }

    private List<Product> loadMeasurementProducts(List<String> measurementFileNames) {
        List<Product> products = new ArrayList<Product>();
        for (String fileName : measurementFileNames) {
            try {
                products.add(readProduct(fileName));
            } catch (IOException e) {
                String msg = String.format("Not able to read file '%s.", fileName);
                logger.log(Level.WARNING, msg, e);
            }
        }
        return products;
    }

    private Product readProduct(String fileName) throws IOException {
        File dataSetFile = new File(getParentInputDirectory(), fileName);
        Product product = ProductIO.readProduct(dataSetFile);
        if (product == null) {
            String msg = String.format("Could not read file '%s. No appropriate reader found.", fileName);
            throw new IOException(msg);
        }
        return product;
    }

    private SynL2Manifest createManifestFile(File inputFile) throws IOException {
        InputStream manifestInputStream = new FileInputStream(inputFile);
        try {
            return new SynL2Manifest(createXmlDocument(manifestInputStream));
        } finally {
            manifestInputStream.close();
        }

    }

    private Document createXmlDocument(InputStream inputStream) throws IOException {
        Document doc = null;
        try {
            doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(inputStream);
        } catch (SAXException e) {
            String msg = "Not able to read manifest XML file.";
            logger.log(Level.WARNING, msg, e);
        } catch (ParserConfigurationException e) {
            String msg = "Not able to read manifest XML file.";
            logger.log(Level.WARNING, msg, e);
        }
        return doc;
    }

    private File getParentInputDirectory() {
        return getInputFile().getParentFile();
    }

    private File getInputFile() {
        return new File(getInput().toString());
    }

}
