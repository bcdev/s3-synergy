package org.esa.beam.dataio.syn;

import org.esa.beam.framework.dataio.DecodeQualification;
import org.esa.beam.framework.dataio.ProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.util.io.BeamFileFilter;

import java.io.File;
import java.util.Locale;
import java.util.regex.Pattern;

/**
 * PlugIn class which provides a Level 2 Sentinel 3 product reader to the framework.
 *
 * @author Marco Peters
 * @since 1.0
 */
public class SynProductReaderPlugIn implements ProductReaderPlugIn {

    public static final String FORMAT_NAME_SYN = "SYN";

    private static final Class[] SUPPORTED_INPUT_TYPES = new Class[]{String.class, File.class};
    private static final String DESCRIPTION = "SYNSAFE Format";
    private static final String MANIFEST_FILE_EXTENSION = ".xml";
    private static final String[] DEFAULT_FILE_EXTENSIONS = new String[]{MANIFEST_FILE_EXTENSION};
    private static final String[] FORMAT_NAMES = new String[]{FORMAT_NAME_SYN};

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
        return "manifest_SYN.xml".equals(name);
    }

    private boolean isDirectoryNameValid(String parentDirectoryName) {
//        S3A_SY_2_SYN____2013621T10920_2013621T101413_0270_001_002_0001_ARG_D_NT_TDS_01.SAFE
//        S3A_OL_2_LFR_20070425T152940_20070425T153025_00045_026933_000_EST_TEST_00.SAFE
        Pattern pattern = Pattern.compile("S3.?_SY_2_..._.*.SAFE");
        return pattern.matcher(parentDirectoryName).matches();
    }

    @Override
    public Class[] getInputTypes() {
        return SUPPORTED_INPUT_TYPES;
    }

    @Override
    public ProductReader createReaderInstance() {
        return new SynProductReader(this);
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
