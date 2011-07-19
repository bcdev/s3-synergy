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

    public static final String FORMAT_NAME_OLCI_L1B = "OLCI L1b";

    private static final Class[] SUPPORTED_INPUT_TYPES = new Class[]{String.class, File.class};
    private static final String DESCRIPTION = "OLCI L1b SAFE Format";
    private static final String MANIFEST_FILE_EXTENSION = ".xml";
    private static final String[] DEFAULT_FILE_EXTENSIONS = new String[]{MANIFEST_FILE_EXTENSION};
    private static final String[] FORMAT_NAMES = new String[]{FORMAT_NAME_OLCI_L1B};

    @Override
    public DecodeQualification getDecodeQualification(Object input) {
        if (isInputValid(input)) {
            return DecodeQualification.INTENDED;
        } else {
            return DecodeQualification.UNABLE;
        }
    }

    private boolean isInputValid(Object input) {
        File inputFile = new File(input.toString());
        String parentDirectoryName = inputFile.getParentFile().getName();
        return isInputFileNameValid(inputFile.getName()) && isDirectoryNameValid(parentDirectoryName);
    }

    private boolean isInputFileNameValid(String name) {
        return "L1b_EO_manifest.xml".equals(name);
    }

    private boolean isDirectoryNameValid(String parentDirectoryName) {
        Pattern pattern = Pattern.compile(".*OL_1_(ERR|EFR)_.*");
        return pattern.matcher(parentDirectoryName).matches();
    }

    @Override
    public Class[] getInputTypes() {
        return SUPPORTED_INPUT_TYPES;
    }

    @Override
    public ProductReader createReaderInstance() {
        return new OlciLevel1ProductReader(this);
    }

    @Override
    public String[] getFormatNames() {
        return FORMAT_NAMES;
    }

    @Override
    public String[] getDefaultFileExtensions() {
        return DEFAULT_FILE_EXTENSIONS;
    }

    @Override
    public String getDescription(Locale locale) {
        return DESCRIPTION;
    }

    @Override
    public BeamFileFilter getProductFileFilter() {
        return new BeamFileFilter(FORMAT_NAMES[0], MANIFEST_FILE_EXTENSION, DESCRIPTION);
    }

}
