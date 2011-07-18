package org.esa.s3.dataio;

import com.bc.ceres.core.ProgressMonitor;
import org.esa.beam.framework.dataio.IllegalFileFormatException;
import org.esa.beam.framework.dataio.ProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.framework.dataio.ProductSubsetDef;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.ProductData;

import java.io.IOException;

/**
 * Product reader resonsible for reading OLCI L1b data products in SAFE format.
 *
 * @author Marco Peters
 * @since 1.0
 */
public class OlciLevel1ProductReader implements ProductReader {

    @Override
    public ProductReaderPlugIn getReaderPlugIn() {
        return null;
    }

    @Override
    public Object getInput() {
        return null;
    }

    @Override
    public ProductSubsetDef getSubsetDef() {
        return null;
    }

    @Override
    public Product readProductNodes(Object input, ProductSubsetDef subsetDef) throws IOException,
                                                                                     IllegalFileFormatException {
        return null;
    }

    @Override
    public void readBandRasterData(Band destBand, int destOffsetX, int destOffsetY, int destWidth, int destHeight,
                                   ProductData destBuffer, ProgressMonitor pm) throws IOException {
    }

    @Override
    public void close() throws IOException {
    }
}
