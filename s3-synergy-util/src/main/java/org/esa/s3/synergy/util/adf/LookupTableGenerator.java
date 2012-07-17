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

    private static final double[] SZA = new double[]{
            0.0,
            17.5,
            35.0,
            42.5,
            70.0
    };
    private static final double[] VZA = new double[]{
            0.03,
            20.0,
            40.0,
            58.0
    };
    private static final double[] ADA = new double[]{
            0.0,
            36.0,
            72.0,
            108.0,
            144.0,
            180.0
    };
    private static final double[] AOT = new double[]{
            0.0,
            0.1,
            0.6,
            4.0
    };
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
    private static final double[] OLC_WAV = new double[]{
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
    };
    private static final double[] SLS_WAV = new double[]{
            550.00,
            665.00,
            865.00,
            1375.00,
            1610.00,
            2250.00
    };
    private static final double[] VGP_WAV = new double[914];
    private static final float[] SYN_ADA = new float[]{
            0.f,
            6.f,
            12.f,
            18.f,
            24.f,
            30.f,
            36.f,
            42.f,
            48.f,
            54.f,
            60.f,
            66.f,
            72.f,
            78.f,
            84.f,
            90.f,
            96.f,
            102.f,
            108.f,
            114.f,
            120.f,
            126.f,
            132.f,
            138.f,
            144.f,
            150.f,
            156.f,
            162.f,
            168.f,
            174.f,
            180.f
    };
    private static final float[] SYN_SZA = new float[]{
            0.f,
            3.5f,
            7.f,
            11.5f,
            14.f,
            17.5f,
            21.f,
            24.5f,
            28.f,
            31.5f,
            35.f,
            38.5f,
            42.f,
            45.5f,
            49.f,
            52.5f,
            56.f,
            59.5f,
            63.f,
            66.5f,
            70.f
    };
    private static final float[] OLC_VZA = new float[]{
            0.f,
            3.5f,
            7.f,
            10.5f,
            14.f,
            17.5f,
            21.f,
            24.5f,
            28.f,
            31.f,
            34.f,
            37.f,
            40.f,
            43.f,
            46.f,
            49.f,
            52.f,
            55.f
    };
    private static final float[] SLN_VZA = new float[]{
            6.f,
            9.5f,
            13.f,
            16.f,
            19.f,
            22.f,
            25.f,
            28.f,
            31.f,
            34.f,
            37.f,
            40.f,
            43.f,
            46.f,
            49.f,
            52.f,
            55.f,
            58.f
    };
    private static final float[] SLO_VZA = new float[]{
            55.0f
    };
    private static final float[] SYN_T550 = new float[]{
            0.f,
            0.05f,
            0.1f,
            0.2f,
            0.4f,
            0.6f,
            1.f,
            1.5f,
            2.f,
            3.f,
            4.f
    };
    private static final float[] AIR_PRESSURE = new float[]{
            800.f,
            900.f,
            1000.f,
            1030.f
    };
    private static final float[] VGP_ADA = {
            0.f,
            18.f,
            36.f,
            54.f,
            72.f,
            90.f,
            108.f,
            126.f,
            144.f,
            162.f,
            180.f
    };
    private static final float[] VGP_SZA = {
            0.f,
            10.f,
            20.f,
            30.f,
            40.f,
            50.f,
            60.f,
            70.f
    };
    private static final float[] VGP_VZA = {
            0.f,
            9.5f,
            19.f,
            28.f,
            37.f,
            46.f,
            55.f
    };
    private static final float[] VGP_T550 = {
            0.f,
            0.1f,
            0.4f,
            1.f,
            2.f,
            4.f
    };
    private static final int WATER_VAPOUR = 3;
    private static final int AMIN = 40;
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
        final File propertiesFile = new File("auxdata.properties");
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
        final double[][] irradianceSpectrum = readSpectrum("dat/syn_irradiance.dat");
        final double[] wavelengths = irradianceSpectrum[0];
        final double[] irradiances = irradianceSpectrum[1];

        final double[] intensities = new double[AOT.length * wavelengths.length * SZA.length * VZA.length * ADA.length];
        readIntensities(intensities, "dat/syn_intensity.dat");

        final double[] sphericalAlbedos = new double[AOT.length * wavelengths.length];
        readSphericalAlbedos(sphericalAlbedos, "dat/syn_spher_alb_up.dat");

        final double[] pathReflectances = computePathReflectance(irradiances, intensities);
        final double[] t = new double[AOT.length * SZA.length * wavelengths.length];

        final LookupTable lutRAtm = new LookupTable(pathReflectances, AOT, wavelengths, SZA, VZA, ADA);
        final LookupTable lutRhoAtm = new LookupTable(sphericalAlbedos, AOT, wavelengths);
        final LookupTable lutT = new LookupTable(t, AOT, wavelengths, SZA);

        final String olcRAtmPath = createTempFile("olc_ratm", true);
        final String slnRAtmPath = createTempFile("sln_ratm", true);
        final String sloRAtmPath = createTempFile("slo_ratm", true);
        final String rhoAtmPath = createTempFile("rho_atm", true);
        final String tPath = createTempFile("t__", true);
        final String dPath = createTempFile("d__", true);

        writeRatm(lutRAtm, OLC_WAV, SYN_ADA, OLC_VZA, SYN_SZA, SYN_T550, AMIN, olcRAtmPath);
        writeRatm(lutRAtm, SLS_WAV, SYN_ADA, SLN_VZA, SYN_SZA, SYN_T550, AMIN, slnRAtmPath);
        writeRatm(lutRAtm, SLS_WAV, SYN_ADA, SLO_VZA, SYN_SZA, SYN_T550, AMIN, sloRAtmPath);
        writeRhoAtm(lutRhoAtm, SYN_WAV, SYN_T550, AMIN, rhoAtmPath);
        writeT(lutT, SYN_WAV, SYN_SZA, SYN_T550, AMIN, tPath);
        writeD(SLS_WAV, dPath);

        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_SYRTAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_SYRTAX");
        properties.setProperty("OLC_R_ATM", olcRAtmPath);
        properties.setProperty("SLN_R_ATM", slnRAtmPath);
        properties.setProperty("SLO_R_ATM", sloRAtmPath);
        properties.setProperty("RHO_ATM", rhoAtmPath);
        properties.setProperty("T", tPath);
        properties.setProperty("D", dPath);

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
        properties.setProperty("CDL_File_Basename", "S3__SY_2_SYCPAX");
        properties.setProperty("R_VEG", vegPath);
        properties.setProperty("R_SOIL", soilPath);

        generateDataset(properties);
    }

    void writeVgtPRadiativeTransferSimulationDataset() throws Exception {
        final double[][] irradianceSpectrum = readSpectrum("dat/vgp_irradiance.dat");
        final double[] wavelengths = irradianceSpectrum[0];
        final double[] irradiances = irradianceSpectrum[1];

        final double[] intensities = new double[AOT.length * wavelengths.length * SZA.length * VZA.length * ADA.length];
        readIntensities(intensities, "dat/vgp_intensity.dat");

        final double[] sphericalAlbedos = new double[AOT.length * wavelengths.length];
        readSphericalAlbedos(sphericalAlbedos, "dat/vgp_spher_alb_up.dat");

        final double[] pathReflectances = computePathReflectance(irradiances, intensities);
        final double[] t = new double[AOT.length * SZA.length * wavelengths.length];

        final LookupTable lutRAtm = new LookupTable(pathReflectances, AOT, wavelengths, SZA, VZA, ADA);
        final LookupTable lutRhoAtm = new LookupTable(sphericalAlbedos, AOT, wavelengths);
        final LookupTable lutT = new LookupTable(t, AOT, wavelengths, SZA);

        final String rAtmPath = createTempFile("vgp_ratm", true);
        final String rhoAtmPath = createTempFile("rho_atm", true);
        final String tPath = createTempFile("t__", true);
        final String cO3Path = createTempFile("cO3", true);
        final String wavelengthPath = createTempFile("wavelength", true);

        writeRatm(lutRAtm, VGP_WAV, VGP_ADA, VGP_VZA, VGP_SZA, VGP_T550, 1, rAtmPath);
        writeRhoAtm(lutRhoAtm, VGP_WAV, SYN_T550, AMIN, rhoAtmPath);
        writeT(lutT, VGP_WAV, SYN_SZA, SYN_T550, AMIN, tPath);
        writeCO3(VGP_WAV, cO3Path);
        writeWavelength(VGP_WAV, wavelengthPath);

        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_VPRTAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VPRTAX");
        properties.setProperty("R_ATM", rAtmPath);
        properties.setProperty("RHO_ATM", rhoAtmPath);
        properties.setProperty("T", tPath);
        properties.setProperty("C_O3", cO3Path);
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
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VPSRAX");
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
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VPCPAX");

        generateDataset(properties);
    }

    void writeVgtSConfigurationParametersDataset() throws Exception {
        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_VSCPAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VSCPAX");

        generateDataset(properties);
    }

    void writeVgtSRadiativeTransferSimulationDataset() throws Exception {
        final File propertiesFile = new File("auxdata.properties");
        if (propertiesFile.exists()) {
            System.getProperties().load(new FileReader(propertiesFile));
        }

        targetDir = new File(System.getProperty("targetDir", TARGET_DIR_DEFAULT));
        ncgenPath = System.getProperty("ncgenPath", NCGEN_PATH_DEFAULT);

        final double[][] irradianceSpectrum = readSpectrum("dat/vgs_irradiance.dat");
        final double[] wavelengths = irradianceSpectrum[0];
        final double[] irradiances = irradianceSpectrum[1];

        final double[] intensities = new double[AOT.length * wavelengths.length * SZA.length * VZA.length * ADA.length];
        readIntensities(intensities, "dat/vgs_intensity.dat");

        final double[] sphericalAlbedos = new double[AOT.length * wavelengths.length];
        readSphericalAlbedos(sphericalAlbedos, "dat/vgs_spher_alb_up.dat");

        final double[] pathReflectances = computePathReflectance(irradiances, intensities);
        final double[] t = new double[AOT.length * SZA.length * wavelengths.length];

        final LookupTable lutRAtm = new LookupTable(pathReflectances, AOT, wavelengths, SZA, VZA, ADA);
        final LookupTable lutRhoAtm = new LookupTable(sphericalAlbedos, AOT, wavelengths);
        final LookupTable lutT = new LookupTable(t, AOT, wavelengths, SZA);

        final String vgtRAtmPath = createTempFile("vgt_ratm", true);
        final String rhoAtmPath = createTempFile("rho_atm", true);
        final String tPath = createTempFile("t__", true);

        writeRatm(lutRAtm, wavelengths, SYN_ADA, OLC_VZA, SYN_SZA, SYN_T550, AMIN, vgtRAtmPath);
        writeRhoAtm(lutRhoAtm, wavelengths, SYN_T550, AMIN, rhoAtmPath);
        writeT(lutT, wavelengths, SYN_SZA, SYN_T550, AMIN, tPath);

        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_VSRTAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_VSRTAX");
        properties.setProperty("VGT_R_ATM", vgtRAtmPath);
        properties.setProperty("RHO_ATM", rhoAtmPath);
        properties.setProperty("T", tPath);

        generateDataset(properties);
    }

    private void generateDataset(Properties properties) throws Exception {
        final TemplateResolver resolver = new TemplateResolver(properties);
        final File cdlFile = new File(resolver.resolveProperty("CDL_File_Basename") + ".cdl");
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
        final String command = ncgenPath + " -k 3 -b " + cdlFile.getPath();
        final Process process = Runtime.getRuntime().exec(command);
        if (process.waitFor() != 0) {
            throw new Exception(
                    MessageFormat.format("process <code>{0}</code> terminated with exit value {1}",
                                         command, process.exitValue()));
        }
    }

    /*
    private static double[] calculateResponse(double[] wavelengths, double center, double bandwidth) {
        final double[] response = new double[VGP_WAV.length];
        double sum = 0.0;
        for (int i = 0; i < response.length; i++) {
            final double x = wavelengths[i] - center;
            final double s = 0.5 * bandwidth;
            final double v = x / s;
            if (v <= 2.0) {
                response[i] = Math.exp(-0.5 * v * v);
                sum += response[i];
            }
        }
        for (int i = 0; i < response.length; i++) {
            response[i] /= sum;
        }
        return response;
    }
    */

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

    private static void writeCO3(double[] wav, String path) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(path);
        try {
            for (int i = 0; i < wav.length; i++) {
                if (i > 0) {
                    writer.print(", ");
                }
                writer.print("0.f");
            }
        } finally {
            writer.close();
        }
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

    private static double[] computePathReflectance(double[] irradiances, double[] intensities) {
        final double[] rAtm = new double[intensities.length];

        int n = 0;
        for (int i = 0; i < AOT.length; i++) {
            for (int j = 0; j < irradiances.length; j++) {
                for (int k = 0; k < SZA.length; k++) {
                    for (int l = 0; l < VZA.length; l++) {
                        for (int m = 0; m < ADA.length; m++) {
                            rAtm[n] = (intensities[n] * Math.PI) / (irradiances[j] * Math.cos(Math.toRadians(
                                    SZA[k])));
                            n++;
                        }
                    }
                }
            }
        }
        return rAtm;
    }

    private static void writeRatm(LookupTable lut, double[] wav, float[] ada, float[] vza, float[] sza,
                                  float[] t550, int amin, String fileName) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(fileName);
        try {
            int r = 0;
            for (int i = 0; i < ada.length; i++) {
                for (int j = 0; j < sza.length; j++) {
                    for (int k = 0; k < vza.length; k++) {
                        for (int l = 0; l < AIR_PRESSURE.length; l++) {
                            for (int m = 0; m < WATER_VAPOUR; m++) {
                                for (int o = 0; o < t550.length; o++) {
                                    for (int p = 0; p < amin; p++) {
                                        for (int q = 0; q < wav.length; q++) {
                                            final double value = lut.getValue(SYN_T550[o],
                                                                              wav[q],
                                                                              sza[j],
                                                                              vza[k],
                                                                              ada[i]);
                                            if (r > 0) {
                                                writer.print(", ");
                                                if (r % wav.length == 0) {
                                                    writer.println();
                                                }
                                            }
                                            writer.print(Math.round(value / 0.004));
                                            r++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } finally {
            writer.close();
        }
    }

    private static void writeRhoAtm(LookupTable lut, double[] wav, float[] t550, int amin, String fileName) throws
                                                                                                            FileNotFoundException {
        final PrintWriter writer = new PrintWriter(fileName);
        try {
            int r = 0;
            for (int i = 0; i < AIR_PRESSURE.length; i++) {
                for (int j = 0; j < WATER_VAPOUR; j++) {
                    for (int k = 0; k < t550.length; k++) {
                        for (int l = 0; l < amin; l++) {
                            for (int m = 0; m < wav.length; m++) {
                                final double value = lut.getValue(t550[k], wav[m]);
                                if (r > 0) {
                                    writer.print(", ");
                                    if (r % wav.length == 0) {
                                        writer.println();
                                    }
                                }
                                writer.print(Math.round(value * 1000000.0) / 1000000.0);
                                writer.print("f");
                                r++;
                            }
                        }
                    }
                }
            }
        } finally {
            writer.close();
        }
    }

    private static void writeT(LookupTable lut, double[] wav, float[] sza, float[] t550, int amin,
                               String fileName) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(fileName);
        try {
            int r = 0;
            for (int i = 0; i < sza.length; i++) {
                for (int j = 0; j < AIR_PRESSURE.length; j++) {
                    for (int k = 0; k < WATER_VAPOUR; k++) {
                        for (int l = 0; l < t550.length; l++) {
                            for (int m = 0; m < amin; m++) {
                                for (int n = 0; n < wav.length; n++) {
                                    final double value = 1.2 * transmission(wav[n], t550[l], sza[i],
                                                                            AIR_PRESSURE[j]);
                                    //final double value = lut.getValue(T550[o], wav[q], SYN_SZA[j]);
                                    if (r > 0) {
                                        writer.print(", ");
                                        if (r % wav.length == 0) {
                                            writer.println();
                                        }
                                    }
                                    writer.print(Math.round(value * 1000000.0) / 1000000.0);
                                    writer.print("f");
                                    r++;
                                }
                            }
                        }
                    }
                }
            }
        } finally {
            writer.close();
        }
    }

    private static void writeD(double[] wav, String fileName) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(fileName);
        try {
            int r = 0;
            for (int i = 0; i < SYN_SZA.length; i++) {
                for (int j = 0; j < AIR_PRESSURE.length; j++) {
                    for (int k = 0; k < SYN_T550.length; k++) {
                        for (int l = 0; l < AMIN; l++) {
                            for (int m = 0; m < wav.length; m++) {
                                final double value = diffuseFraction(wav[m],
                                                                     SYN_T550[k],
                                                                     SYN_SZA[i],
                                                                     AIR_PRESSURE[j]);
                                if (r > 0) {
                                    writer.print(", ");
                                    if (r % wav.length == 0) {
                                        writer.println();
                                    }
                                }
                                writer.print(Math.round(value * 1000000.0) / 1000000.0);
                                writer.print("f");
                                r++;
                            }
                        }
                    }
                }
            }
        } finally {
            writer.close();
        }
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

    private static void readIntensities(double[] intensities, String name) {
        final InputStream is = LookupTableGenerator.class.getResourceAsStream(name);
        final Scanner scanner = new Scanner(is, "US-ASCII");
        scanner.useLocale(Locale.US);

        int i = 0;
        while (scanner.hasNextLine()) {
            final String line = scanner.nextLine().trim();
            if (line.startsWith("#") || line.isEmpty()) {
                continue;
            }
            final String[] tokens = line.split("\\s+");
            for (int k = 1; k < tokens.length; k++) {
                intensities[i] = Double.parseDouble(tokens[k]);
                i++;
            }
        }
    }

    private static void readSphericalAlbedos(double[] spherAlbs, String name) {
        final InputStream is = LookupTableGenerator.class.getResourceAsStream(name);
        final Scanner scanner = new Scanner(is, "US-ASCII");
        scanner.useLocale(Locale.US);

        int i = 0;
        while (scanner.hasNextLine()) {
            final String line = scanner.nextLine().trim();
            if (line.startsWith("#") || line.isEmpty()) {
                continue;
            }
            final String[] tokens = line.split("\\s+");
            spherAlbs[i] = Double.parseDouble(tokens[1]);
            if (spherAlbs[i] > 1.0) {
                // we have read a line that does not contain spherical albedo
                continue;
            }
            i++;
        }
    }

    private static void readTransmissions(double[] transmissions, String name) {
        final InputStream is = LookupTableGenerator.class.getResourceAsStream(name);
        final Scanner scanner = new Scanner(is, "US-ASCII");
        scanner.useLocale(Locale.US);

        int i = 0;
        while (scanner.hasNextLine()) {
            final String line = scanner.nextLine().trim();
            if (line.startsWith("#") || line.isEmpty()) {
                continue;
            }
            final String[] tokens = line.split("\\s+");
            transmissions[i] = Double.parseDouble(tokens[tokens.length - 1]);
            i++;
        }
    }

    private static double diffuseFraction(double wav, double aot, double sza, double p) {
        // test if wavelength is in um or nm
        if (wav > 100.0) {
            wav /= 1000.0;
        }
        if (aot < 0) {
            return 0.0;
        } else {
            final double dmeff = 0.55;
            final double ryfrac = 0.5;
            final double aerfrac = 0.75;
            final double k = -1.25;
            final double a = aot / Math.pow(0.55, k);
            final double p0 = 1013.25;
            final double tauRay = tauRayleigh(wav, sza, p, p0);
            final double tauAer = dmeff * a * Math.pow(wav, k) / Math.cos(Math.toRadians(sza));
            final double tauTot = tauRay + tauAer;
            final double dir = Math.exp(-tauTot);
            final double diff = ryfrac * (1.0 - Math.exp(-tauRay)) + aerfrac * (1.0 - Math.exp(-tauAer));

            return diff / (dir + diff);
        }
    }

    private static double transmission(double wav, double aot, double sza, double p) {
        // test if wavelength is in um or nm
        if (wav > 100.0) {
            wav /= 1000.0;
        }
        if (aot < 0) {
            return 0.0;
        } else {
            final double dmeff = 0.55;
            final double ryfrac = 0.5;
            final double aerfrac = 0.75;
            final double k = -1.25;
            final double a = aot / Math.pow(0.55, k);
            final double p0 = 1013.25;
            final double tauRay = tauRayleigh(wav, sza, p, p0);
            final double tauAer = dmeff * a * Math.pow(wav, k) / Math.cos(Math.toRadians(sza));
            final double tauTot = tauRay + tauAer;
            final double dir = Math.exp(-tauTot);
            final double diff = ryfrac * (1.0 - Math.exp(-tauRay)) + aerfrac * (1.0 - Math.exp(-tauAer));

            return dir + diff;
        }
    }

    private static double tauRayleigh(double wav, double sza, double p, double p0) {
        return (p / p0) / Math.cos(Math.toRadians(sza) * (0.008569 * Math.pow(wav, -4.0) * (1.0 + 0.0113 * Math.pow(
                wav, -2.0) + 0.00013 * Math.pow(wav, -4.0))));
    }
}
