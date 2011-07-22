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

package org.esa.s3.ol2pp.syn;

import org.esa.beam.util.math.LookupTable;
import org.junit.Test;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Properties;
import java.util.Scanner;

import static org.junit.Assert.assertNotNull;

public class LookupTableGeneratorTest {

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
    private static final double[] WAV = new double[]{
            400.00,
            412.50,
            442.50,
            490.00,
            510.00,
            550.00,
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
            1375.00,
            1610.00,
            2250.00
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

    private static final double[] AOT = new double[]{
            0.0,
            0.1,
            0.6,
            4.0
    };

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
            34.f,
            37.5f,
            41.f,
            44.f,
            49.f,
            53.f,
            57.f,
            61.f,
            65.f,
            70.f
    };

    private static final float[] OLC_VZA = new float[]{
            0.0f,
            3.0f,
            6.0f,
            9.0f,
            12.0f,
            15.0f,
            18.0f,
            21.0f,
            24.0f,
            27.0f,
            30.0f,
            33.0f,
            36.0f,
            39.0f,
            42.0f,
            45.0f,
            50.0f,
            55.0f
    };

    private static final float[] SLN_VZA = new float[]{
            6.0f,
            9.0f,
            12.0f,
            15.0f,
            18.0f,
            21.0f,
            24.0f,
            27.0f,
            30.0f,
            33.0f,
            36.0f,
            39.0f,
            42.0f,
            45.0f,
            48.0f,
            51.0f,
            54.0f,
            58.0f
    };
    private static final float[] SLO_VZA = new float[]{
            55.0f
    };

    private static final float[] T550 = new float[]{
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

    private static final int WATER_VAPOUR = 3;
    private static final int AMIN = 40;

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void testLookupTableGeneration() throws Exception {
        final double[] irradiances = new double[WAV.length];
        readIrradiances(irradiances);

        final double[] intensities = new double[AOT.length * WAV.length * SZA.length * VZA.length * ADA.length];
        readIntensities(intensities);

        final double[] spherAlbs = new double[AOT.length * WAV.length];
        readSpherAlbs(spherAlbs);

        final double[] transmissions = new double[AOT.length * WAV.length];
        readTransmissions(transmissions);

        final double[] rAtm = computePathReflectance(irradiances, intensities);
        final double[] t = new double[AOT.length * SZA.length * WAV.length];

        int n = 0;
        int m = 0;
        for (int i = 0; i < AOT.length; i++) {
            for (int k = 0; k < WAV.length; k++) {
                for (int j = 0; j < SZA.length; j++) {
                    t[n] = Math.pow(transmissions[m], 1.0 / Math.cos(SZA[j]));
                    n++;
                }
                m++;
            }
        }

        final LookupTable lutRatm = new LookupTable(rAtm, AOT, WAV, SZA, VZA, ADA);
        final LookupTable lutRhoAtm = new LookupTable(spherAlbs, AOT, WAV);
        final LookupTable lutT = new LookupTable(t, AOT, WAV, SZA);

        // 	ubyte OLC_R_atm(ADA, SZA, OLC_VZA, air_pressure, water_vapour, T550, AMIN, OLC_channel) ;

        writeRatm(lutRatm, OLC_VZA, OLC_WAV, "olc_ratm.dat");
        writeRatm(lutRatm, SLN_VZA, SLS_WAV, "sln_ratm.dat");
        writeRatm(lutRatm, SLO_VZA, SLS_WAV, "slo_ratm.dat");

        // float rho_atm(air_pressure, water_vapour, T550, AMIN, SYN_channel) ;

        writeRhoAtm(lutRhoAtm, SYN_WAV, "rho_atm.dat");

        // float t(SZA, air_pressure, water_vapour, T550, AMIN, SYN_channel) ;

        writeT(lutT, SYN_WAV, "t.dat");

        // float D(SZA, air_pressure, T550, AMIN, SLS_band) ;

        writeD(SLS_WAV, "d.dat");

        final Properties properties = new Properties();
        properties.setProperty("Template_File_Basename", "S3__SY_2_SYRTAX_template");
        properties.setProperty("CDL_File_Basename", "S3__SY_2_SYRTAX");
        properties.setProperty("OLC_R_ATM", "olc_ratm.dat");
        properties.setProperty("SLN_R_ATM", "sln_ratm.dat");
        properties.setProperty("SLO_R_ATM", "slo_ratm.dat");
        properties.setProperty("RHO_ATM", "rho_atm.dat");
        properties.setProperty("T", "t.dat");
        properties.setProperty("D", "d.dat");

        generateDataset(properties);
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    private double[] computePathReflectance(double[] irradiances, double[] intensities) {
        final double[] rAtm = new double[intensities.length];

        int n = 0;
        for (int i = 0; i < AOT.length; i++) {
            for (int j = 0; j < irradiances.length; j++) {
                for (int k = 0; k < SZA.length; k++) {
                    for (int l = 0; l < VZA.length; l++) {
                        for (int m = 0; m < ADA.length; m++) {
                            rAtm[n] = (intensities[n] * Math.PI) / (irradiances[j] * Math.cos(Math.toRadians(SZA[k])));
                            n++;
                        }
                    }
                }
            }
        }
        return rAtm;
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    private void writeRatm(LookupTable lut, float[] vza, double[] wav, String fileName) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(fileName);
        try {
            int r = 0;
            for (int i = 0; i < SYN_ADA.length; i++) {
                for (int j = 0; j < SYN_SZA.length; j++) {
                    for (int k = 0; k < vza.length; k++) {
                        for (int l = 0; l < AIR_PRESSURE.length; l++) {
                            for (int m = 0; m < WATER_VAPOUR; m++) {
                                for (int o = 0; o < T550.length; o++) {
                                    for (int p = 0; p < AMIN; p++) {
                                        for (int q = 0; q < wav.length; q++) {
                                            final double value = lut.getValue(T550[o],
                                                                              wav[q],
                                                                              SYN_SZA[j],
                                                                              vza[k],
                                                                              SYN_ADA[i]);
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

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    private void writeRhoAtm(LookupTable lut, double[] wav, String fileName) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(fileName);
        try {
            int r = 0;
            for (int i = 0; i < AIR_PRESSURE.length; i++) {
                for (int j = 0; j < WATER_VAPOUR; j++) {
                    for (int k = 0; k < T550.length; k++) {
                        for (int l = 0; l < AMIN; l++) {
                            for (int m = 0; m < wav.length; m++) {
                                final double value = lut.getValue(T550[k], wav[m]);
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

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    private void writeT(LookupTable lut, double[] wav, String fileName) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(fileName);
        try {
            int r = 0;
            for (int i = 0; i < SYN_SZA.length; i++) {
                for (int j = 0; j < AIR_PRESSURE.length; j++) {
                    for (int k = 0; k < WATER_VAPOUR; k++) {
                        for (int l = 0; l < T550.length; l++) {
                            for (int m = 0; m < AMIN; m++) {
                                for (int n = 0; n < wav.length; n++) {
                                    final double value = transmission(wav[n], T550[l], SYN_SZA[i], AIR_PRESSURE[j]);
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

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    private void writeD(double[] wav, String fileName) throws FileNotFoundException {
        final PrintWriter writer = new PrintWriter(fileName);
        try {
            int r = 0;
            for (int i = 0; i < SYN_SZA.length; i++) {
                for (int j = 0; j < AIR_PRESSURE.length; j++) {
                    for (int k = 0; k < T550.length; k++) {
                        for (int l = 0; l < AMIN; l++) {
                            for (int m = 0; m < wav.length; m++) {
                                final double value = diffuseFraction(wav[m],
                                                                     T550[k],
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

    private void generateDataset(Properties properties) throws Exception {
        final TemplateResolver resolver = new TemplateResolver(properties);
        final File cdlFile = new File(resolver.resolveProperty("CDL_File_Basename") + ".cdl");
        BufferedReader reader = null;
        BufferedWriter writer = null;
        try {
            final String templateName = resolver.resolveProperty("Template_File_Basename");
            final InputStream is = getClass().getResourceAsStream("aux/" + templateName + ".cdl");
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
    }

    private void readIrradiances(double[] irradiances) {
        final InputStream is = getClass().getResourceAsStream("aux/irradiance.dat");
        assertNotNull(is);
        final Scanner scanner = new Scanner(is, "US-ASCII");

        int i = 0;
        while (scanner.hasNextLine()) {
            final String line = scanner.nextLine().trim();
            if (line.startsWith("#") || line.isEmpty()) {
                continue;
            }
            final String[] tokens = line.split("\\s+");
            irradiances[i] = Double.parseDouble(tokens[1]);
            i++;
        }
    }

    private void readIntensities(double[] intensities) {
        final InputStream is = getClass().getResourceAsStream("aux/intensity.dat");
        assertNotNull(is);
        final Scanner scanner = new Scanner(is, "US-ASCII");

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

    private void readSpherAlbs(double[] spherAlbs) {
        final InputStream is = getClass().getResourceAsStream("aux/spher_alb_up.dat");
        assertNotNull(is);
        final Scanner scanner = new Scanner(is, "US-ASCII");

        int i = 0;
        while (scanner.hasNextLine()) {
            final String line = scanner.nextLine().trim();
            if (line.startsWith("#") || line.isEmpty()) {
                continue;
            }
            final String[] tokens = line.split("\\s+");
            spherAlbs[i] = Double.parseDouble(tokens[1]);
            i++;
        }
    }

    private void readTransmissions(double[] transmissions) {
        final InputStream is = getClass().getResourceAsStream("aux/spher_alb_dn.dat");
        assertNotNull(is);
        final Scanner scanner = new Scanner(is, "US-ASCII");

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

    private double diffuseFraction(double wav, double aot, double sza, double p) {
        // test if wavelength is in um or nm
        if (wav > 100.0) {
            wav /= 1000.0;
        }

        double diff_frac;
        if (aot < 0) {
            diff_frac = 0.0;
        } else {
            double dmeff = 0.55;
            double ryfrac = 0.5;
            double aerfrac = 0.75;
            double k = -1.25;
            double a = aot / Math.pow(0.55, k);
            double p0 = 1013.25;
            double tau_rayl = (p / p0) / Math.cos(sza / 180.0 * Math.PI) *
                              (0.008569 * Math.pow(wav, -4.0) * (1.0 + 0.0113 * Math.pow(wav,
                                                                                         -2.0) + 0.00013 * Math.pow(wav,
                                                                                                                    -4.0)));
            double tau_aero = dmeff * a * Math.pow(wav, k) / Math.cos(sza / 180 * Math.PI);
            double tau_tot = tau_rayl + tau_aero;

            double dir = Math.exp(-tau_tot);
            double diff = ryfrac * (1.0 - Math.exp(-tau_rayl)) + aerfrac * (1.0 - Math.exp(-tau_aero));

            diff_frac = diff / (dir + diff);
        }

        return diff_frac;
    }

    private double transmission(double wav, double aot, double sza, double p) {
        // test if wavelength is in um or nm
        if (wav > 100.0) {
            wav /= 1000.0;
        }

        double t;
        if (aot < 0) {
            t = 0.0;
        } else {
            double dmeff = 0.55;
            double ryfrac = 0.5;
            double aerfrac = 0.75;
            double k = -1.25;
            double a = aot / Math.pow(0.55, k);
            double p0 = 1013.25;
            double tau_rayl = (p / p0) / Math.cos(sza / 180.0 * Math.PI) *
                              (0.008569 * Math.pow(wav, -4.0) * (1.0 + 0.0113 * Math.pow(wav,
                                                                                         -2.0) + 0.00013 * Math.pow(wav,
                                                                                                                    -4.0)));
            double tau_aero = dmeff * a * Math.pow(wav, k) / Math.cos(sza / 180 * Math.PI);
            double tau_tot = tau_rayl + tau_aero;

            double dir = Math.exp(-tau_tot);
            double diff = ryfrac * (1.0 - Math.exp(-tau_rayl)) + aerfrac * (1.0 - Math.exp(-tau_aero));

            t = dir + diff;
        }
        return t;
    }

}
