/*
 * Copyright (C) 2010 Brockmann Consult GmbH (info@brockmann-consult.de)
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see http://www.gnu.org/licenses/
 */

package org.esa.S3L2PP;

import org.esa.beam.util.io.CsvReader;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.List;
import java.util.Properties;

public class DictionaryGenerator {

    private static final String TARGET_DIR_DEFAULT = "C:\\dev\\projects\\S3L2PP\\src\\main\\resources\\dictionary";
    private static File targetDir;

    public static void main(String[] args) throws Exception {
        targetDir = new File(System.getProperty("targetDir", TARGET_DIR_DEFAULT));
        generateSDRDataset(1, 30);
        generateDataset("L2_SYN_aerosol_angstrom_exponent", "a550");
        generateDataset("L2_SYN_aerosol_model_index_number", "amin");
        generateDataset("L2_SYN_aerosol_optical_thickness", "t550");
        generateDataset("L2_SYN_geo-location", "geolocation");
        generateDataset("L2_SYN_geophysical_atmospheric_data", "tiepoints_meteo");
        generateDataset("L2_SYN_OLCI_sub-sampled_annotations", "tiepoints_olci");
        generateDataset("L2_SYN_SLSTR_nadir_sub-sampled_annotations", "tiepoints_slstr_n");
        generateDataset("L2_SYN_SLSTR_oblique_sub-sampled_annotations", "tiepoints_slstr_o");
        generateDataset("L2_SYN_status_flags", "flags");
        generateDataset("L2_SYN_time_stamps", "time");
    }

    private static void generateDataset(String templateName, String datasetName) throws Exception {
        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", templateName);
        properties.setProperty("dataset_name", datasetName);
        generateDataset(properties);
    }

    private static void generateSDRDataset(int minChannel, int maxChannel) throws Exception {
        InputStream csv = DictionaryGenerator.class.getResourceAsStream("wavelengths.csv");
        final CsvReader csvReader = new CsvReader(new InputStreamReader(csv), new char[]{','});
        final List<String[]> wavelengths = csvReader.readStringRecords();
        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "L2_SYN_Surface_Directional_Reflectance");
        properties.setProperty("SDR", "SDR_${i}");
        properties.setProperty("SDR_er", "SDR_${i}_er");
        for (int i = minChannel; i <= maxChannel; i++) {
            String[] wavelengthSet = wavelengths.get(i - 1);
            float temp = Float.parseFloat(wavelengthSet[1]);
            int roundedCentralWavelength = Math.round(temp);
            properties.setProperty("central_wavelength", wavelengthSet[1]);
            properties.setProperty("min_wavelength", wavelengthSet[2]);
            properties.setProperty("max_wavelength", wavelengthSet[3]);
            String suffix = "";
            if (i > 18) {
                suffix = "n";
            }
            if (i > 24) {
                suffix = "o";
            }
            properties.setProperty("dataset_name", "r0" + roundedCentralWavelength + suffix);
            properties.setProperty("i", String.format("%d", i));
            generateDataset(properties);
        }
    }

    private static void generateDataset(Properties properties) throws Exception {
        final TemplateResolver resolver = new TemplateResolver(properties);
        final File targetFile = new File(targetDir, resolver.resolveProperty("dataset_name") + ".xml");
        BufferedReader reader = null;
        BufferedWriter writer = null;
        try {
            final String templateName = resolver.resolveProperty("Template_File_Basename");
            final InputStream is = DictionaryGenerator.class.getResourceAsStream(templateName + ".xml");
            reader = new BufferedReader(new InputStreamReader(is, "US-ASCII"));
            writer = new BufferedWriter(new FileWriter(targetFile));
            String line = reader.readLine();
            while (line != null) {
                line = resolver.resolve(line);
                writer.write(line + "\n");
                line = reader.readLine();
            }
        } finally {
            try {
                if (writer != null) {
                    writer.close();
                }
                if (reader != null) {
                    reader.close();
                }
            } catch (IOException e) {
                // ignore
            }
        }
    }

}
