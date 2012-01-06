package org.esa.beam.dataio.slstr;

import org.esa.beam.framework.dataio.DecodeQualification;
import org.esa.beam.framework.dataio.ProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.util.io.BeamFileFilter;

import java.io.File;
import java.util.Locale;
import java.util.regex.Pattern;

/**
 * PlugIn class which provides a Level 2 Sentinel 3 SLSTR product reader to the framework.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class SlstrLevel2LndProductReaderPlugIn implements ProductReaderPlugIn {

    public static final String FORMAT_NAME_SLSTR_L2 = "SLSTR-L2-LST";

    private static final Class[] SUPPORTED_INPUT_TYPES = new Class[]{String.class, File.class};
    private static final String DESCRIPTION = "SLSTR L2b SAFE Format";
    private static final String MANIFEST_FILE_EXTENSION = ".xml";      // todo: discuss
    private static final String[] DEFAULT_FILE_EXTENSIONS = new String[]{MANIFEST_FILE_EXTENSION};
    private static final String[] FORMAT_NAMES = new String[]{FORMAT_NAME_SLSTR_L2};


    @Override
    public DecodeQualification getDecodeQualification(Object input) {
        if (isInputValid(input)) {
            return DecodeQualification.INTENDED;
        } else {
            return DecodeQualification.UNABLE;
        }
    }

    @Override
    public Class[] getInputTypes() {
        return SUPPORTED_INPUT_TYPES;
    }

    @Override
    public ProductReader createReaderInstance() {
        return new SlstrLevel2LndProductReader(this);
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


    private boolean isInputValid(Object input) {
        File inputFile = new File(input.toString());
        String parentDirectoryName = inputFile.getParentFile().getName();
        final boolean inputFileNameValid = isInputFileNameValid(inputFile.getName());
        final boolean directoryNameValid = isDirectoryNameValid(parentDirectoryName);
        return inputFileNameValid && directoryNameValid;
    }

    private boolean isInputFileNameValid(String name) {
        final String manifestFileName = "manifest".concat(MANIFEST_FILE_EXTENSION);
        return manifestFileName.equals(name);
    }

    private boolean isDirectoryNameValid(String parentDirectoryName) {
        Pattern pattern = Pattern.compile("S3.?_SL_2_LST..._.*.SAFE");
        return pattern.matcher(parentDirectoryName).matches();
    }

}

