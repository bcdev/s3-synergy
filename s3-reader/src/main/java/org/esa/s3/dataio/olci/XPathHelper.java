package org.esa.s3.dataio.olci;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xmlpull.v1.builder.XmlDoctype;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import java.io.IOException;
import java.io.StringReader;

/**
 * Provides helper methods which simplify the usage of {@link javax.xml.xpath.XPath}
 *
 * @author Marco Peters
 * @since 1.0
 */
class XPathHelper {

    private XPath xPath;

    XPathHelper(XPath xPath) {
        this.xPath = xPath;
    }

    public String getString(String pathExpr, Node node) {
        try {
            return xPath.evaluate(pathExpr, node);
        } catch (XPathExpressionException e) {
            throw new IllegalArgumentException("XPath '" + pathExpr + "' invalid.", e);
        }
    }

    public NodeList getNodeList(String pathExpr, Node node) {
        try {
            return (NodeList) xPath.evaluate(pathExpr, node, XPathConstants.NODESET);
        } catch (XPathExpressionException e) {
            throw new IllegalArgumentException("XPath '" + pathExpr + "' invalid.", e);
        }
    }

    public Node getNode(String pathExpr, Node node) {
        try {
            return (Node) xPath.evaluate(pathExpr, node, XPathConstants.NODE);
        } catch (XPathExpressionException e) {
            throw new IllegalArgumentException("XPpath '" + pathExpr + "' invalid.", e);
        }

    }
}
