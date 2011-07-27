package org.esa.beam.dataio.olci;

import org.esa.beam.framework.datamodel.ProductData;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathFactory;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;

/**
 * Class encapsulating the manifest file of an Olci Level 2 product.
 *
 * @author Marco Peters
 * @since 1.0
 */
class OlciL2Manifest {

    private Document doc;
    private XPathHelper xPathHelper;

    OlciL2Manifest(Document manifestDocument) {
        doc = manifestDocument;
        XPath xPath = XPathFactory.newInstance().newXPath();
        xPathHelper = new XPathHelper(xPath);
    }

    public String getDescription() {
        return xPathHelper.getString("/XFDU/informationPackageMap/contentUnit/@textInfo", doc);
    }

    public ProductData.UTC getStartTime() {
        Node periodNode = xPathHelper.getNode("/XFDU/metadataSection/metadataObject[@ID='acquisitionPeriod']", doc);
        String utcString = xPathHelper.getString("//metadataWrap/xmlData/acquisitionPeriod/startTime", periodNode);
        utcString = removeTimeZoneCharacter(utcString);
        try {
            return ProductData.UTC.parse(utcString, "yyyy-MM-dd'T'HH:mm:ss");
        } catch (ParseException ignored) {
            return null;
        }
    }

    private String removeTimeZoneCharacter(String utcString) {
        return utcString.substring(0, utcString.length() - 1);
    }

    public ProductData.UTC getStopTime() {
        Node periodNode = xPathHelper.getNode("/XFDU/metadataSection/metadataObject[@ID='acquisitionPeriod']", doc);
        String utcString = xPathHelper.getString("//metadataWrap/xmlData/acquisitionPeriod/stopTime", periodNode);
        utcString = removeTimeZoneCharacter(utcString);
        try {
            return ProductData.UTC.parse(utcString, "yyyy-MM-dd'T'HH:mm:ss");
        } catch (ParseException ignored) {
            return null;
        }
    }

    public List<String> getMeasurementFileNames() {
        NodeList dataObjects = xPathHelper.getNodeList(
                "/XFDU/dataObjectSection/dataObject[@repID='measurementDataSchema']", doc);
        List<String> fileNames = new ArrayList<String>();
        for (int i = 0; i < dataObjects.getLength(); i++) {
            Node item = dataObjects.item(i);
            String fileName = xPathHelper.getString("./byteStream/fileLocation/@href", item);
            fileNames.add(fileName);
        }

        return fileNames;
    }

    public String getGeoCoordinatesFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='geoCoordinatesSchema']",
                                                 doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getTiePointsFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='tiepointsDataSchema']",
                                                 doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }
}
