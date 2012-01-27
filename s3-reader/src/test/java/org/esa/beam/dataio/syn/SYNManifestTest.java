package org.esa.beam.dataio.syn;

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

public class SynManifestTest {

    private SynManifest manifestTest;

    @Before
    public void before() throws ParserConfigurationException, IOException, SAXException {
        InputStream stream = getClass().getResourceAsStream("SYN_TEST_manifest.xml");
        try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
            manifestTest = new SynManifest(doc);
        } finally {
            stream.close();

        }
    }

    @Test
    public void testGetDescription() throws Exception {
        assertEquals("Sentinel 3 SYN Level 2", manifestTest.getDescription());
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
        assertEquals(34, measurementFiles.size());
        assertEquals("r0400.nc", measurementFiles.get(0));
        assertEquals("r0560.nc", measurementFiles.get(5));
        assertEquals("r0550n.nc", measurementFiles.get(18));
        assertEquals("r1375o.nc", measurementFiles.get(27));
        assertEquals("flags.nc", measurementFiles.get(33));
    }

    @Test
    public void testGetGeoCoordinatesFileName() {
        assertEquals("geolocation.nc", manifestTest.getGeoCoordinatesFileName());
    }

    @Test
    public void testGetTiepointsFileNames() {
        List<String> tiepointsFiles = manifestTest.getTiepointsFileNames();
        assertEquals(4, tiepointsFiles.size());
        assertEquals("tiepoints_meteo.nc", tiepointsFiles.get(0));
        assertEquals("tiepoints_olci.nc", tiepointsFiles.get(1));
        assertEquals("tiepoints_slstr_n.nc", tiepointsFiles.get(2));
        assertEquals("tiepoints_slstr_o.nc", tiepointsFiles.get(3));
    }

    @Test
    public void testGetTimeFileName() {
        assertEquals("time.nc", manifestTest.getTimeFileName());
    }
}
