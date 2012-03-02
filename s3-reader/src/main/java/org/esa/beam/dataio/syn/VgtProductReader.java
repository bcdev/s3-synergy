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
public class VgtProductReader extends AbstractProductReader {

    private final Logger logger;
    private List<Product> measurementProducts;
    private List<Product> tiepointProducts;
    private VgtManifest manifest;

    public VgtProductReader(VgtProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
        logger = Logger.getLogger(getClass().getSimpleName());
    }

    @Override
    protected Product readProductNodesImpl() throws IOException {
        File inputFile = getInputFile();
        manifest = createManifestFile(inputFile);
        return createProduct();
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
        for (Product product : tiepointProducts) {
            product.dispose();
        }
        super.close();
    }

    private Product createProduct() {
        final String productName = getProductName();
        final String productType = getProductType();

        final List<String> measurementFileNames = manifest.getMeasurementFileNames();
        measurementFileNames.add(manifest.getStatusFlagFile());

        if (isVGSProduct()) {
            measurementFileNames.addAll(manifest.getGeometryFileNames());
            measurementFileNames.add(manifest.getTimeCoordinatesFile());
        }
        measurementProducts = loadDataProducts(measurementFileNames);
        final List<String> tiepointFileNames = manifest.getTiepointFileNames();
        if (isVGPProduct()) {
            tiepointFileNames.addAll(manifest.getGeometryFileNames());
        }
        tiepointProducts = loadDataProducts(tiepointFileNames);
        final int width = measurementProducts.get(0).getSceneRasterWidth();
        final int height = measurementProducts.get(0).getSceneRasterHeight();
        Product product = new Product(productName, productType, width, height, this);
        product.setStartTime(manifest.getStartTime());
        product.setEndTime(manifest.getStopTime());
        product.setFileLocation(getInputFile());

        product.getMetadataRoot().addElement(new MetadataElement("Global_Attributes"));
        product.getMetadataRoot().addElement(new MetadataElement("Variable_Attributes"));
        ProductUtils.copyMetadata(measurementProducts.get(0).getMetadataRoot().getElement("Global_Attributes"),
                                  product.getMetadataRoot().getElement("Global_Attributes"));
        product.getMetadataRoot().getElement("Global_Attributes").setAttributeString("dataset_name", productName);
        for (Product measurementProduct : measurementProducts) {
            for (final MetadataElement element : measurementProduct.getMetadataRoot().getElement(
                    "Variable_Attributes").getElements()) {
                if (isMeasurementBandElement(element)) {
                    product.getMetadataRoot().getElement("Variable_Attributes").addElement(element.createDeepClone());
                }
            }
        }

        attachMeasurementBands(measurementProducts, product);
        attachAnnotationData(tiepointProducts, product);
        ProductUtils.copyGeoCoding(measurementProducts.get(0), product);
        return product;
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
            final int subsampling = getTiePointSubsamplingFactor();
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

    private int getTiePointSubsamplingFactor() {
        if (isVGSProduct()) {
            return 1;
        } else if (isVGPProduct()) {
            return 8;
        } else {
            throw new IllegalArgumentException("Invalid input - product seems to be neither of type VGS nor VGP.");
        }
    }

    private String getProductType() {
        final String descr = manifest.getDescription();
        if (descr.contains("VGT S")) {
            return "VGT_S_L2_SYN";
        } else if (descr.contains("VGT P")) {
            return "VGT_P_L2_SYN";
        } else {
            throw new IllegalArgumentException("Invalid input - product seems to be neither of type VGS nor VGP.");
        }
    }

    private boolean isVGSProduct() {
        return getProductType().equals("VGT_S_L2_SYN");
    }

    private boolean isVGPProduct() {
        return getProductType().equals("VGT_P_L2_SYN");
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

    private void attachMeasurementBands(List<Product> measurementProducts, Product product) {
        for (final Product bandProduct : measurementProducts) {
            for (final Band sourceBand : bandProduct.getBands()) {
                final String bandName = sourceBand.getName();
                if (!product.containsBand(bandName)) {
                    final Band targetBand = ProductUtils.copyBand(bandName, bandProduct, product);
                    if (sourceBand.getName().endsWith(VgtFlagCodings.VGTP_SM_FLAG_BAND_NAME)) {
                        final FlagCoding vgtpSmFlagCoding = VgtFlagCodings.createSmFlagCoding();
                        targetBand.setSampleCoding(vgtpSmFlagCoding);
                        product.getFlagCodingGroup().add(vgtpSmFlagCoding);
                    } else {
                        setMeasurementBandScalingFactor(targetBand);
                    }
                    targetBand.setSourceImage(sourceBand.getSourceImage());
                }
            }
        }
    }

    private String getProductName() {
        return FileUtils.getFilenameWithoutExtension(getParentInputDirectory());
    }

    private List<Product> loadDataProducts(List<String> fileNameList) {
        List<Product> products = new ArrayList<Product>();
        for (String fileName : fileNameList) {
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

    private VgtManifest createManifestFile(File inputFile) throws IOException {
        InputStream manifestInputStream = new FileInputStream(inputFile);
        try {
            return new VgtManifest(createXmlDocument(manifestInputStream));
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
