package org.esa.beam.dataio.slstr;

import com.bc.ceres.core.ProgressMonitor;
import org.esa.beam.framework.dataio.AbstractProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.ProductData;

import java.io.File;
import java.io.IOException;

/**
 * Product reader responsible for reading SLSTR L1b data products in SAFE format.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class SlstrLevel1ProductReader extends AbstractProductReader {
    /**
     * Constructs a new abstract product reader.
     *
     * @param readerPlugIn the reader plug-in which created this reader, can be <code>null</code> for internal reader
     *                     implementations
     */
    protected SlstrLevel1ProductReader(ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
    }

    @Override
    protected Product readProductNodesImpl() throws IOException {
        // todo: complete
        File inputDir = getInputDirectory();
        return createProduct(inputDir);
    }

    @Override
    protected void readBandRasterDataImpl(int sourceOffsetX, int sourceOffsetY, int sourceWidth, int sourceHeight, int sourceStepX, int sourceStepY, Band destBand, int destOffsetX, int destOffsetY, int destWidth, int destHeight, ProductData destBuffer, ProgressMonitor pm) throws IOException {
        // todo
    }

    private Product createProduct(File inputDir) {
        // todo
        return null;
    }

    private File getInputDirectory() {
        return new File(getInput().toString());
    }

}
