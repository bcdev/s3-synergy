package org.esa.beam.dataio.vgtp;

import org.esa.beam.framework.datamodel.ProductData;
import org.esa.beam.util.XPathHelper;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathFactory;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;

/**
 * Class encapsulating the manifest file of an OLCI/SLSTR L2 SYN product.
 *
 * @author Olaf Danne
 * @since 1.0
 */
class VgtpManifest {

    private Document doc;
    private XPathHelper xPathHelper;

    VgtpManifest(Document manifestDocument) {
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
        if (!Character.isDigit(utcString.charAt(utcString.length() - 1))) {
            utcString = removeTimeZoneCharacter(utcString);
        }
        try {
            return ProductData.UTC.parse(utcString, "yyyy-MM-dd'T'HH:mm:ss");
        } catch (ParseException ignored) {
            return null;
        }
    }

    public ProductData.UTC getStopTime() {
        Node periodNode = xPathHelper.getNode("/XFDU/metadataSection/metadataObject[@ID='acquisitionPeriod']", doc);
        String utcString = xPathHelper.getString("//metadataWrap/xmlData/acquisitionPeriod/stopTime", periodNode);
        if (!Character.isDigit(utcString.charAt(utcString.length() - 1))) {
            utcString = removeTimeZoneCharacter(utcString);
        }
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

    public List<String> getTiepointsFileNames() {
        NodeList dataObjects = xPathHelper.getNodeList(
                "/XFDU/metadataSection/metadataObject[@repID='tiepointsSchema']", doc);
        List<String> fileNames = new ArrayList<String>();
        for (int i = 0; i < dataObjects.getLength(); i++) {
            Node item = dataObjects.item(i);
            String fileName = xPathHelper.getString("./byteStream/fileLocation/@href", item);
            fileNames.add(fileName);
        }

        return fileNames;
    }

    public List<String> getGeometryFileNames() {
        NodeList dataObjects = xPathHelper.getNodeList(
                "/XFDU/metadataSection/metadataObject[@repID='geometryDataSchema']", doc);
        List<String> fileNames = new ArrayList<String>();
        for (int i = 0; i < dataObjects.getLength(); i++) {
            Node item = dataObjects.item(i);
            String fileName = xPathHelper.getString("./byteStream/fileLocation/@href", item);
            fileNames.add(fileName);
        }

        return fileNames;
    }

//    public String getGeoCoordinatesFileName() {
//        Node geoDataObject = xPathHelper.getNode("/XFDU/metadataSection/metadataObject[@repID='geocoordinatesSchema']",
//                doc);
//        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
//    }

    public String getTimeFileName() {
        Node timeObject = xPathHelper.getNode("/XFDU/metadataSection/metadataObject[@repID='timeCoordinatesSchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", timeObject);
    }

    private String removeTimeZoneCharacter(String utcString) {
        return utcString.substring(0, utcString.length() - 1);
    }
}
