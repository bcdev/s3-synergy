package org.esa.beam.dataio.slstr;

import org.junit.Before;
import org.junit.Test;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.io.InputStream;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

public class SlstrL2SSTManifestTest {
    private SlstrL2LndManifest manifestTest;

    @Before
    public void before() throws ParserConfigurationException, IOException, SAXException {
        InputStream stream = getClass().getResourceAsStream("SLSTR_L2_SST_WCT_SAFE_TEST_manifest.xml");
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
        assertEquals("Sentinel 3 SLST Level 2 WCT", description);
    }

    // todo continue

}
