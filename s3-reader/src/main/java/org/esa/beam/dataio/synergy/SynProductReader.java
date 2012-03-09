package org.esa.beam.dataio.synergy;

import com.bc.ceres.core.ProgressMonitor;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductIO;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.framework.datamodel.Band;
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
    private Manifest manifest;

    protected SynProductReader(ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
        logger = Logger.getLogger(getClass().getSimpleName());
    }

    protected Manifest getManifest() {
        return manifest;
    }

    @Override
    protected final Product readProductNodesImpl() throws IOException {
        final File inputFile = getInputFile();
        manifest = createManifestFile(inputFile);

        return createProduct(manifest);
    }

    @Override
    protected final void readBandRasterDataImpl(int sourceOffsetX, int sourceOffsetY, int sourceWidth, int sourceHeight,
                                                int sourceStepX, int sourceStepY, Band destBand, int destOffsetX,
                                                int destOffsetY, int destWidth, int destHeight, ProductData destBuffer,
                                                ProgressMonitor pm) throws IOException {
        throw new IllegalStateException("Data are provided by images");
    }

    @Override
    public void close() throws IOException {
        for (final Product product : openProductList) {
            product.dispose();
        }
        super.close();
    }

    private Product createProduct(Manifest manifest) throws IOException {
        readProducts(manifest.getMeasurementFileNames());

        final String productName = getProductName();
        final String productType = getReaderPlugIn().getFormatNames()[0];

        final Product childProduct = openProductList.get(0);
        final int w = childProduct.getSceneRasterWidth();
        final int h = childProduct.getSceneRasterHeight();
        final Product product = new Product(productName, productType, w, h, this);

        final ProductData.UTC startTime = childProduct.getStartTime();
        final ProductData.UTC endTime = childProduct.getEndTime();
        if (startTime == null || endTime == null) {
            product.setStartTime(manifest.getStartTime());
            product.setEndTime(manifest.getStopTime());
        } else {
            product.setStartTime(startTime);
            product.setEndTime(endTime);
        }
        product.setFileLocation(getInputFile());

        attachMeasurementBands(product);
        attachAnnotationData(manifest, product);

        final MetadataElement globalAttributes = new MetadataElement("Global_Attributes");
        final MetadataElement variableAttributes = new MetadataElement("Variable_Attributes");
        ProductUtils.copyMetadata(childProduct.getMetadataRoot().getElement("Global_Attributes"), globalAttributes);
        globalAttributes.setAttributeString("dataset_name", productName);
        for (final Product p : openProductList) {
            for (final MetadataElement element : p.getMetadataRoot().getElement("Variable_Attributes").getElements()) {
                variableAttributes.addElement(element.createDeepClone());
            }
        }
        product.getMetadataRoot().addElement(globalAttributes);
        product.getMetadataRoot().addElement(variableAttributes);

        if (childProduct.getGeoCoding() != null) {
            ProductUtils.copyGeoCoding(childProduct, product);
        }

        return product;
    }

    protected abstract void attachAnnotationData(Manifest manifest, Product product) throws IOException;

    protected void readProducts(List<String> fileNames) throws IOException {
        for (final String fileName : fileNames) {
            readProduct(fileName);
        }
    }

    protected final Product readProduct(String fileName) throws IOException {
        final File file = new File(getInputFileParentDirectory(), fileName);
        final Product product = ProductIO.readProduct(file);
        if (product == null) {
            throw new IOException(
                    MessageFormat.format("Cannot read file ''{0}''. No appropriate reader found.", fileName));
        }
        openProductList.add(product);
        return product;
    }

    protected final File getInputFile() {
        return new File(getInput().toString());
    }

    protected final File getInputFileParentDirectory() {
        return getInputFile().getParentFile();
    }

    private void attachMeasurementBands(Product product) {
        final StringBuilder patternBuilder = new StringBuilder();
        for (final Product childProduct : openProductList) {
            if (childProduct.getAutoGrouping() != null) {
                if (patternBuilder.length() > 0) {
                    patternBuilder.append(":");
                }
                patternBuilder.append(childProduct.getAutoGrouping());
            }
            for (final String bandName : childProduct.getBandNames()) {
                ProductUtils.copyBand(bandName, childProduct, product, true);
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
