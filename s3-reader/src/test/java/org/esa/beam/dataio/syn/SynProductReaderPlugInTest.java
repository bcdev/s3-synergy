package org.esa.beam.dataio.syn;

import org.esa.beam.dataio.olci.OlciLevel2ProductReaderPlugIn;
import org.esa.beam.framework.dataio.DecodeQualification;
import org.esa.beam.framework.dataio.ProductIOPlugInManager;
import org.esa.beam.framework.dataio.ProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.junit.Before;
import org.junit.Test;

import java.io.File;
import java.util.Iterator;

import static org.junit.Assert.*;

public class SynProductReaderPlugInTest {

    public static final String SYN_SENSOR_ID = "SY";

    private SynProductReaderPlugIn plugIn;

    @Before
    public void setup() {
        plugIn = new SynProductReaderPlugIn();
    }

    @Test
    public void testIfPlugInIsLoaded() {
        ProductIOPlugInManager ioPlugInManager = ProductIOPlugInManager.getInstance();
        Iterator<ProductReaderPlugIn> readerPlugIns = ioPlugInManager.getReaderPlugIns(
                SynProductReaderPlugIn.FORMAT_NAME_SYN);
        assertTrue(readerPlugIns.hasNext());
        assertTrue(readerPlugIns.next() instanceof SynProductReaderPlugIn);

    }

    @Test
    public void testDecodeQualificationWithFullResolution() {
        String validPath = createManifestFilePath(SYN_SENSOR_ID);
        assertEquals(DecodeQualification.INTENDED, plugIn.getDecodeQualification(validPath));
    }

    @Test
    public void testDecodeQualificationWithReducedResolution() {
        String validPath = createManifestFilePath(SYN_SENSOR_ID);
        assertEquals(DecodeQualification.INTENDED, plugIn.getDecodeQualification(validPath));
    }

    @Test
    public void testDecodeQualificationWithInvalidDataSource() {
        String invalidSensorId = "SL";
        String invalidPath = createManifestFilePath(invalidSensorId);
        assertEquals(DecodeQualification.UNABLE, plugIn.getDecodeQualification(invalidPath));
    }

    @Test
    public void testDecodeQualificationWithoutFile() {
        String invalidPath = "/SY_1_ERR_TTTTTTTTTTTT_instanceID_GGG_CCCC_VV";
        assertEquals(DecodeQualification.UNABLE, plugIn.getDecodeQualification(invalidPath));
    }

    @Test
    public void testDecodeQualificationWithWrongFile() {
        String invalidPath = "/S3_SY_2_ERR_TTTTTTTTTTTT_instanceID_GGG_CCCC_VV/someFile.doc";
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
    private String createManifestFilePath(String sensorId) {
        String validParentDirectory = String.format("/S3_%s_2_MMM_TTTTTTTTTTTT_instanceID_GGG_CCCC_VV.SAFE/", sensorId);
        String manifestFile = "manifest_SYN.xml";
        return validParentDirectory + manifestFile;
    }
}
