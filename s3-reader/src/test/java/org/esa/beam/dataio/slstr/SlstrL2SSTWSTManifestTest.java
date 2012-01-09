package org.esa.beam.dataio.slstr;

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

public class SlstrL2SSTWSTManifestTest {
    private SlstrL2SSTManifest manifestTest;

    @Before
    public void before() throws ParserConfigurationException, IOException, SAXException {
        InputStream stream = getClass().getResourceAsStream("SLSTR_L2_SST_WST_SAFE_TEST_manifest.xml");
        try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
            manifestTest = new SlstrL2SSTManifest(doc);
        } finally {
            stream.close();
        }
    }

    @Test
    public void testGetDescription() throws Exception {
        assertNotNull(manifestTest);
        final String description = manifestTest.getDescription();
        assertEquals("Sentinel 3 SLST Level 2 WST", description);
    }

    @Test
    public void testGetStartTime() throws Exception {
        final ProductData.UTC expected = ProductData.UTC.parse("2007-09-10T09:51:30.389847", "yyyy-MM-dd'T'HH:mm:ss");
        assertTrue(expected.equalElems(manifestTest.getStartTime()));
    }

    @Test
    public void testGetStopTime() throws Exception {
        final ProductData.UTC expected = ProductData.UTC.parse("2007-09-10T09:54:41.864790", "yyyy-MM-dd'T'HH:mm:ss");
        assertTrue(expected.equalElems(manifestTest.getStopTime()));
    }

    @Test
    public void testGetMeasurementFileNames() {
        final List<String> measurementFiles = manifestTest.getWSTMeasurementFileNames();
        assertEquals(1, measurementFiles.size());
        assertEquals("L2P_SST_in.nc", measurementFiles.get(0));
    }

}
