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

public class SlstrL2SSTWCTManifestTest {
    private SlstrL2SSTManifest manifestTest;

    @Before
    public void before() throws ParserConfigurationException, IOException, SAXException {
        InputStream stream = getClass().getResourceAsStream("SLSTR_L2_SST_WCT_SAFE_TEST_manifest.xml");
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
        assertEquals("Sentinel 3 SLST Level 2 WCT", description);
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
    public void testGetSingleViewMeasurementFileNames() {
        final List<String> measurementFiles = manifestTest.getWCTNadirViewMeasurementFileNames();
        assertEquals(3, measurementFiles.size());
        assertEquals("N2_SST_in.nc", measurementFiles.get(0));
        assertEquals("N3R_SST_in.nc", measurementFiles.get(1));
        assertEquals("N3_SST_in.nc", measurementFiles.get(2));
    }

    @Test
    public void testGetDualViewMeasurementFileNames() {
        final List<String> measurementFiles = manifestTest.getWCTDualViewMeasurementFileNames();
        assertEquals(2, measurementFiles.size());
        assertEquals("D2_SST_io.nc", measurementFiles.get(0));
        assertEquals("D3_SST_io.nc", measurementFiles.get(1));
    }

    @Test
    public void testGetVirtualDataFileNames() {
        final List<String> virtualDataFileNames = manifestTest.getVirtualDataFileNames();
        assertEquals(5, virtualDataFileNames.size());
        assertEquals("N2_SST_in.ncml", virtualDataFileNames.get(0));
        assertEquals("N3R_SST_in.ncml", virtualDataFileNames.get(1));
        assertEquals("N3_SST_in.ncml", virtualDataFileNames.get(2));
        assertEquals("D2_SST_io.ncml", virtualDataFileNames.get(3));
        assertEquals("D3_SST_io.ncml", virtualDataFileNames.get(4));
    }

    @Test
    public void testGetNadirFlagsFileName() {
        final String nadirFlagsFileName = manifestTest.getNadirFlagsFileName();
        assertEquals("flags_in.nc", nadirFlagsFileName);
    }

    @Test
    public void testGetObliqueFlagsFileName() {
        final String obliqueFlagsFileName = manifestTest.getObliqueFlagsFileName();
        assertEquals("flags_io.nc", obliqueFlagsFileName);
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
        final String nadirSolarViewGeometryFileName = manifestTest.getNadirSolarViewGeometryTiepointsFileName();
        assertEquals("geometry_in.nc", nadirSolarViewGeometryFileName);
    }

    @Test
    public void testGetObliqueSolarViewGeometryFileName() {
        final String obliqueSolarViewGeometryFileName = manifestTest.getObliqueSolarViewGeometryTiepointsFileName();
        assertEquals("geometry_io.nc", obliqueSolarViewGeometryFileName);
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

}
