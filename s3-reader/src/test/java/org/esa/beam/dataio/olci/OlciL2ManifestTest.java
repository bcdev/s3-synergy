package org.esa.beam.dataio.olci;

import org.esa.beam.dataio.olci.OlciL2Manifest;
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

import static org.junit.Assert.*;

public class OlciL2ManifestTest {

    private OlciL2Manifest manifestTest;

    @Before
    public void before() throws ParserConfigurationException, IOException, SAXException {
        InputStream stream = getClass().getResourceAsStream("L2_TEST_manifest.xml");
        try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
            manifestTest = new OlciL2Manifest(doc);
        } finally {
            stream.close();
        }
    }

    @Test
    public void testGetDescription() throws Exception {
        assertEquals("Sentinel 3 OLC Level 2 WRR", manifestTest.getDescription());
    }

    @Test
    public void testGetStartTime() throws Exception {
        ProductData.UTC expected = ProductData.UTC.parse("2007-09-10T09:51:30.389847", "yyyy-MM-dd'T'HH:mm:ss");
        assertTrue(expected.equalElems(manifestTest.getStartTime()));
    }

    @Test
    public void testGetStopTime() throws Exception {
        ProductData.UTC expected = ProductData.UTC.parse("2007-09-10T09:54:41.864790", "yyyy-MM-dd'T'HH:mm:ss");
        assertTrue(expected.equalElems(manifestTest.getStopTime()));
    }

    @Test
    public void testGetMeasurementFileNames() {
        List<String> measurementFiles = manifestTest.getMeasurementFileNames();
        assertEquals(28, measurementFiles.size());
        assertEquals("r400.nc", measurementFiles.get(0));
        assertEquals("r560.nc", measurementFiles.get(5));
        assertEquals("chl_nn.nc", measurementFiles.get(18));
        assertEquals("wqsf.nc", measurementFiles.get(27));
    }

    @Test
    public void testGetGeoCoordinatesFileName() {
        assertEquals("geoCoordinates.nc", manifestTest.getGeoCoordinatesFileName());
    }
    @Test
    public void testGetTiePointsFileName() {
        assertEquals("tiePoints.nc", manifestTest.getTiePointsFileName());
    }
}
