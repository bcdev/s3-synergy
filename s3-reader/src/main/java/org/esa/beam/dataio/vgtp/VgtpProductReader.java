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

package org.esa.beam.dataio.vgtp;

import com.bc.ceres.core.ProgressMonitor;
import com.bc.ceres.glevel.MultiLevelImage;
import org.esa.beam.dataio.syn.SynProductReaderPlugIn;
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
 * Product reader responsible for reading VGT P data products in SAFE format.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class VgtpProductReader extends AbstractProductReader {

    private final Logger logger;
    private List<Product> measurementProducts;
    private List<Product> tiepointsProducts;
    private Product geoCoordinatesProduct;
    private int width;
    private int height;

    public VgtpProductReader(VgtpProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
        logger = Logger.getLogger(getClass().getSimpleName());
    }

    @Override
    protected Product readProductNodesImpl() throws IOException {
        File inputFile = getInputFile();
        VgtpManifest manifest = createManifestFile(inputFile);
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
        for (Product product : tiepointsProducts) {
            product.dispose();
        }
        if (geoCoordinatesProduct != null) {
            geoCoordinatesProduct.dispose();
        }
        super.close();
    }

    private Product createProduct(VgtpManifest manifest) {
        measurementProducts = loadDataProducts(manifest.getMeasurementFileNames());
        tiepointsProducts = loadDataProducts(manifest.getTiepointsFileNames());
        width = measurementProducts.get(0).getSceneRasterWidth();
        height = measurementProducts.get(0).getSceneRasterHeight();
        Product product = new Product(getProductName(), SynProductReaderPlugIn.FORMAT_NAME_SYN, width,
                                      height, this);
        product.setStartTime(manifest.getStartTime());
        product.setEndTime(manifest.getStopTime());
        product.setFileLocation(getInputFile());
        // todo: copy metadata
        attachMeasurementBands(measurementProducts, product);
        attachAnnotationData(tiepointsProducts, manifest, product);
        return product;
    }

    private void attachAnnotationData(List<Product> tiepointsProducts, VgtpManifest manifest, Product product) {
        attachTiePointsToProduct(tiepointsProducts, product);
    }

    private void attachTiePointsToProduct(List<Product> tiePointsProducts, Product product) {
        for (Product tpProduct : tiePointsProducts) {
            final MetadataElement metadataRoot = tpProduct.getMetadataRoot();
            final MetadataElement globalAttributes = metadataRoot.getElement("Global_Attributes");
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
            product.addTiePointGrid(tiePointGrid);
//            if (product.getTiePointGrid("TP_latitude") != null && product.getTiePointGrid("TP_longitude") != null) {
//                product.setGeoCoding(new TiePointGeoCoding(product.getTiePointGrid("TP_latitude"),
//                                                           product.getTiePointGrid("TP_longitude")));
//            }
        }
    }

    private void attachMeasurementBands(List<Product> measurementProducts, Product product) {
        for (final Product bandProduct : measurementProducts) {
            for (final Band sourceBand : bandProduct.getBands()) {
                final String bandName = sourceBand.getName();
                if (!product.containsBand(bandName)) {
                    final Band targetBand = ProductUtils.copyBand(bandName, bandProduct, product);
                    if (sourceBand.getName().endsWith(VgtpFlagCodings.VGTP_SM_FLAG_BAND_NAME)) {
                        final FlagCoding vgtpSmFlagCoding = VgtpFlagCodings.createSmFlagCoding();
                        targetBand.setSampleCoding(vgtpSmFlagCoding);
                        product.getFlagCodingGroup().add(vgtpSmFlagCoding);
                    }
                    targetBand.setSourceImage(sourceBand.getSourceImage());
                }
            }
        }
    }

    private String getProductName() {
        return FileUtils.getFilenameWithoutExtension(getParentInputDirectory());
    }

    private List<Product> loadDataProducts(List<String> dataFileNames) {
        List<Product> products = new ArrayList<Product>();
        for (String fileName : dataFileNames) {
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

    private VgtpManifest createManifestFile(File inputFile) throws IOException {
        InputStream manifestInputStream = new FileInputStream(inputFile);
        try {
            return new VgtpManifest(createXmlDocument(manifestInputStream));
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
