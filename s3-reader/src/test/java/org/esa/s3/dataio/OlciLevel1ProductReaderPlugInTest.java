package org.esa.s3.dataio;

import org.esa.beam.framework.dataio.DecodeQualification;
import org.esa.beam.framework.dataio.ProductReader;
import org.junit.Before;
import org.junit.Test;

import java.io.File;

import static org.junit.Assert.*;

public class OlciLevel1ProductReaderPlugInTest {

    private OlciLevel1ProductReaderPlugIn plugIn;

    @Before
    public void setup() {
        plugIn = new OlciLevel1ProductReaderPlugIn();
    }

    @Test
    public void testDecodeQualificationWithFullResolution() {
        String validPath = createPath("OL", OlciL1bModes.EFR.name());
        assertEquals(DecodeQualification.INTENDED, plugIn.getDecodeQualification(validPath));
    }

    @Test
    public void testDecodeQualificationWithReducedResolution() {
        String validPath = createPath("OL", OlciL1bModes.ERR.name());
        assertEquals(DecodeQualification.INTENDED, plugIn.getDecodeQualification(validPath));
    }

    @Test
    public void testDecodeQualificationWithInvalidDataSource() {
        String invalidDataSource = "SL";
        String invalidPath = createPath(invalidDataSource, OlciL1bModes.ERR.name());
        assertEquals(DecodeQualification.UNABLE, plugIn.getDecodeQualification(invalidPath));
    }

    @Test
    public void testDecodeQualificationWithInvalidMode() {
        String invalidMode = "ABC";
        String invalidPath = createPath("OL", invalidMode);
        assertEquals(DecodeQualification.UNABLE, plugIn.getDecodeQualification(invalidPath));
    }

    @Test
    public void testDecodeQualificationWithoutSAFEExtensions() {
        String invalidPath = "/OL_1_ERR_TTTTTTTTTTTT_instanceID_GGG_CCCC_VV/someFile.nc";
        assertEquals(DecodeQualification.UNABLE, plugIn.getDecodeQualification(invalidPath));
    }

    @Test
    public void testSupportedInputTypes() {
        Class[] inputTypes = plugIn.getInputTypes();
        assertEquals(2, inputTypes.length);
        assertEquals(String.class, inputTypes[0]);
        assertEquals(File.class, inputTypes[1]);
    }

    @Test
    public void testCreateReaderInstanceReturnsNewInstanceEachTime() {
        ProductReader firstInstance = plugIn.createReaderInstance();
        assertNotNull(firstInstance);
        ProductReader secondInstance = plugIn.createReaderInstance();
        assertNotSame(secondInstance, firstInstance);
    }
    private String createPath(String dataSource, String mode) {
        String validParentDirectory = String.format("/%s_1_%s_TTTTTTTTTTTT_instanceID_GGG_CCCC_VV.SAFE/", dataSource, mode);
        String aFileInDirectory = "someFile.nc";
        return validParentDirectory + aFileInDirectory;
    }
}
