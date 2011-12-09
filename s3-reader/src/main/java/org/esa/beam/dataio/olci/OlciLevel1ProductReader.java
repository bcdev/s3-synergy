package org.esa.beam.dataio.olci;

import com.bc.ceres.core.ProgressMonitor;
import com.bc.ceres.glevel.MultiLevelImage;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductIO;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.MetadataElement;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.ProductData;
import org.esa.beam.framework.datamodel.TiePointGrid;
import org.esa.beam.util.ProductUtils;
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
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Product reader responsible for reading OLCI L1b data products in SAFE format.
 *
 * @author Marco Peters
 * @since 1.0
 */
class OlciLevel1ProductReader extends AbstractProductReader {

    private static final float[] spectralWavelengths = new float[21];
    private static final float[] spectralBandwidths = new float[21];

    static {
        getSpectralBandsProperties(spectralWavelengths, spectralBandwidths);
    }

    static void getSpectralBandsProperties(float[] wavelengths, float[] bandwidths) {
        final Properties properties = new Properties();

        try {
            properties.load(OlciLevel1ProductReader.class.getResourceAsStream("radianceBands.properties"));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }

        for (int i = 0; i < wavelengths.length; i++) {
            wavelengths[i] = Float.parseFloat(properties.getProperty("wavelengths." + i));
        }
        for (int i = 0; i < bandwidths.length; i++) {
            bandwidths[i] = Float.parseFloat(properties.getProperty("bandwidths." + i));
        }
    }

    private final Logger logger;
    private List<Product> bandProducts;
    private List<Product> annotationProducts;

    protected OlciLevel1ProductReader(OlciLevel1ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
        logger = Logger.getLogger(getClass().getSimpleName());
    }

    @Override
    protected Product readProductNodesImpl() throws IOException {
        File inputFile = getInputFile();
        OlciL1bManifest manifest = createManifestFile(inputFile);
        return createProduct(manifest);
    }

    private Product createProduct(OlciL1bManifest manifest) {
        Product product = new Product(manifest.getProductName(), manifest.getProductType(),
                                      manifest.getColumnCount(), manifest.getLineCount(), this);
        product.setDescription(manifest.getDescription());
        product.setStartTime(manifest.getStartTime());
        product.setEndTime(manifest.getStopTime());
        product.setFileLocation(getInputFile());
        MetadataElement root = product.getMetadataRoot();
        root.addElement(manifest.getFixedHeader());
        root.addElement(manifest.getMainProductHeader());
        root.addElement(manifest.getSpecificProductHeader());
        attachBandsToProduct(manifest, product);
        attachAnnotationDataToProduct(manifest, product);
        return product;
    }

    private void attachAnnotationDataToProduct(OlciL1bManifest manifest, Product product) {
        List<DataSetPointer> annotationPointers = manifest.getDataSetPointers(DataSetPointer.Type.A);
        annotationPointers = removeOrphanedDataSetPointers(annotationPointers);
        annotationProducts = createDataSetProducts(annotationPointers);
        attachGeoCodingToProduct(annotationProducts, product);
        attachFlagCodingToProduct(annotationProducts, product);
        attachTiePointsToProduct(annotationProducts, product);
    }

    private void attachTiePointsToProduct(List<Product> annotationProducts, Product product) {
        for (Product annotationProduct : annotationProducts) {
            if ("TiePoints".equals(annotationProduct.getName())) {
                Band[] tiePointBands = annotationProduct.getBands();
                MetadataElement metadataRoot = annotationProduct.getMetadataRoot();
                MetadataElement globalAttributes = metadataRoot.getElement("Global_Attributes");
                int subsampling = globalAttributes.getAttributeInt("subsampling_factor");
                for (Band band : tiePointBands) {
                    MultiLevelImage sourceImage = band.getGeophysicalImage();

                    int width = sourceImage.getWidth();
                    int height = sourceImage.getHeight();
                    float[] tiePointData = new float[width * height];
                    sourceImage.getData().getSamples(0,0, width, height, 0, tiePointData);
                    TiePointGrid tiePointGrid = new TiePointGrid(band.getName(), band.getRasterWidth(),
                                                                 band.getRasterHeight(), 0, 0, subsampling,
                                                                 subsampling, tiePointData, true);
                    product.addTiePointGrid(tiePointGrid);
                }
            }
        }
    }

    private void attachFlagCodingToProduct(List<Product> annotationProducts, Product product) {
        for (Product annotationProduct : annotationProducts) {
            if(annotationProduct.getFlagCodingGroup().getNodeCount() > 0) {
                ProductUtils.copyFlagBands(annotationProduct, product);
            }
        }
    }

    private void attachGeoCodingToProduct(List<Product> annotationProducts, Product product) {
        // TODO - use tie point geocoding (rq-2011-12-09)
        for (Product annotationProduct : annotationProducts) {
            if(annotationProduct.getGeoCoding() != null) {
                ProductUtils.copyGeoCoding(annotationProduct, product);
            }
        }
    }

    private void attachBandsToProduct(OlciL1bManifest manifest, Product product) {
        List<DataSetPointer> measurementPointers = manifest.getDataSetPointers(DataSetPointer.Type.M);
        measurementPointers = removeOrphanedDataSetPointers(measurementPointers);
        bandProducts = createDataSetProducts(measurementPointers);
        addRadianceBands(bandProducts, product);
    }

    private List<Product> createDataSetProducts(List<DataSetPointer> dataSetPointers) {
        List<Product> dataSetProducts = new ArrayList<Product>();
        for (DataSetPointer dataSetPointer : dataSetPointers) {
            try {
                File dataSetFile = new File(getParentInputDirectory(), dataSetPointer.getFileName());
                Product product = ProductIO.readProduct(dataSetFile);
                if (product != null) {
                    dataSetProducts.add(product);
                }else {
                    String msg = String.format("Could not read file '%s. No appropriate reader found.",
                                               dataSetPointer.getFileName());
                    logger.log(Level.WARNING, msg);
                }
            } catch (IOException e) {
                String msg = String.format("Not able to read file '%s.", dataSetPointer.getFileName());
                logger.log(Level.WARNING, msg, e);
            }
        }
        return dataSetProducts;
    }

    private void addRadianceBands(List<Product> bandProducts, Product product) {
        for (final Product bandProduct : bandProducts) {
            for (final Band sourceBand : bandProduct.getBands()) {
                if (hasSameRasterDimension(product, bandProduct)) {
                    String bandName = sourceBand.getName();
                    Band targetBand = ProductUtils.copyBand(bandName, bandProduct, product);
                    targetBand.setSourceImage(sourceBand.getSourceImage());
                    if (bandName.matches("TOA_radiances_Oa[0-2][0-9]")) {
                        final int channel = Integer.parseInt(bandName.substring(16, 18));
                        targetBand.setSpectralBandIndex(channel - 1);
                        targetBand.setSpectralWavelength(spectralWavelengths[channel - 1]);
                        targetBand.setSpectralBandwidth(spectralBandwidths[channel - 1]);
                    }
                }
            }
        }
    }

    private boolean hasSameRasterDimension(Product productOne, Product productTwo) {
        int widthOne = productOne.getSceneRasterWidth();
        int heightOne = productOne.getSceneRasterHeight();
        int widthTwo = productTwo.getSceneRasterWidth();
        int heightTwo = productTwo.getSceneRasterHeight();
        return widthOne == widthTwo && heightOne == heightTwo;
    }

    private List<DataSetPointer> removeOrphanedDataSetPointers(List<DataSetPointer> dataSetPointers) {
        File parentFile = getParentInputDirectory();
        List<DataSetPointer> filteredPointers = new ArrayList<DataSetPointer>();
        for (DataSetPointer dataSetPointer : dataSetPointers) {
            String fileName = dataSetPointer.getFileName();
            File dataSetFile = new File(parentFile, fileName);
            if(!dataSetFile.exists()) {
                String patchedFileName  = patchFileName(fileName);
                dataSetFile = new File(parentFile, patchedFileName);
                dataSetPointer.setFileName(patchedFileName);
            }
            if(dataSetFile.exists()) {
                filteredPointers.add(dataSetPointer);
            }
        }
        return filteredPointers;
    }

    static String patchFileName(String fileName) {
        if(fileName.matches("radianceOa[1-9].nc")) {
            return fileName.substring(0, 8) + "sOa0" + fileName.substring(10);
        }
        if(fileName.matches("radianceOa[1-2][0-9].nc")) {
            return fileName.substring(0, 8) + "sOa" + fileName.substring(10);
        }
        return fileName;
    }

    private OlciL1bManifest createManifestFile(File inputFile) throws IOException {
        InputStream manifestInputStream = new FileInputStream(inputFile);
        try {
            return new OlciL1bManifest(createXmlDocument(manifestInputStream));
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
    protected void readBandRasterDataImpl(int sourceOffsetX, int sourceOffsetY, int sourceWidth, int sourceHeight,
                                          int sourceStepX, int sourceStepY, Band destBand, int destOffsetX,
                                          int destOffsetY, int destWidth, int destHeight, ProductData destBuffer,
                                          ProgressMonitor pm) throws IOException {
        throw new IllegalStateException(String.format("No source to read from for band '%s'.", destBand.getName()));
    }

    @Override
    public void close() throws IOException {
        for (Product bandProduct : bandProducts) {
            bandProduct.dispose();
        }
        for (Product annotationProduct : annotationProducts) {
            annotationProduct.dispose();
        }
        super.close();
    }
}
