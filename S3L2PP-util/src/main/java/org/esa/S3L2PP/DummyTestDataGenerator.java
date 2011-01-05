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
public class DummyTestDataGenerator {

    private static final String NCGEN_PATH_DEFAULT = "/usr/local/bin/ncgen";
    private static final String NCGEN_PATH;

    static {
        NCGEN_PATH = System.getProperty("ncgenPath", NCGEN_PATH_DEFAULT);
    }

    public static void main(String[] args) {
        try {
            generateDummyOlciRadianceDatasets();
            generateDummySlstrRadianceDatasets();
            generateDummyGeolocationDataset();
            generateDummyOlciTimeStampDataset();
            generateDummyOlciFlagsDataset();
            generateDummySlstrFlagsDatasets();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static void generateDummyOlciRadianceDatasets() throws Exception {
        final Properties properties = createDefaultProperties();
        properties.setProperty("TEMPLATE_NAME", "OLC_RADIANCE_O");
        generateDatasets(1, 21, properties);
    }

    private static void generateDummySlstrRadianceDatasets() throws Exception {
        final Properties properties = createDefaultProperties();
        properties.setProperty("TEMPLATE_NAME", "SLST_NAD_RADIANCE_S");
        generateDatasets(1, 6, properties);

        properties.setProperty("TEMPLATE_NAME", "SLST_ALT_RADIANCE_S");
        generateDatasets(1, 6, properties);
    }

    private static void generateDummyGeolocationDataset() throws Exception {
        final Properties properties = createDefaultProperties();
        properties.setProperty("TEMPLATE_NAME", "GEOLOCATION_REF");
        generateDataset(properties);
    }

    private static void generateDummyOlciTimeStampDataset() throws Exception {
        final Properties properties = createDefaultProperties();
        properties.setProperty("TEMPLATE_NAME", "TIME_STAMP_OLC");
        generateDataset(properties);
    }

    private static void generateDummyOlciFlagsDataset() throws Exception {
        final Properties properties = createDefaultProperties();
        properties.setProperty("TEMPLATE_NAME", "TIME_STAMP_OLC");
        generateDataset(properties);
    }

    private static void generateDummySlstrFlagsDatasets() throws Exception {
        final Properties properties = createDefaultProperties();
        properties.setProperty("TEMPLATE_NAME", "PIX_ANNOT_SLST_NAD");
        properties.setProperty("CDL_FILE_NAME", "${TEMPLATE_NAME}_${A_OR_TDI}");
        properties.setProperty("A_OR_TDI", "A");
        generateDataset(properties);
        properties.setProperty("A_OR_TDI", "TDI");
        generateDataset(properties);

        properties.setProperty("TEMPLATE_NAME", "PIX_ANNOT_SLST_ALT");
        properties.setProperty("A_OR_TDI", "A");
        generateDataset(properties);
        properties.setProperty("A_OR_TDI", "TDI");
        generateDataset(properties);
    }

    private static void generateDataset(Properties properties) throws Exception {
        final TemplateResolver resolver = new TemplateResolver(properties);
        final File cdlFile = new File(resolver.getProperty("CDL_DIR"),
                                      resolver.getProperty("CDL_FILE_NAME") + ".cdl");
        BufferedReader reader = null;
        BufferedWriter writer = null;
        try {
            final String templateName = resolver.getProperty("TEMPLATE_NAME");
            final InputStream is = DummyTestDataGenerator.class.getResourceAsStream(templateName + ".cdl");
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
        final File ncFile = new File(resolver.getProperty("NC_DIR"), cdlFile.getName().replace(".cdl", ".nc"));
        final String command = NCGEN_PATH + " -k 3 -o " + ncFile.getPath() + " " + cdlFile.getPath();
        final Process process = Runtime.getRuntime().exec(command);
        if (process.waitFor() != 0) {
            throw new Exception(
                    MessageFormat.format("process <code>{0}</code> terminated with exit value {1}",
                                         command, process.exitValue()));
        }
    }

    private static void generateDatasets(int first, int last, Properties properties) throws Exception {
        final TemplateResolver resolver = new TemplateResolver(properties);
        for (int i = first; i <= last; i++) {
            final File cdlFile = new File(resolver.getProperty("CDL_DIR"),
                                          resolver.getProperty("CDL_FILE_NAME") + i + ".cdl");
            BufferedReader reader = null;
            BufferedWriter writer = null;
            try {
                final String templateName = resolver.getProperty("TEMPLATE_NAME");
                final InputStream is = DummyTestDataGenerator.class.getResourceAsStream(templateName + ".cdl");
                reader = new BufferedReader(new InputStreamReader(is, "US-ASCII"));
                writer = new BufferedWriter(new FileWriter(cdlFile));
                String line = reader.readLine();
                while (line != null) {
                    properties.setProperty("i", String.valueOf(i));
                    if (i < 10) {
                        properties.setProperty("ii", "0${i}");
                    } else {
                        properties.setProperty("ii", "${i}");
                    }
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
            final File ncFile = new File(resolver.getProperty("NC_DIR"), cdlFile.getName().replace(".cdl", ".nc"));
            final String command = NCGEN_PATH + " -k 3 -o " + ncFile.getPath() + " " + cdlFile.getPath();
            final Process process = Runtime.getRuntime().exec(command);
            if (process.waitFor() != 0) {
                throw new Exception(
                        MessageFormat.format("process <code>{0}</code> terminated with exit value {1}",
                                             command, process.exitValue()));
            }
        }
    }

    private static Properties createDefaultProperties() {
        final Properties properties = new Properties();
        properties.setProperty("CDL_FILE_NAME", "${TEMPLATE_NAME}");
        properties.setProperty("CDL_DIR", "testdata/cdl/dummy");
        properties.setProperty("NC_DIR", "testdata/nc/dummy");

        return properties;
    }
}

