package org.esa.s3.dataio;

import org.esa.beam.framework.dataio.DecodeQualification;
import org.esa.beam.framework.dataio.ProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.util.io.BeamFileFilter;

import java.io.File;
import java.util.Locale;
import java.util.regex.Pattern;

/**
 * PlugIn class which provides the Sentinel 3 product reader to the framework.
 *
 * @author Marco Peters
 * @since 1.0
 */
public class OlciLevel1ProductReaderPlugIn implements ProductReaderPlugIn {

    private static final Class[] SUPPORTED_INPUT_TYPES = new Class[]{String.class, File.class};
    private static final String description = "OLCI L1b Product Reader";

    @Override
    public DecodeQualification getDecodeQualification(Object input) {
        File inputFile = new File(input.toString());
        String parentDirectoryName = inputFile.getParentFile().getName();
        if (isDirectoryNameValid(parentDirectoryName)) {
            return DecodeQualification.INTENDED;
        } else {
            return DecodeQualification.UNABLE;
        }
    }

    private boolean isDirectoryNameValid(String parentDirectoryName) {
        Pattern pattern = Pattern.compile(".*OL_1_(ERR|EFR)_.*.SAFE");
        return pattern.matcher(parentDirectoryName).matches();
    }

    @Override
    public Class[] getInputTypes() {
        return SUPPORTED_INPUT_TYPES;
    }

    @Override
    public ProductReader createReaderInstance() {
        return new OlciLevel1ProductReader();
    }

    @Override
    public String[] getFormatNames() {
        return new String[0];
    }

    @Override
    public String[] getDefaultFileExtensions() {
        return new String[0];
    }

    @Override
    public String getDescription(Locale locale) {
        return description;
    }

    @Override
    public BeamFileFilter getProductFileFilter() {
        return new BeamFileFilter("OLCI L1b", ".nc", "OLCI L1b SAFE format");
    }

}
