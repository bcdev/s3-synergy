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

package org.esa.beam.dataio.synergy;

import org.esa.beam.framework.datamodel.ProductData;
import org.esa.beam.util.XPathHelper;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.xpath.XPathFactory;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;

/**
 * Class encapsulating the manifest file of Sentinel-3 Synergy products.
 *
 * @author Olaf Danne
 * @author Ralf Quast
 * @since 1.0
 */
class Manifest {

    private final Document doc;
    private final String annotationDataSectionPath;
    private final XPathHelper xPathHelper;

    public static Manifest createManifest(Document manifestDocument) {
        final XPathHelper helper = new XPathHelper(XPathFactory.newInstance().newXPath());
        final String description = helper.getString("/XFDU/informationPackageMap/contentUnit/@textInfo",
                                                    manifestDocument);
        if (description.contains("VGT")) {
            return new Manifest(manifestDocument, "dataObjectSection/dataObject");
        } else {
            return new Manifest(manifestDocument, "metadataSection/metadataObject");
        }
    }

    private Manifest(Document manifestDocument, String path) {
        doc = manifestDocument;
        annotationDataSectionPath = path;
        xPathHelper = new XPathHelper(XPathFactory.newInstance().newXPath());
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

    public List<String> getTiePointFileNames() {
        NodeList dataObjects = xPathHelper.getNodeList(
                "/XFDU/" + annotationDataSectionPath + "[@repID='tiepointsSchema']", doc);
        List<String> fileNames = new ArrayList<String>();
        for (int i = 0; i < dataObjects.getLength(); i++) {
            Node item = dataObjects.item(i);
            String fileName = xPathHelper.getString("./byteStream/fileLocation/@href", item);
            fileNames.add(fileName);
        }

        return fileNames;
    }

    public String getGeoCoordinatesFileName() {
        Node geoDataObject = xPathHelper.getNode("/XFDU/metadataSection/metadataObject[@repID='geocoordinatesSchema']",
                                                 doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", geoDataObject);
    }

    public String getStatusFlagFileName() {
        Node statusFlagObject = xPathHelper.getNode("/XFDU/dataObjectSection/dataObject[@repID='statusFlagsSchema']",
                                                    doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", statusFlagObject);
    }

    public String getTimeFileName() {
        Node timeObject = xPathHelper.getNode("/XFDU/" + annotationDataSectionPath + "[@repID='timeCoordinatesSchema']",
                                              doc);
        return xPathHelper.getString("./byteStream/fileLocation/@href", timeObject);
    }

    public List<String> getGeometryFileNames() {
        NodeList geometryObjects = xPathHelper.getNodeList(
                "/XFDU/dataObjectSection/dataObject[@repID='geometryDataSchema']", doc);
        List<String> fileNames = new ArrayList<String>();
        for (int i = 0; i < geometryObjects.getLength(); i++) {
            Node item = geometryObjects.item(i);
            String fileName = xPathHelper.getString("./byteStream/fileLocation/@href", item);
            fileNames.add(fileName);
        }

        return fileNames;
    }

    private String removeTimeZoneCharacter(String utcString) {
        return utcString.substring(0, utcString.length() - 1);
    }
}
