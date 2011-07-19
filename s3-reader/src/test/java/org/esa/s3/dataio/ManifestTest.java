package org.esa.s3.dataio;

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

public class ManifestTest {

    private Manifest manifestTest;

    @Before
    public void before() throws ParserConfigurationException, IOException, SAXException {
        InputStream stream = getClass().getResourceAsStream("TEST_manifest.xml");
        try {
            Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(stream);
            manifestTest = new Manifest(doc);
        } finally {
            stream.close();
        }
    }

    @Test
    public void testGetProductName() {
        assertEquals("OL_1_ERR", manifestTest.getProductName());
    }

    @Test
    public void testGetProductType() {
        assertEquals("  OL_1_ERR", manifestTest.getProductType());
    }

    @Test
    public void testGetDescription() {
        assertEquals("Sentinel 3 Level 1B Product", manifestTest.getDescription());
    }

    @Test
    public void testGetLineCount() {
        assertEquals(1664, manifestTest.getLineCount());
    }

    @Test
    public void testGetColumnCount() {
        assertEquals(1217, manifestTest.getColumnCount());
    }

    @Test
    public void testGetMeasurementDataSetPointers() {
        List<DataSetPointer> dataSetPointers = manifestTest.getDataSetPointers(DataSetPointer.Type.M);
        assertEquals(22, dataSetPointers.size());
    }
}
