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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

public class SlstrL2LndManifestTest {
    private SlstrL2LndManifest manifestTest;

    @Before
    public void before() throws ParserConfigurationException, IOException, SAXException {
        InputStream stream = getClass().getResourceAsStream("SLSTR_L2_LND_SAFE_TEST_manifest.xml");
        try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
            manifestTest = new SlstrL2LndManifest(doc);
        } finally {
            stream.close();
        }
    }

    @Test
    public void testGetDescription() throws Exception {
        assertNotNull(manifestTest);
        final String description = manifestTest.getDescription();
        assertEquals("Sentinel 3 SLST Level 2 LST", description);
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
        final List<String> measurementFiles = manifestTest.getMeasurementFileNames();
        assertEquals(2, measurementFiles.size());
        assertEquals("LST_in.nc", measurementFiles.get(0));
        assertEquals("FRP_in.nc", measurementFiles.get(1));
    }

    @Test
    public void testGetVirtualDataFileNames() {
        final List<String> virtualDataFileNames = manifestTest.getVirtualDataFileNames();
        assertEquals(2, virtualDataFileNames.size());
        assertEquals("LST_in.ncml", virtualDataFileNames.get(0));
        assertEquals("FRP_in.ncml", virtualDataFileNames.get(1));
    }

    @Test
    public void testGetNadirFlagsFileName() {
        final String nadirFlagsFileName = manifestTest.getNadirFlagsFileName();
        assertEquals("flags_in.nc", nadirFlagsFileName);
    }

    @Test
    public void testGetIndicesFileName() {
        final String indicesFileName = manifestTest.getIndicesFileName();
        assertEquals("indices_in.nc", indicesFileName);
    }

    @Test
    public void testGetGeodeticDataCoordinatesFileName() {
        final String geodeticDataCoordinatesFileName = manifestTest.getGeodeticDataCoordinatesFileName();
        assertEquals("geodetic_in.nc", geodeticDataCoordinatesFileName);
    }

    @Test
    public void testGetCartesianDataCoordinatesFileName() {
        final String cartesianDataCoordinatesFileName = manifestTest.getCartesianDataCoordinatesFileName();
        assertEquals("cartesian_in.nc", cartesianDataCoordinatesFileName);
    }

    @Test
    public void testGetGeodeticTiepointCoordinatesSchemaFileName() {
        final String geodeticTiepointCoordinatesFileName = manifestTest.getGeodeticTiepointCoordinatesFileName();
        assertEquals("geodetic_tx.nc", geodeticTiepointCoordinatesFileName);
    }

    @Test
    public void testGetCartesianTiepointCoordinatesSchemaFileName() {
        final String cartesianTiepointCoordinatesFileName = manifestTest.getCartesianTiepointCoordinatesFileName();
        assertEquals("cartesian_tx.nc", cartesianTiepointCoordinatesFileName);
    }

    @Test
    public void testGetNadirSolarViewGeometryFileName() {
        final String nadirSolarViewGeometryFileName = manifestTest.getGeometryTiepointsFileName();
        assertEquals("geometry_in.nc", nadirSolarViewGeometryFileName);
    }

    @Test
    public void testGetMeteorologicalDataFileName() {
        final String meteorologicalDataFileName = manifestTest.getMeteorologicalTiePointsFileName();
        assertEquals("met_tx.nc", meteorologicalDataFileName);
    }

    @Test
    public void testGetTimeCoordinatesFileName() {
        final String timeCoordinatesFileName = manifestTest.getTimeCoordinatesFileName();
        assertEquals("time_in.nc", timeCoordinatesFileName);
    }


    // todo continue

}
