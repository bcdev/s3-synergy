package org.esa.beam.dataio.olci;

import org.junit.Test;

import static org.junit.Assert.assertEquals;

public class OlciLevel1ProductReaderTest {

    @Test
    public void testPatchFileName() {
        assertEquals("radiancesOa03.nc", OlciLevel1ProductReader.patchFileName("radianceOa3.nc"));
        assertEquals("radiancesOa13.nc", OlciLevel1ProductReader.patchFileName("radianceOa13.nc"));
        assertEquals("qualityFlags.nc", OlciLevel1ProductReader.patchFileName("qualityFlags.nc"));
    }

    @Test
    public void testSpectralBandsProperties() {
        final float[] wavelengths = new float[21];
        final float[] bandwidths = new float[21];
        OlciLevel1ProductReader.getSpectralBandsProperties(wavelengths, bandwidths);

        assertEquals(400.0, wavelengths[0], 0.0);
        assertEquals(412.5, wavelengths[1], 0.0);
        assertEquals(1020.0, wavelengths[20], 0.0);

        assertEquals(400.0, wavelengths[0], 0.0);
        assertEquals(412.5, wavelengths[1], 0.0);
        assertEquals(1020.0, wavelengths[20], 0.0);
    }
}
