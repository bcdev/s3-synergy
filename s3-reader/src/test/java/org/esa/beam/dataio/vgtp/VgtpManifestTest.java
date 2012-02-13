package org.esa.beam.dataio.vgtp;

import org.esa.beam.framework.datamodel.ProductData;
import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class VgtpManifestTest {

    private VgtpManifest manifestTest;

    @Before
    public void before() throws ParserConfigurationException, IOException, SAXException {
        InputStream stream = getClass().getResourceAsStream("VGT_P_TEST_manifest.xml");
        try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
            manifestTest = new VgtpManifest(doc);
        } finally {
            stream.close();

        }
    }

    @Test
    public void testGetDescription() throws Exception {
        assertEquals("Sentinel 3 SYN Level 2 VGT P", manifestTest.getDescription());
    }

    @Test
    public void testGetStartTime() throws Exception {
        ProductData.UTC expected = ProductData.UTC.parse("2013-06-21T10:09:20", "yyyy-MM-dd'T'HH:mm:ss");
        final ProductData.UTC startTime = manifestTest.getStartTime();
        assertTrue(expected.equalElems(startTime));
    }

    @Test
    public void testGetStopTime() throws Exception {
        ProductData.UTC expected = ProductData.UTC.parse("2013-06-21T10:14:13", "yyyy-MM-dd'T'HH:mm:ss");
        final ProductData.UTC stopTime = manifestTest.getStopTime();
        assertTrue(expected.equalElems(stopTime));
    }

    @Test
    public void testGetMeasurementFileNames() {
        List<String> measurementFiles = manifestTest.getMeasurementFileNames();
        // todo: manifest to be adjusted
//        assertEquals(4, measurementFiles.size());
        assertEquals(8, measurementFiles.size());
        assertEquals("b0.nc", measurementFiles.get(0));
        assertEquals("b2.nc", measurementFiles.get(1));
        assertEquals("b3.nc", measurementFiles.get(2));
        assertEquals("mir.nc", measurementFiles.get(3));
    }

    @Test
    public void testGetTiepointsFileNames() {
        List<String> tiepointsFiles = manifestTest.getTiepointsFileNames();
        // todo: manifest to be adjusted
//        assertEquals(4, tiepointsFiles.size());
//        assertEquals("og.nc", tiepointsFiles.get(0));
//        assertEquals("wvg.nc", tiepointsFiles.get(1));
//        assertEquals("ag.nc", tiepointsFiles.get(2));
//        assertEquals("sm.nc", tiepointsFiles.get(3));
    }

    @Test
    public void testGetGeometryFileNames() {
        List<String> geometryFiles = manifestTest.getGeometryFileNames();
        // todo: manifest to be adjusted
//        assertEquals(4, tiepointsFiles.size());
//        assertEquals("sza.nc", geometryFiles.get(0));
//        assertEquals("saa.nc", geometryFiles.get(1));
//        assertEquals("vza.nc", geometryFiles.get(2));
//        assertEquals("vaa.nc", geometryFiles.get(3));
    }

}
