package org.esa.beam.dataio.synergy;

import org.esa.beam.framework.dataio.DecodeQualification;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.util.io.BeamFileFilter;

import java.io.File;
import java.util.Locale;
import java.util.regex.Pattern;

abstract class SynProductReaderPlugIn implements ProductReaderPlugIn {

    private static final Class[] SUPPORTED_INPUT_TYPES = new Class[]{String.class, File.class};
    private static final String[] DEFAULT_FILE_EXTENSIONS = new String[]{".safe", ".xml"};

    private final String formatName;
    private final Pattern directoryNamePattern;
    private final String description;
    private final String[] formatNames;

    protected SynProductReaderPlugIn(String formatName, String directoryNamePattern, String description) {
        this.formatName = formatName;
        this.directoryNamePattern = Pattern.compile(directoryNamePattern);
        this.description = description;
        this.formatNames = new String[]{formatName};
    }

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
    public String[] getFormatNames() {
        return formatNames;
    }

    @Override
    public String[] getDefaultFileExtensions() {
        return DEFAULT_FILE_EXTENSIONS;
    }

    @Override
    public String getDescription(Locale locale) {
        return description;
    }

    @Override
    public BeamFileFilter getProductFileFilter() {
        return new BeamFileFilter(formatName, DEFAULT_FILE_EXTENSIONS, description);
    }

    private boolean isInputValid(Object input) {
        final File inputFile = new File(input.toString());
        final String parentDirectoryName = inputFile.getParentFile().getName();
        return isValidInputFileName(inputFile.getName()) && isValidDirectoryName(parentDirectoryName);
    }

    private boolean isValidInputFileName(String name) {
        return "manifest.safe".equals(name.toLowerCase()) || "manifest.xml".equals(name.toLowerCase());
    }

    private boolean isValidDirectoryName(String parentDirectoryName) {
        return directoryNamePattern.matcher(parentDirectoryName).matches();
    }
}
