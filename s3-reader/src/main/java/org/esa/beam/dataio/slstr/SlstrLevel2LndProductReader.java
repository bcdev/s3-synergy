package org.esa.beam.dataio.slstr;

import com.bc.ceres.core.ProgressMonitor;
import com.bc.ceres.glevel.MultiLevelImage;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductIO;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.framework.datamodel.*;
import org.esa.beam.util.ProductUtils;
import org.esa.beam.util.io.FileUtils;
import org.esa.beam.util.logging.BeamLogManager;
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
public class SlstrLevel2LndProductReader extends AbstractProductReader {

    private final Logger logger;

    private final String[] indicesProductBandNames = {"scan", "pixel", "detector"};
    private final String[] flagsProductBandNames = {"cloud", "bayes", "pointing", "confidence"};

    /**
     * Constructs a new abstract product reader.
     *
     * @param readerPlugIn the reader plug-in which created this reader, can be <code>null</code> for internal reader
     *                     implementations
     */
    protected SlstrLevel2LndProductReader(ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
        logger = BeamLogManager.getSystemLogger();
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
        final List<Product> measurementProducts = loadMeasurementProducts(manifest.getMeasurementFileNames());
        final int width = measurementProducts.get(0).getSceneRasterWidth();
        final int height = measurementProducts.get(0).getSceneRasterHeight();
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
        // Tie Points (16km grid)
        // geodetic_tx.nc
        attachGeodeticTiePointsToProduct(manifest.getGeodeticTiepointCoordinatesFileName(), product);
        // cartesian_tx.nc
        attachCartesianTiePointsToProduct(manifest.getCartesianTiepointCoordinatesFileName(), product);
        // geometry_tn.nc
        attachGeometryTiePointsToProduct(manifest.getNadirSolarViewGeometryTiepointsFileName(), product);
        // met_tx.nc
        attachMeteorologicalTiePointsToProduct(manifest.getMeteorologicalTiePointsFileName(), product);

        // Regular bands (1km grid)
        // cartesian_in.nc:
        attachCartesianDataCoodinatesToProduct(manifest.getCartesianDataCoordinatesFileName(), product);
        // geodetic_in.nc
        attachGeodeticDataCoodinatesToProduct(manifest.getGeodeticDataCoordinatesFileName(), product);
        // flags_in.nc:
        attachFlagsToProduct(manifest.getNadirFlagsFileName(), product);
        // indices_in.nc:
        attachIndicesToProduct(manifest.getIndicesFileName(), product);
        // time_in.nc
        attachTimeCoodinatesToProduct(manifest.getTimeCoordinatesFileName(), product);
    }

    private void attachMeasurementBands(List<Product> measurementProducts, Product product) {
        for (final Product bandProduct : measurementProducts) {
            for (final Band sourceBand : bandProduct.getBands()) {
                final String bandName = sourceBand.getName();
                final Band targetBand = ProductUtils.copyBand(bandName, bandProduct, product);
                targetBand.setSourceImage(sourceBand.getSourceImage());
            }
        }
    }

    private void attachGeodeticTiePointsToProduct(String tiePointsFileName, Product product) {
        // file: geodetic_tx.nc
        try {
            final Product geodeticTiePointsProduct = readProduct(tiePointsFileName);
            final MetadataElement metadataRoot = geodeticTiePointsProduct.getMetadataRoot();
            final MetadataElement globalAttributes = metadataRoot.getElement("Global_Attributes");
            final short[] resolutions = (short[]) globalAttributes.getAttribute("resolution").getDataElems();
            for (final Band band : geodeticTiePointsProduct.getBands()) {
                final MultiLevelImage sourceImage = band.getGeophysicalImage();
                final int width = sourceImage.getWidth();
                final int height = sourceImage.getHeight();
                final float[] tiePointData = new float[width * height];
                sourceImage.getData().getSamples(0, 0, width, height, 0, tiePointData);
                TiePointGrid tiePointGrid = new TiePointGrid(band.getName(), band.getRasterWidth(),
                        band.getRasterHeight(), 0, 0, resolutions[0] / 1000.0f,
                        resolutions[1] / 1000.0f, tiePointData, true);
                product.addTiePointGrid(tiePointGrid);
            }
            if (product.getTiePointGrid("latitude") != null && product.getTiePointGrid("longitude") != null) {
                product.setGeoCoding(new TiePointGeoCoding(product.getTiePointGrid("latitude"),
                        product.getTiePointGrid("longitude")));
            }
        } catch (IOException e) {
            String msg = String.format("Not able to read file '%s.", tiePointsFileName);
            logger.log(Level.WARNING, msg, e);
        }
    }

    private void attachCartesianTiePointsToProduct(String tiePointsFileName, Product product) {
        // file: cartesian_tx.nc
        // todo: clarify what these data mean and how to handle them
        // I.e., product dimensions are unclear, datasets are empty in test files
    }

    private void attachCartesianDataCoodinatesToProduct(String cartesianDataCoordinatesFileName, Product product) {
        // file: cartesian_in.nc
        // todo: clarify what these data mean and how to handle them.
        // I.e., product dimensions are unclear, datasets are empty in test file
    }

    private void attachGeodeticDataCoodinatesToProduct(String geodeticDataCoordinatesFileName, Product product) {
        // file: geodetic_in.nc
        // todo: clarify what these data mean and how to handle them.
        // I.e., product dimensions are unclear, 'elevation' datasets are empty in test files
    }

    private void attachFlagsToProduct(String flagsFileName, Product product) {
        // file: indices_in.nc
        try {
            final Product flagsProduct = readProduct(flagsFileName);
            for (final Band sourceBand : flagsProduct.getBands()) {
                final String bandName = sourceBand.getName();
                for (String flagsBandName: flagsProductBandNames) {
                    if ((bandName.equals(flagsBandName))) {
                        final Band targetBand = ProductUtils.copyBand(bandName, flagsProduct, product);
                        targetBand.setSourceImage(sourceBand.getSourceImage());
                    }
                }
            }
        } catch (IOException e) {
            String msg = String.format("Not able to read file '%s.", flagsFileName);
            logger.log(Level.WARNING, msg, e);
        }
    }

    private void attachGeometryTiePointsToProduct(String geometryTiepointsFileName, Product product) {
        // file: geometry_tn.nc
        try {
            final Product geometryTiePointsProduct = readProduct(geometryTiepointsFileName);
            final MetadataElement metadataRoot = geometryTiePointsProduct.getMetadataRoot();
            final MetadataElement globalAttributes = metadataRoot.getElement("Global_Attributes");
            final short[] resolutions = (short[]) globalAttributes.getAttribute("resolution").getDataElems();
            for (final Band band : geometryTiePointsProduct.getBands()) {
                final MultiLevelImage sourceImage = band.getGeophysicalImage();
                final int width = sourceImage.getWidth();
                final int height = sourceImage.getHeight();
                final float[] tiePointData = new float[width * height];
                sourceImage.getData().getSamples(0, 0, width, height, 0, tiePointData);
                TiePointGrid tiePointGrid = new TiePointGrid(band.getName(), band.getRasterWidth(),
                        band.getRasterHeight(), 0, 0, resolutions[0] / 1000.0f,
                        resolutions[1] / 1000.0f, tiePointData, true);
                product.addTiePointGrid(tiePointGrid);
            }
        } catch (IOException e) {
            String msg = String.format("Not able to read file '%s.", geometryTiepointsFileName);
            logger.log(Level.WARNING, msg, e);
        }
    }

    private void attachIndicesToProduct(String indicesFileName, Product product) {
        // file: indices_in.nc
        try {
            final Product indicesProduct = readProduct(indicesFileName);
            for (final Band sourceBand : indicesProduct.getBands()) {
                final String bandName = sourceBand.getName();
                for (String indicesBandName:indicesProductBandNames) {
                    if ((bandName.equals(indicesBandName))) {
                        final Band targetBand = ProductUtils.copyBand(bandName, indicesProduct, product);
                        targetBand.setSourceImage(sourceBand.getSourceImage());
                    }
                }
            }
        } catch (IOException e) {
            String msg = String.format("Not able to read file '%s.", indicesFileName);
            logger.log(Level.WARNING, msg, e);
        }
    }

    private void attachMeteorologicalTiePointsToProduct(String meteorologicalTiePointsFileName, Product product) {
        // file: met_tx.nc
        // todo: clarify how to handle these data
        // I.e., product dimensions are unclear, datasets are empty in test files
    }

    private void attachTimeCoodinatesToProduct(String timeCoordinatesFileName, Product product) {
        // file: geodetic_tx.nc
        try {
            // todo: this will currently fail because netCDF file contains a 64bit variable 'scan_time'
            final Product timeProduct = readProduct(timeCoordinatesFileName);
        } catch (IOException e) {
            String msg = String.format("Not able to read file '%s.", timeCoordinatesFileName);
            logger.log(Level.WARNING, msg, e);
        }
    }

    private String getProductName() {
        return FileUtils.getFilenameWithoutExtension(getParentInputDirectory());
    }

    private List<Product> loadMeasurementProducts(List<String> measurementFileNames) {
        List<Product> products = new ArrayList<Product>();
        for (String fileName : measurementFileNames) {
            // todo: FRP_in.nc has empty strings in global attributes, cannot be read by current netCDF reader. Fix there.
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
