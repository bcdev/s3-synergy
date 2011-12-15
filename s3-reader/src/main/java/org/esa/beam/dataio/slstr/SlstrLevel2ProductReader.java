package org.esa.beam.dataio.slstr;

import com.bc.ceres.core.ProgressMonitor;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductIO;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
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
 * Product reader responsible for reading SLSTR L2 data products in SAFE format.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class SlstrLevel2ProductReader extends AbstractProductReader {

    private final Logger logger = null;
    private List<Product> measurementProducts;
    private Product geodeticDataCoordinatesProduct;
    private Product cartesianDataCoordinatesProduct;
    private Product geodeticTiePointsProduct;
    private Product cartesianTiePointsProduct;

    /**
     * Constructs a new abstract product reader.
     *
     * @param readerPlugIn the reader plug-in which created this reader, can be <code>null</code> for internal reader
     *                     implementations
     */
    protected SlstrLevel2ProductReader(ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
    }

    @Override
    protected Product readProductNodesImpl() throws IOException {
        File inputFile = getInputFile();
        SlstrL2LndManifest manifest = createManifestFile(inputFile);
        return createProduct(manifest);
    }

    @Override
    protected void readBandRasterDataImpl(int sourceOffsetX, int sourceOffsetY, int sourceWidth, int sourceHeight, int sourceStepX, int sourceStepY, Band destBand, int destOffsetX, int destOffsetY, int destWidth, int destHeight, ProductData destBuffer, ProgressMonitor pm) throws IOException {
        throw new IllegalStateException(String.format("No source to read from for band '%s'.", destBand.getName()));
    }

    private Product createProduct(SlstrL2LndManifest manifest) {
        measurementProducts = loadMeasurementProducts(manifest.getMeasurementFileNames());
        int width = measurementProducts.get(0).getSceneRasterWidth();
        int height = measurementProducts.get(0).getSceneRasterHeight();
        Product product = new Product(getProductName(), SlstrLevel2LndProductReaderPlugIn.FORMAT_NAME_SLSTR_L2, width,
                height, this);
        product.setStartTime(manifest.getStartTime());
        product.setEndTime(manifest.getStopTime());
        product.setFileLocation(getInputFile());
        attachMeasurementBands(measurementProducts, product);
        attachAnnotationData(manifest, product);
        return product;
    }

    private void attachAnnotationData(SlstrL2LndManifest manifest, Product product) {
        attachGeodeticDataCoodinatesToProduct(manifest.getGeodeticDataCoordinatesFileName(), product);
        attachCartesianDataCoodinatesToProduct(manifest.getCartesianDataCoordinatesFileName(), product);
//        attachTiePointsToProduct(manifest.getTiePointsFileName(), product);
    }

//    private void attachTiePointsToProduct(String tiePointsFileName, Product product) {
//        try {
//            tiePointsProduct = readProduct(tiePointsFileName);
//            final MetadataElement metadataRoot = tiePointsProduct.getMetadataRoot();
//            final MetadataElement globalAttributes = metadataRoot.getElement("Global_Attributes");
//            final int subsampling = globalAttributes.getAttributeInt("subsampling_factor");
//            for (final Band band : tiePointsProduct.getBands()) {
//                final MultiLevelImage sourceImage = band.getGeophysicalImage();
//                final int width = sourceImage.getWidth();
//                final int height = sourceImage.getHeight();
//                final float[] tiePointData = new float[width * height];
//                sourceImage.getData().getSamples(0, 0, width, height, 0, tiePointData);
//                TiePointGrid tiePointGrid = new TiePointGrid(band.getName(), band.getRasterWidth(),
//                        band.getRasterHeight(), 0, 0, subsampling,
//                        subsampling, tiePointData, true);
//                product.addTiePointGrid(tiePointGrid);
//            }
//            if (product.getTiePointGrid("TP_latitude") != null && product.getTiePointGrid("TP_longitude") != null) {
//                product.setGeoCoding(new TiePointGeoCoding(product.getTiePointGrid("TP_latitude"),
//                        product.getTiePointGrid("TP_longitude")));
//            }
//        } catch (IOException e) {
//            String msg = String.format("Not able to read file '%s.", tiePointsFileName);
//            logger.log(Level.WARNING, msg, e);
//        }
//    }

    private void attachGeodeticDataCoodinatesToProduct(String geodeticDataCoordinatesFileName, Product product) {
        try {
            geodeticDataCoordinatesProduct = readProduct(geodeticDataCoordinatesFileName);
            geodeticDataCoordinatesProduct.transferGeoCodingTo(product, null);
        } catch (IOException e) {
            String msg = String.format("Not able to read file '%s.", geodeticDataCoordinatesFileName);
            logger.log(Level.WARNING, msg, e);
        }
    }

    private void attachCartesianDataCoodinatesToProduct(String cartesianDataCoordinatesFileName, Product product) {
        try {
            cartesianDataCoordinatesProduct = readProduct(cartesianDataCoordinatesFileName);
            cartesianDataCoordinatesProduct.transferGeoCodingTo(product, null);
        } catch (IOException e) {
            String msg = String.format("Not able to read file '%s.", cartesianDataCoordinatesFileName);
            logger.log(Level.WARNING, msg, e);
        }
    }

    private void attachMeasurementBands(List<Product> measurementProducts, Product product) {
        int k = 0;
        for (final Product bandProduct : measurementProducts) {
            for (final Band sourceBand : bandProduct.getBands()) {
                final String bandName = sourceBand.getName();
                final Band targetBand = ProductUtils.copyBand(bandName, bandProduct, product);
                targetBand.setSourceImage(sourceBand.getSourceImage());
//                if (bandName.matches("R[0-9]{3}[0-9]?")) {
//                    targetBand.setSpectralBandIndex(k);
//                    targetBand.setSpectralWavelength(spectralWavelengths[k]);
//                    targetBand.setSpectralBandwidth(spectralBandwidths[k]);
//                    k++;
//                }
            }
        }
    }


    private String getProductName() {
        return FileUtils.getFilenameWithoutExtension(getParentInputDirectory());
    }

    private List<Product> loadMeasurementProducts(List<String> measurementFileNames) {
        List<Product> products = new ArrayList<Product>();
        for (String fileName : measurementFileNames) {
            // todo: FRP_in.nc has empty strings in global attributes, cannot be read by current netCDF reader
            if (!fileName.startsWith("FRP")) {
                try {
                    products.add(readProduct(fileName));
                } catch (IOException e) {
                    String msg = String.format("Not able to read file '%s.", fileName);
                    logger.log(Level.WARNING, msg, e);
                }
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

    private SlstrL2LndManifest createManifestFile(File inputFile) throws IOException {
        InputStream manifestInputStream = new FileInputStream(inputFile);
        try {
            return new SlstrL2LndManifest(createXmlDocument(manifestInputStream));
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
}
