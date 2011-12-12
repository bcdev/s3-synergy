package org.esa.beam.dataio.slstr;

import org.esa.beam.framework.dataio.DecodeQualification;
import org.esa.beam.framework.dataio.ProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.util.io.BeamFileFilter;

import java.util.Locale;

/**
 * PlugIn class which provides a Level 2 Sentinel 3 SLSTR product reader to the framework.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class SlstrLevel2ProductReaderPlugin implements ProductReaderPlugIn {
    @Override
    public DecodeQualification getDecodeQualification(Object input) {
        // todo
        return null;
    }

    @Override
    public Class[] getInputTypes() {
        // todo
        return new Class[0];
    }

    @Override
    public ProductReader createReaderInstance() {
        // todo
        return null;
    }

    @Override
    public String[] getFormatNames() {
        // todo
        return new String[0];
    }

    @Override
    public String[] getDefaultFileExtensions() {
        // todo
        return new String[0];
    }

    @Override
    public String getDescription(Locale locale) {
        // todo
        return null;
    }

    @Override
    public BeamFileFilter getProductFileFilter() {
        // todo
        return null;
    }
}
