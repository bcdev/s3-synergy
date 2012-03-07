/*
 * Copyright (c) 2012. Brockmann Consult (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

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
    private static final String DESCRIPTION = "Sentinel-3 SLSTR Level-2 LST product";
    private static final String[] DEFAULT_FILE_EXTENSIONS = new String[]{".safe", ".xml"};
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
        return new BeamFileFilter(FORMAT_NAMES[0], DEFAULT_FILE_EXTENSIONS, DESCRIPTION);
    }


    private boolean isInputValid(Object input) {
        File inputFile = new File(input.toString());
        String parentDirectoryName = inputFile.getParentFile().getName();
        final boolean inputFileNameValid = isInputFileNameValid(inputFile.getName());
        final boolean directoryNameValid = isDirectoryNameValid(parentDirectoryName);
        return inputFileNameValid && directoryNameValid;
    }

    private boolean isInputFileNameValid(String name) {
        return "manifest.safe".equals(name) || "manifest.xml".equals(name);
    }

    private boolean isDirectoryNameValid(String parentDirectoryName) {
        Pattern pattern = Pattern.compile("S3.?_SL_2_LST..._.*.SAFE");
        return pattern.matcher(parentDirectoryName).matches();
    }

}

