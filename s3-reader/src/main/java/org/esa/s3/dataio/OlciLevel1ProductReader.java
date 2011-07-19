package org.esa.s3.dataio;

import com.bc.ceres.core.ProgressMonitor;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductIO;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.ProductData;
import org.esa.beam.util.ProductUtils;
import org.hsqldb.Index;
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
 * Product reader responsible for reading OLCI L1b data products in SAFE format.
 *
 * @author Marco Peters
 * @since 1.0
 */
class OlciLevel1ProductReader extends AbstractProductReader {

    private List<Product> bandProducts;
    private Logger logger;

    protected OlciLevel1ProductReader(OlciLevel1ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);

        logger = Logger.getLogger(getClass().getSimpleName());
    }

    @Override
    protected Product readProductNodesImpl() throws IOException {
        File inputFile = getInputFile();
        Manifest manifest = createManifestFile(inputFile);
        return createProduct(manifest);
    }

    private Product createProduct(Manifest manifest) {
        Product product = new Product(manifest.getProductName(), manifest.getProductType(),
                                      manifest.getColumnCount(), manifest.getLineCount());

        List<DataSetPointer> dataSetPointers = manifest.getDataSetPointers(DataSetPointer.Type.M);
        dataSetPointers = removeNotExistingDataSetPointers(dataSetPointers);
        bandProducts = createBandProducts(dataSetPointers);
        addBands(bandProducts, product);
        return product;
    }

    private List<Product> createBandProducts(List<DataSetPointer> dataSetPointers) {
        List<Product> bandProducts = new ArrayList<Product>();
        for (DataSetPointer dataSetPointer : dataSetPointers) {
            try {
                bandProducts.add(ProductIO.readProduct(new File(getParentInputDirectory(), dataSetPointer.getFileName())));
            } catch (IOException e) {
                String msg = String.format("Not able to read file '%s.", dataSetPointer.getFileName());
                logger.log(Level.WARNING, msg, e);
            }
        }
        return bandProducts;
    }

    private void addBands(List<Product> bandProducts, Product product) {
        for (Product bandProduct : bandProducts) {
            Band[] bands = bandProduct.getBands();
            for (Band band : bands) {
                Band targetBand = ProductUtils.copyBand(band.getName(), bandProduct, product);
                targetBand.setSourceImage(band.getSourceImage());
            }
        }

    }

    private List<DataSetPointer> removeNotExistingDataSetPointers(List<DataSetPointer> dataSetPointers) {
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

    private String patchFileName(String fileName) {
        if(isFileNameValid(fileName)) {
            return fileName;
        }

        int dotIndex = fileName.indexOf('.');
        char firstDigit = fileName.charAt(dotIndex - 2);
        if(!Character.isDigit(firstDigit)) {
            firstDigit = '0';
        }
        String index = String.format("%s%s", firstDigit, fileName.charAt(dotIndex - 1));
        return "radiancesOa" + index + ".nc";
    }

    private boolean isFileNameValid(String fileName) {
        return fileName.indexOf('.') == 13;
    }

    private Manifest createManifestFile(File inputFile) throws IOException {
        InputStream manifestInputStream = new FileInputStream(inputFile);
        try {
            return new Manifest(createXmlDocument(manifestInputStream));
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
    }

    @Override
    public void close() throws IOException {
        for (Product bandProduct : bandProducts) {
            bandProduct.dispose();
        }
        super.close();

    }
}
