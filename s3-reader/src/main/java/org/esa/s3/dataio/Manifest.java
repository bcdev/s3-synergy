package org.esa.s3.dataio;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;
import java.util.ArrayList;
import java.util.List;

/**
 * Class encapsulating the manifest file.
 *
 * @author Marco Peters
 * @since 1.0
 */
class Manifest {

    public static final String FIXED_HEADER_BASE_PATH = "/Earth_Explorer_Header/Fixed_Header/";
    public static final String MPH_BASE_PATH = "/Earth_Explorer_Header/Variable_Header/Main_Product_Header/";
    public static final String SPH_BASE_PATH = "/Earth_Explorer_Header/Variable_Header/Specific_Product_Header/";

    private Document doc;
    private XPath xpath;

    /**
     * Creates an instance of this class by using the given W3C document.
     *
     * @param manifestDocument the W3C manifest document.
     */
    Manifest(Document manifestDocument) {
        this.doc = manifestDocument;
        xpath = XPathFactory.newInstance().newXPath();
    }

    public String getProductName() {
        return getString(MPH_BASE_PATH + "Product_Name", doc);
    }

    public String getDescription() {
        return getString(FIXED_HEADER_BASE_PATH + "File_Description", doc);
    }

    public String getProductType() {
        return getString(FIXED_HEADER_BASE_PATH + "File_Type", doc);
    }

    public int getLineCount() {
        return Integer.parseInt(getString(SPH_BASE_PATH + "Image_Size/Lines_Number", doc));
    }

    public int getColumnCount() {
        return Integer.parseInt(getString(SPH_BASE_PATH + "Columns_Number", doc));
    }

    private String getString(String pathExpr, Node node) {
        try {
            return xpath.evaluate(pathExpr, node);
        } catch (XPathExpressionException e) {
            throw new IllegalArgumentException("xpath '" + pathExpr + "' invalid.", e);
        }
    }

    public List<DataSetPointer> getDataSetPointers(DataSetPointer.Type type) {
        NodeList nodeList = getNodeList(doc, SPH_BASE_PATH + "List_of_Data_Objects/Data_Object_Descriptor[Type='M']");
        List<DataSetPointer> dataSetPointers = new ArrayList<DataSetPointer>();
        for (int i = 0; i < nodeList.getLength(); i++) {
            Node dataObjectDescriptorNode = nodeList.item(i);
            String fileName = getString("Filename", dataObjectDescriptorNode);
            String fileFormat = getString("File_Format", dataObjectDescriptorNode);
            dataSetPointers.add(new DataSetPointer(fileName, fileFormat, type));
        }

        return dataSetPointers;
    }

    private NodeList getNodeList(Node refNode, String pathExpr) {
        try {
            return (NodeList) xpath.evaluate(pathExpr, refNode, XPathConstants.NODESET);
        } catch (XPathExpressionException e) {
            throw new IllegalArgumentException("xpath '" + pathExpr + "' invalid.", e);
        }
    }

    private Node getNode(String pathExpr) {
        try {
            return (Node) xpath.evaluate(pathExpr, doc, XPathConstants.NODE);
        } catch (XPathExpressionException e) {
            throw new IllegalArgumentException("xpath '" + pathExpr + "' invalid.", e);
        }

    }
}
