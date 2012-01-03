package org.esa.beam.dataio.slstr;

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
 * Class encapsulating the manifest file of an SLSTR Level 2 product.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class SlstrL2LndManifest {
    // todo
    private Document doc;
    private XPathHelper xPathHelper;

    SlstrL2LndManifest(Document manifestDocument) {
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
        List<String> fileNames = new ArrayList<String>();

        NodeList lstDataObjects = xPathHelper.getNodeList(
                "/XFDU/dataObjectSection/dataObject[@repID='LSTdataSchema']", doc);
        for (int i = 0; i < lstDataObjects.getLength(); i++) {
            Node item = lstDataObjects.item(i);
            String fileName = xPathHelper.getString("./byteStream/fileLocation/@href", item);
            fileNames.add(fileName);
        }

        NodeList frpDataObjects = xPathHelper.getNodeList(
                "/XFDU/dataObjectSection/dataObject[@repID='FRPdataSchema']", doc);
        for (int i = 0; i < frpDataObjects.getLength(); i++) {
            Node item = frpDataObjects.item(i);
            String fileName = xPathHelper.getString("./byteStream/fileLocation/@href", item);
            fileNames.add(fileName);
        }

        return fileNames;
    }

    public List<String> getVirtualDataFileNames() {
        NodeList dataObjects = xPathHelper.getNodeList(
                "/XFDU/dataObjectSection/dataObject[@repID='virtualDataSchema']", doc);
        List<String> fileNames = new ArrayList<String>();
        for (int i = 0; i < dataObjects.getLength(); i++) {
            Node item = dataObjects.item(i);
            String fileName = xPathHelper.getString("./byteStream/fileLocation/@href", item);
            fileNames.add(fileName);
        }

        return fileNames;
    }

    public String getNadirFlagsFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='nadirFlagsSchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getIndicesFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='indicesSchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getGeodeticDataCoordinatesFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='geodeticDataCoordinatesSchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getCartesianDataCoordinatesFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='cartesianDataCoordinatesSchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getGeodeticTiepointCoordinatesFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='geodeticTiepointCoordinatesSchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getCartesianTiepointCoordinatesFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='cartesianTiepointCoordinatesSchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getNadirSolarViewGeometryTiepointsFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='nadirSolarViewGeometrySchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getMeteorologicalTiePointsFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='meteorologicalDataSchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getTimeCoordinatesFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='timeCoordinatesSchema']",
                doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }


}
