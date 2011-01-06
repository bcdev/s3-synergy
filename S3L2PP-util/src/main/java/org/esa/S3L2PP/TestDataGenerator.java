package org.esa.S3L2PP;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.MessageFormat;
import java.util.Properties;

/**
 * Dummy test data generator.
 */
public class TestDataGenerator {

    private static final String GLOBAL_ATTRIBUTES = "// global attributes:\n" +
                                                    "\t\t:Conventions = \"CF-1.4\" ;\n" +
                                                    "\t\t:title = \"SYN L1c dummy test data\" ;\n" +
                                                    "\t\t:institution = \"Brockmann Consult GmbH\" ;\n" +
                                                    "\t\t:source = \"Sentinel-3 SYN\" ;\n" +
                                                    "\t\t:history = \"\" ;\n" +
                                                    "\t\t:comment = \"\" ;\n" +
                                                    "\t\t:references = \"S3-RS-TAF-SY-01247\" ;\n" +
                                                    "\t\t:contact = \"info@brockmann-consult.de\" ;\n" +
                                                    "\t\t:netCDF_version = \"netCDF-4\" ;\n" +
                                                    "\t\t:Data_set_name = \"${CDL_File_Basename}.nc\" ;";

    private static final String CDL_TARGET_DIR_DEFAULT = "testdata/cdl/dummy";
    private static final String NC_TARGET_DIR_DEFAULT = "testdata/nc/dummy";
    private static final String NCGEN_PATH_DEFAULT = "/usr/local/bin/ncgen";

    private static final File CDL_TARGET_DIR;
    private static final File NC_TARGET_DIR;
    private static final String NCGEN_PATH;

    static {
        CDL_TARGET_DIR = new File(System.getProperty("cdlDir", CDL_TARGET_DIR_DEFAULT));
        NC_TARGET_DIR = new File(System.getProperty("ncDir", NC_TARGET_DIR_DEFAULT));
        NCGEN_PATH = System.getProperty("ncgenPath", NCGEN_PATH_DEFAULT);
    }

    public static void main(String[] args) {
        try {
            generateDummyOlciRadianceDatasets();
            generateDummySlstrRadianceDatasets();
            generateDataset("GEOLOCATION_REF");
            generateDataset("TIME_STAMP_OLC");
            generateDataset("PIX_ANNOT_OLC");
            generateDummySlstrFlagsDatasets();
            generateDataset("SUBS_ANNOT_GEOM_OLC");
            generateDataset("SUBS_ANNOT_METEO_OLC");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void generateDummyOlciRadianceDatasets() throws Exception {
        final Properties properties = createProperties();
        properties.setProperty("Template_File_Basename", "OLC_RADIANCE_O");
        generateDatasets(1, 21, properties);
    }

    private static void generateDummySlstrRadianceDatasets() throws Exception {
        final Properties properties = createProperties();
        properties.setProperty("Template_File_Basename", "SLST_NAD_RADIANCE_S");
        generateDatasets(1, 6, properties);

        properties.setProperty("Template_File_Basename", "SLST_ALT_RADIANCE_S");
        generateDatasets(1, 6, properties);
    }

    private static void generateDummySlstrFlagsDatasets() throws Exception {
        final Properties properties = createProperties();
        properties.setProperty("Template_File_Basename", "PIX_ANNOT_SLST_NAD");
        properties.setProperty("CDL_File_Basename", "${Template_File_Basename}_${A_or_TDI}");
        properties.setProperty("A_or_TDI", "A");
        generateDataset(properties);
        properties.setProperty("A_or_TDI", "TDI");
        generateDataset(properties);

        properties.setProperty("Template_File_Basename", "PIX_ANNOT_SLST_ALT");
        properties.setProperty("A_or_TDI", "A");
        generateDataset(properties);
        properties.setProperty("A_or_TDI", "TDI");
        generateDataset(properties);
    }

    private static void generateDataset(String templateName) throws Exception {
        final Properties properties = createProperties();
        properties.setProperty("Template_File_Basename", templateName);
        generateDataset(properties);
    }

    private static void generateDatasets(int minChannel, int maxChannel, Properties properties) throws Exception {
        properties.setProperty("CDL_File_Basename", "${Template_File_Basename}${i}");
        for (int i = minChannel; i <= maxChannel; i++) {
            properties.setProperty("i", String.format("%d", i));
            properties.setProperty("ii", String.format("%02d", i));
            generateDataset(properties);
        }
    }

    private static void generateDataset(Properties properties) throws Exception {
        final TemplateResolver resolver = new TemplateResolver(properties);
        final File cdlFile = new File(CDL_TARGET_DIR, resolver.resolveProperty("CDL_File_Basename") + ".cdl");
        BufferedReader reader = null;
        BufferedWriter writer = null;
        try {
            final String templateName = resolver.resolveProperty("Template_File_Basename");
            final InputStream is = TestDataGenerator.class.getResourceAsStream(templateName + ".cdl");
            reader = new BufferedReader(new InputStreamReader(is, "US-ASCII"));
            writer = new BufferedWriter(new FileWriter(cdlFile));
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
        generateNcFile(cdlFile);
    }

    private static Properties createProperties() {
        final Properties properties = new Properties();
        properties.setProperty("Global_Attributes", GLOBAL_ATTRIBUTES);
        properties.setProperty("CDL_File_Basename", "${Template_File_Basename}");
        return properties;
    }

    private static void generateNcFile(File cdlFile) throws Exception {
        final File ncFile = new File(NC_TARGET_DIR, cdlFile.getName().replace(".cdl", ".nc"));
        final String command = NCGEN_PATH + " -k 3 -o " + ncFile.getPath() + " " + cdlFile.getPath();
        final Process process = Runtime.getRuntime().exec(command);
        if (process.waitFor() != 0) {
            throw new Exception(
                    MessageFormat.format("process <code>{0}</code> terminated with exit value {1}",
                                         command, process.exitValue()));
        }
    }
}
