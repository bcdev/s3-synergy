package org.esa.beam.dataio.syn;

import com.bc.ceres.core.ProgressMonitor;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductIO;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.FlagCoding;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.ProductData;
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
 * Product reader responsible for reading OLCI L2 data products in SAFE format.
 *
 * @author Marco Peters
 * @since 1.0
 */
public class SynProductReader extends AbstractProductReader {

    private static final float[] spectralWavelengths = new float[21];
    private static final float[] spectralBandwidths = new float[21];

    private final Logger logger;
    private List<Product> measurementProducts;
    private Product geoCoordinatesProduct;
    private Product tiePointsProduct;

    public SynProductReader(SynProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
        logger = Logger.getLogger(getClass().getSimpleName());
    }

    @Override
    protected Product readProductNodesImpl() throws IOException {
        File inputFile = getInputFile();
        SynManifest manifest = createManifestFile(inputFile);
        return createProduct(manifest);
    }


    @Override
    protected void readBandRasterDataImpl(int sourceOffsetX, int sourceOffsetY, int sourceWidth, int sourceHeight,
                                          int sourceStepX, int sourceStepY, Band destBand, int destOffsetX,
                                          int destOffsetY, int destWidth, int destHeight, ProductData destBuffer,
                                          ProgressMonitor pm) throws IOException {
        throw new IllegalStateException(String.format("No source to read from for band '%s'.", destBand.getName()));
    }

    private Product createProduct(SynManifest manifest) {
        measurementProducts = loadMeasurementProducts(manifest.getMeasurementFileNames());
        int width = measurementProducts.get(0).getSceneRasterWidth();
        int height = measurementProducts.get(0).getSceneRasterHeight();
        Product product = new Product(getProductName(), SynProductReaderPlugIn.FORMAT_NAME_SYN, width,
                                      height, this);
        product.setStartTime(manifest.getStartTime());
        product.setEndTime(manifest.getStopTime());
        product.setFileLocation(getInputFile());
        attachMeasurementBands(measurementProducts, product);
        attachAnnotationData(manifest, product);
        return product;
    }

    private void attachAnnotationData(SynManifest manifest, Product product) {
        attachGeoCoodinatesToProduct(manifest.getGeoCoordinatesFileName(), product);
        attachTiePointsToProduct(manifest.getTiepointsFileNames(), product);
    }

    private void attachTiePointsToProduct(List<String> tiePointsFileNames, Product product) {
        // the tie points are provided on a different grid, so we currently don't use them for the SYN product
    }

    private void attachGeoCoodinatesToProduct(String geoCoordinatesFileName, Product product) {
        try {
            geoCoordinatesProduct = readProduct(geoCoordinatesFileName);
            geoCoordinatesProduct.transferGeoCodingTo(product, null);
        } catch (IOException e) {
            String msg = String.format("Not able to read file '%s.", geoCoordinatesFileName);
            logger.log(Level.WARNING, msg, e);
        }
    }

    private void attachMeasurementBands(List<Product> measurementProducts, Product product) {
        int k = 0;
        for (final Product bandProduct : measurementProducts) {
            for (final Band sourceBand : bandProduct.getBands()) {
                final String bandName = sourceBand.getName();
                final Band targetBand = ProductUtils.copyBand(bandName, bandProduct, product);
                if (sourceBand.getName().endsWith(SynFlagCodings.SYN_FLAG_BAND_NAME)) {
                    final FlagCoding synFlagCoding = SynFlagCodings.createSynFlagCoding();
                    targetBand.setSampleCoding(synFlagCoding);
                    product.getFlagCodingGroup().add(synFlagCoding);
                } else if (sourceBand.getName().endsWith(SynFlagCodings.OLCI_FLAG_BAND_NAME)) {
                    final FlagCoding olciFlagCoding = SynFlagCodings.createOlciFlagCoding();
                    targetBand.setSampleCoding(olciFlagCoding);
                    product.getFlagCodingGroup().add(olciFlagCoding);
                } else if (sourceBand.getName().endsWith(SynFlagCodings.SLSTR_NADIR_FLAG_BAND_NAME)) {
                    final FlagCoding slnFlagCoding = SynFlagCodings.createSlstrNadirFlagCoding();
                    targetBand.setSampleCoding(slnFlagCoding);
                    product.getFlagCodingGroup().add(slnFlagCoding);
                } else if (sourceBand.getName().endsWith(SynFlagCodings.SLSTR_OBLIQUE_FLAG_BAND_NAME)) {
                    final FlagCoding sloFlagCoding = SynFlagCodings.createSlstrObliqueCoding();
                    targetBand.setSampleCoding(sloFlagCoding);
                    product.getFlagCodingGroup().add(sloFlagCoding);
                }
                targetBand.setSourceImage(sourceBand.getSourceImage());
            }
        }
    }

//    private void attachAlbedoDataToProduct(Product product, Product albedoInputProduct) {
//        if (hasSameRasterDimension(product, albedoInputProduct)) {
//            for (final Band sourceBand : albedoInputProduct.getBands()) {
//                String bandName = sourceBand.getName();
//                Band targetBand = ProductUtils.copyBand(bandName, albedoInputProduct, product);
//                if (sourceBand.getName().startsWith(ALBEDO_BAND_NAME_PREFIX)) {
//                    targetBand.setScalingFactor(0.0001);
//                } else if(sourceBand.getName().equals(QUALITY_FLAG_BAND_NAME)) {
//                    final FlagCoding qualityFlagCoding = createQualityFlagCoding(QUALITY_FLAG_BAND_NAME);
//                    targetBand.setSampleCoding(qualityFlagCoding);
//                    product.getFlagCodingGroup().add(qualityFlagCoding);
//                }
//                targetBand.setSourceImage(sourceBand.getSourceImage());
//            }
//        }
//    }


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

    private SynManifest createManifestFile(File inputFile) throws IOException {
        InputStream manifestInputStream = new FileInputStream(inputFile);
        try {
            return new SynManifest(createXmlDocument(manifestInputStream));
        } finally {
            manifestInputStream.close();
        }

    }

    private Document createXmlDocument(InputStream inputStream) throws IOException {
        Document doc = null;
        try {
            doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(inputStream);
        } catch (SAXException e) {
            e.printStackTrace();
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        }
        return doc;
    }

    private File getParentInputDirectory() {
        return getInputFile().getParentFile();
    }

    private File getInputFile() {
        return new File(getInput().toString());
    }

    @Override
    public void close() throws IOException {
        for (Product product : measurementProducts) {
            product.dispose();
        }
        if (geoCoordinatesProduct != null) {
            geoCoordinatesProduct.dispose();
        }
        if (tiePointsProduct != null) {
            tiePointsProduct.dispose();
        }
        super.close();
    }
}
