/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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

package org.esa.s3.synergy.util.adf;

import org.esa.beam.util.math.LookupTable;
import org.esa.s3.synergy.util.TemplateResolver;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;
import java.util.Properties;
import java.util.Scanner;

class LookupTableGenerator {

    private static final double[] SYN_WAV = new double[]{
            400.00,
            412.50,
            442.50,
            490.00,
            510.00,
            560.00,
            620.00,
            665.00,
            673.75,
            681.25,
            708.75,
            753.75,
            761.25,
            778.75,
            865.00,
            885.00,
            900.00,
            1020.00,
            550.00,
            665.00,
            865.00,
            1375.00,
            1610.00,
            2250.00,
            550.00,
            665.00,
            865.00,
            1375.00,
            1610.00,
            2250.00
    };

    private static final double[] VGP_WAV = new double[914];

    private static final String TARGET_DIR_DEFAULT = ".";
    private static final String NCGEN_PATH_DEFAULT = "/usr/local/bin/ncgen";

    private File targetDir;
    private String ncgenPath;

    static {
        int i = 0;
        for (int k = 0; k < 91; k++, i++) {
            VGP_WAV[i] = 410.0 + k;
        }
        for (int k = 0; k < 221; k++, i++) {
            VGP_WAV[i] = 560.0 + k;
        }
        for (int k = 0; k < 301; k++, i++) {
            VGP_WAV[i] = 700.0 + k;
        }
        for (int k = 0; k < 301; k++, i++) {
            VGP_WAV[i] = 1500.0 + k;
        }
    }

    static void main(String[] args) throws Exception {
        final LookupTableGenerator generator = new LookupTableGenerator();

        try {
            generator.generateLookupTables();
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    LookupTableGenerator() throws IOException {
        final File propertiesFile = new File("adf.properties");
        if (propertiesFile.exists()) {
            System.getProperties().load(new FileReader(propertiesFile));
        }

        targetDir = new File(System.getProperty("targetDir", TARGET_DIR_DEFAULT));
        ncgenPath = System.getProperty("ncgenPath", NCGEN_PATH_DEFAULT);
    }

    void generateLookupTables() throws Exception {
        writeSynL2ConfigurationParametersDataset();
        writeSynL2RadiativeTransferSimulationDataset();
        writeVgtPConfigurationParametersDataset();
        writeVgtPRadiativeTransferSimulationDataset();
        writeVgtPSpectralResponseDataset();
        writeVgtSConfigurationParametersDataset();
        writeVgtSRadiativeTransferSimulationDataset();
    }

    void writeSynL2RadiativeTransferSimulationDataset() throws Exception {
        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_SYRTAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_SYRTAX_${VALIDITY_START}_${VALIDITY_STOP}_${CREATION_TIME}__BC__D_NT_AUX_${VERSION}");
        properties.setProperty("OLC_R_ATM", "ggluck.swan.ac.uk/ftp/Ralf/OLC_R_atm.txt");
        properties.setProperty("SLN_R_ATM", "ggluck.swan.ac.uk/ftp/Ralf/SLN_R_atm.txt");
        properties.setProperty("SLO_R_ATM", "ggluck.swan.ac.uk/ftp/Ralf/SLO_R_atm.txt");
        properties.setProperty("RHO_ATM", "ggluck.swan.ac.uk/ftp/Ralf/rho_atm.txt");
        properties.setProperty("T", "ggluck.swan.ac.uk/ftp/Ralf/t.txt");
        properties.setProperty("D", "ggluck.swan.ac.uk/ftp/Ralf/D.txt");
        properties.setProperty("C_O3", "ggluck.swan.ac.uk/ftp/Ralf/C_03.txt");

        generateDataset(properties);
    }

    void writeSynL2ConfigurationParametersDataset() throws Exception {
        final double[][] vegSpectrum = readSpectrum("dat/veg.dat", 1000.0);
        final double[][] soilSpectrum = readSpectrum("dat/soil.dat", 1000.0);

        final LookupTable vegLut = new LookupTable(vegSpectrum[1], vegSpectrum[0]);
        final LookupTable soilLut = new LookupTable(soilSpectrum[1], soilSpectrum[0]);

        final String vegPath = createTempFile("veg", true);
        final String soilPath = createTempFile("soil", true);

        writeSpectrum(vegLut, SYN_WAV, vegPath);
        writeSpectrum(soilLut, SYN_WAV, soilPath);

        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_SYCPAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_SYCPAX_${VALIDITY_START}_${VALIDITY_STOP}_${CREATION_TIME}__BC__D_NT_AUX_${VERSION}");
        properties.setProperty("R_VEG", vegPath);
        properties.setProperty("R_SOIL", soilPath);

        generateDataset(properties);
    }

    void writeVgtPRadiativeTransferSimulationDataset() throws Exception {
        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_VPRTAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VPRTAX_${VALIDITY_START}_${VALIDITY_STOP}_${CREATION_TIME}__BC__D_NT_AUX_${VERSION}");
        properties.setProperty("R_ATM", "ggluck.swan.ac.uk/ftp/Ralf/VP_R_atm.txt");
        properties.setProperty("RHO_ATM", "ggluck.swan.ac.uk/ftp/Ralf/VP_rho_atm.txt");
        properties.setProperty("T", "ggluck.swan.ac.uk/ftp/Ralf/VP_t.txt");
        properties.setProperty("C_O3", "ggluck.swan.ac.uk/ftp/Ralf/VP_C_03.txt");

        final String wavelengthPath = createTempFile("wavelength", true);
        writeWavelength(VGP_WAV, wavelengthPath);
        properties.setProperty("WAVELENGTH", wavelengthPath);

        generateDataset(properties);
    }

    void writeVgtPSpectralResponseDataset() throws Exception {
        final double[][] irradianceSpectrum = readSpectrum("dat/vgp_irradiance.dat");
        final LookupTable irradianceLut = new LookupTable(irradianceSpectrum[1], irradianceSpectrum[0]);

        final double[] b0 = createResponse(0, 91, "dat/RSR_VEGETATION_BAND_0.txt");
        final double[] b2 = createResponse(91, 221, "dat/RSR_VEGETATION_BAND_1.txt");
        final double[] b3 = createResponse(312, 301, "dat/RSR_VEGETATION_BAND_2.txt");
        final double[] b4 = createResponse(613, 301, "dat/RSR_VEGETATION_BAND_3.txt");

        final String wavelengthPath = createTempFile("wavelength", true);
        final String b0Path = createTempFile("b0_", true);
        final String b2Path = createTempFile("b2_", true);
        final String b3Path = createTempFile("b3_", true);
        final String b4Path = createTempFile("b4_", true);
        final String irradiancePath = createTempFile("irradiance", true);

        writeWavelength(VGP_WAV, wavelengthPath);
        writeWavelength(b0, b0Path);
        writeWavelength(b2, b2Path);
        writeWavelength(b3, b3Path);
        writeWavelength(b4, b4Path);
        writeSpectrum(irradianceLut, VGP_WAV, irradiancePath);

        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_VPSRAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VPSRAX_${VALIDITY_START}_${VALIDITY_STOP}_${CREATION_TIME}__BC__D_NT_AUX_${VERSION}");
        properties.setProperty("WAVELENGTH", wavelengthPath);
        properties.setProperty("B0_SRF", b0Path);
        properties.setProperty("B2_SRF", b2Path);
        properties.setProperty("B3_SRF", b3Path);
        properties.setProperty("MIR_SRF", b4Path);
        properties.setProperty("SOLAR_IRRADIANCE", irradiancePath);

        generateDataset(properties);
    }

    void writeVgtPConfigurationParametersDataset() throws Exception {
        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_VPCPAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VPCPAX_${VALIDITY_START}_${VALIDITY_STOP}_${CREATION_TIME}__BC__D_NT_AUX_${VERSION}");

        generateDataset(properties);
    }

    void writeVgtSConfigurationParametersDataset() throws Exception {
        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_VSCPAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VSCPAX_${VALIDITY_START}_${VALIDITY_STOP}_${CREATION_TIME}__BC__D_NT_AUX_${VERSION}");

        generateDataset(properties);
    }

    void writeVgtSRadiativeTransferSimulationDataset() throws Exception {
        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_VSRTAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VSRTAX_${VALIDITY_START}_${VALIDITY_STOP}_${CREATION_TIME}__BC__D_NT_AUX_${VERSION}");
        properties.setProperty("VGT_R_ATM", "ggluck.swan.ac.uk/ftp/Ralf/VS_R_atm.txt");
        properties.setProperty("RHO_ATM", "ggluck.swan.ac.uk/ftp/Ralf/VS_rho_atm.txt");
        properties.setProperty("T", "ggluck.swan.ac.uk/ftp/Ralf/VS_t.txt");
        properties.setProperty("C_O3", "ggluck.swan.ac.uk/ftp/Ralf/VS_C_03.txt");

        generateDataset(properties);
    }

    private void generateDataset(Properties properties) throws Exception {
        final TemplateResolver resolver = new TemplateResolver(properties);
        final File cdlFile = new File(targetDir, resolver.resolveProperty("CDL_File_Basename") + ".cdl");
        BufferedReader reader = null;
        BufferedWriter writer = null;
        try {
            final String templateName = resolver.resolveProperty("Template_File_Basename");
            final InputStream is = getClass().getResourceAsStream(templateName + ".cdl");
            reader = new BufferedReader(new InputStreamReader(is, "US-ASCII"));
            writer = new BufferedWriter(new FileWriter(cdlFile));
            String line = reader.readLine();
            while (line != null) {
                line = resolver.resolve(line, writer);
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

    private void generateNcFile(File cdlFile) throws Exception {
        final String command = ncgenPath + " -b -k 3 -o " + cdlFile.getPath().replace("cdl", "nc") + " " + cdlFile.getPath();
        final Process process = Runtime.getRuntime().exec(command);
        if (process.waitFor() != 0) {
            throw new Exception(
                    MessageFormat.format("process <code>{0}</code> terminated with exit value {1}",
                                         command, process.exitValue()));
        }
    }

    private static double[] createResponse(int first, int length, String name) {
        final double[] response = new double[VGP_WAV.length];
        final double[][] spectrum = readSpectrum(name);
        for (int i = first; i < first + length; i++) {
            final int k = Arrays.binarySearch(spectrum[0], VGP_WAV[i]);
            if (k >= 0) {
                response[i] = spectrum[1][k];
            }
        }
        return response;
    }

    private static void writeWavelength(double[] w, String path) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(path);
        try {
            for (int i = 0; i < w.length; i++) {
                if (i > 0) {
                    writer.print(", ");
                }
                writer.print(Math.round(w[i] * 1000000.0) / 1000000.0);
                writer.print("f");
            }
        } finally {
            writer.close();
        }
    }

    private static String createTempFile(String prefix, boolean deleteOnExit) throws IOException {
        final File file = File.createTempFile(prefix, ".cdl");
        if (deleteOnExit) {
            file.deleteOnExit();
        }
        return file.getPath();
    }

    private static void writeSpectrum(LookupTable lut, double[] wav, String fileName) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(fileName);
        try {
            int r = 0;
            for (int n = 0; n < wav.length; n++) {
                final double value = lut.getValue(wav[n]);
                if (r > 0) {
                    writer.print(", ");
                }
                writer.print(Math.round(value * 1000000.0) / 1000000.0);
                writer.print("f");
                r++;
            }
        } finally {
            writer.close();
        }
    }

    private static double[][] readSpectrum(String name) {
        return readSpectrum(name, 1.0);
    }

    private static double[][] readSpectrum(String name, double toNanometer) {
        final InputStream is = LookupTableGenerator.class.getResourceAsStream(name);
        final Scanner scanner = new Scanner(is, "US-ASCII");
        scanner.useLocale(Locale.US);
        final List<double[]> pointList = new ArrayList<double[]>();

        while (scanner.hasNextLine()) {
            final String line = scanner.nextLine().trim();
            if (line.startsWith("#") || line.isEmpty()) {
                continue;
            }
            final String[] tokens = line.split("\\s+");
            final double[] point = new double[2];
            point[0] = Double.parseDouble(tokens[0]);
            point[1] = Double.parseDouble(tokens[1]);
            pointList.add(point);
        }
        final double[][] spectrum = new double[2][pointList.size()];
        for (int i = 0, pointListSize = pointList.size(); i < pointListSize; i++) {
            final double[] point = pointList.get(i);
            spectrum[0][i] = point[0] * toNanometer;
            spectrum[1][i] = point[1];
        }
        return spectrum;
    }
}
