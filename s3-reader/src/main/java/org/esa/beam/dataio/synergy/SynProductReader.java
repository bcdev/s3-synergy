package org.esa.beam.dataio.synergy;

import com.bc.ceres.core.ProgressMonitor;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductIO;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.CrsGeoCoding;
import org.esa.beam.framework.datamodel.MetadataElement;
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
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

abstract class SynProductReader extends AbstractProductReader {

    private final List<Product> openProductList = new ArrayList<Product>();

    private final Logger logger;

    protected SynProductReader(ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
        logger = Logger.getLogger(getClass().getSimpleName());
    }

    @Override
    protected final Product readProductNodesImpl() throws IOException {
        final File inputFile = getInputFile();
        final Manifest manifest = createManifestFile(inputFile);

        return createProduct(manifest);
    }

    @Override
    protected final void readBandRasterDataImpl(int sourceOffsetX, int sourceOffsetY, int sourceWidth, int sourceHeight,
                                                int sourceStepX, int sourceStepY, Band destBand, int destOffsetX,
                                                int destOffsetY, int destWidth, int destHeight, ProductData destBuffer,
                                                ProgressMonitor pm) throws IOException {
        throw new IllegalStateException("Data are provided by images.");
    }

    @Override
    public final void close() throws IOException {
        for (final Product product : openProductList) {
            product.dispose();
        }
        openProductList.clear();
        super.close();
    }

    protected abstract List<String> getFileNames(Manifest manifest);

    protected void attachBandData(Band sourceBand, Product targetProduct) {
        ProductUtils.copyBand(sourceBand.getName(), sourceBand.getProduct(), targetProduct, true);
    }

    protected void attachTiePointData(Band sourceBand, Product targetProduct) {
    }

    protected void attachGeoCoding(Product targetProduct) throws IOException {
    }

    private Product createProduct(Manifest manifest) throws IOException {
        readProducts(getFileNames(manifest));

        final String productName = getProductName();
        final String productType = getReaderPlugIn().getFormatNames()[0];

        final Product sourceProduct = openProductList.get(0);
        final int w = sourceProduct.getSceneRasterWidth();
        final int h = sourceProduct.getSceneRasterHeight();
        final Product targetProduct = new Product(productName, productType, w, h, this);

        final ProductData.UTC startTime = sourceProduct.getStartTime();
        final ProductData.UTC endTime = sourceProduct.getEndTime();
        if (startTime == null || endTime == null) {
            targetProduct.setStartTime(manifest.getStartTime());
            targetProduct.setEndTime(manifest.getStopTime());
        } else {
            targetProduct.setStartTime(startTime);
            targetProduct.setEndTime(endTime);
        }
        targetProduct.setFileLocation(getInputFile());

        if (sourceProduct.getGeoCoding() instanceof CrsGeoCoding) {
            ProductUtils.copyGeoCoding(sourceProduct, targetProduct);
        }

        attachData(targetProduct);
        attachGeoCoding(targetProduct);

        final MetadataElement globalAttributes = new MetadataElement("Global_Attributes");
        final MetadataElement variableAttributes = new MetadataElement("Variable_Attributes");
        ProductUtils.copyMetadata(sourceProduct.getMetadataRoot().getElement("Global_Attributes"), globalAttributes);
        globalAttributes.setAttributeString("dataset_name", productName);
        for (final Product p : openProductList) {
            for (final MetadataElement element : p.getMetadataRoot().getElement("Variable_Attributes").getElements()) {
                variableAttributes.addElement(element.createDeepClone());
            }
        }
        targetProduct.getMetadataRoot().addElement(globalAttributes);
        targetProduct.getMetadataRoot().addElement(variableAttributes);

        return targetProduct;
    }

    private void readProducts(List<String> fileNames) throws IOException {
        for (final String fileName : fileNames) {
            readProduct(fileName);
        }
    }

    private Product readProduct(String fileName) throws IOException {
        final File file = new File(getInputFileParentDirectory(), fileName);
        final Product product = ProductIO.readProduct(file);
        if (product == null) {
            final String msg = MessageFormat.format("Cannot read file ''{0}''. No appropriate reader found.", fileName);
            logger.log(Level.SEVERE, msg);
            throw new IOException(msg);
        }
        openProductList.add(product);
        return product;
    }

    private File getInputFile() {
        return new File(getInput().toString());
    }

    private File getInputFileParentDirectory() {
        return getInputFile().getParentFile();
    }

    private void attachData(Product product) {
        final int w = product.getSceneRasterWidth();
        final int h = product.getSceneRasterHeight();

        final StringBuilder patternBuilder = new StringBuilder();
        for (final Product childProduct : openProductList) {
            if (childProduct.getAutoGrouping() != null) {
                if (patternBuilder.length() > 0) {
                    patternBuilder.append(":");
                }
                patternBuilder.append(childProduct.getAutoGrouping());
            }
            for (final Band band : childProduct.getBands()) {
                if (band.getSceneRasterWidth() == w && band.getSceneRasterHeight() == h) {
                    attachBandData(band, product);
                } else {
                    attachTiePointData(band, product);
                }
            }
        }
        product.setAutoGrouping(patternBuilder.toString());
    }

    private String getProductName() {
        return FileUtils.getFilenameWithoutExtension(getInputFileParentDirectory());
    }

    private Manifest createManifestFile(File inputFile) throws IOException {
        final InputStream manifestInputStream = new FileInputStream(inputFile);
        try {
            return Manifest.createManifest(createXmlDocument(manifestInputStream));
        } finally {
            manifestInputStream.close();
        }
    }

    private Document createXmlDocument(InputStream inputStream) throws IOException {
        final String msg = "Cannot create document from manifest XML file.";

        try {
            return DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(inputStream);
        } catch (SAXException e) {
            logger.log(Level.SEVERE, msg, e);
            throw new IOException(msg, e);
        } catch (ParserConfigurationException e) {
            logger.log(Level.SEVERE, msg, e);
            throw new IOException(msg, e);
        }
    }
}
