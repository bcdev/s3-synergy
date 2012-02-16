/*
 * Copyright (c) 2012. Brockmann Consult (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation. This program is distributed in the hope it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
 */

package org.esa.beam.util;

import org.esa.beam.util.XPathHelper;
import org.junit.BeforeClass;
import org.junit.Test;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPathFactory;
import java.io.IOException;
import java.io.StringReader;

import static org.junit.Assert.assertEquals;


public class XPathHelperTest {

    private static XPathHelper xPathHelper;
    private static Document xmlDoc;

    @BeforeClass
    public static void beforeClass() throws ParserConfigurationException, IOException, SAXException {
        xPathHelper = new XPathHelper(XPathFactory.newInstance().newXPath());
        String xmlString = "<xml>" +
                           "  <firstTag>" +
                           "    <innerTag>" +
                           "    <valueTag>A</valueTag>" +
                           "    <valueTag>B</valueTag>" +
                           "    <valueTag>A</valueTag>" +
                           "    </innerTag>" +
                           "  </firstTag>" +
                           "  <secondTag>" +
                           "    <valueTag>C</valueTag>" +
                           "  </secondTag>" +
                           "</xml>";
        xmlDoc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(
                new InputSource(new StringReader(xmlString)));

    }

    @Test
    public void testGetString() throws Exception {
        assertEquals("C", xPathHelper.getString("/xml/secondTag/valueTag", xmlDoc));
    }

    @Test
    public void testGetNodeList() throws Exception {
        NodeList nodeList = xPathHelper.getNodeList("/xml/firstTag/innerTag/valueTag[.='A']", xmlDoc);
        assertEquals(2, nodeList.getLength());
        Node valueTagOne = nodeList.item(0);
        assertEquals("valueTag", valueTagOne.getNodeName());
        assertEquals("A", valueTagOne.getTextContent());
        Node valueTagTwo = nodeList.item(1);
        assertEquals("valueTag", valueTagTwo.getNodeName());
        assertEquals("A", valueTagTwo.getTextContent());
    }

    @Test
    public void testGetNode() throws Exception {
        Node node = xPathHelper.getNode("/xml/secondTag/valueTag", xmlDoc);
        assertEquals("C", node.getTextContent());

    }
}
