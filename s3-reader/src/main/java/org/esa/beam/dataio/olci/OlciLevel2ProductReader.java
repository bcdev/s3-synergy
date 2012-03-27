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

package org.esa.beam.dataio.olci;

import org.esa.beam.dataio.manifest.Manifest;
import org.esa.beam.dataio.manifest.ManifestProductReader;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.MetadataElement;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.RasterDataNode;
import org.esa.beam.framework.datamodel.TiePointGeoCoding;
import org.esa.beam.framework.datamodel.TiePointGrid;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

/**
 * Product reader responsible for reading OLCI Level-2 data products in SAFE format.
 *
 * @author Marco Peters
 * @author Ralf Quast
 * @since 1.0
 */
class OlciLevel2ProductReader extends ManifestProductReader {

    private static final float[] spectralWavelengths = new float[21];
    private static final float[] spectralBandwidths = new float[21];

    private int spectralBandIndex;

    static {
        getSpectralBandsProperties(spectralWavelengths, spectralBandwidths);
    }

    static void getSpectralBandsProperties(float[] wavelengths, float[] bandwidths) {
        final Properties properties = new Properties();

        try {
            properties.load(OlciLevel1ProductReader.class.getResourceAsStream("spectralBands.properties"));
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }

        for (int i = 0; i < wavelengths.length; i++) {
            wavelengths[i] = Float.parseFloat(properties.getProperty("wavelengths." + i));
        }
        for (int i = 0; i < bandwidths.length; i++) {
            bandwidths[i] = Float.parseFloat(properties.getProperty("bandwidths." + i));
        }
    }

    OlciLevel2ProductReader(OlciLevel2ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
    }

    @Override
    protected List<String> getFileNames(Manifest manifest) {
        final List<String> fileList = new ArrayList<String>();

        fileList.addAll(manifest.getFileNames("measurementDataSchema"));
        fileList.addAll(manifest.getFileNames("geoCoordinatesSchema"));
        fileList.addAll(manifest.getFileNames("tiepointsDataSchema"));

        return fileList;
    }

    @Override
    protected RasterDataNode addSpecialNode(Band sourceBand, Product targetProduct) {
        final Product sourceProduct = sourceBand.getProduct();
        final MetadataElement metadataRoot = sourceProduct.getMetadataRoot();
        final MetadataElement globalAttributes = metadataRoot.getElement("Global_Attributes");
        final int subsampling = globalAttributes.getAttributeInt("subsampling_factor");

        return copyBand(sourceBand, targetProduct, subsampling, subsampling, 0.0f, 0.0f);
    }

    @Override
    protected void configureTargetNode(Band sourceBand, RasterDataNode targetNode) {
        if (targetNode.getName().matches("R[0-9]{3}[0-9]?")) {
            if (targetNode instanceof Band) {
                final Band targetBand = (Band) targetNode;
                targetBand.setSpectralBandIndex(spectralBandIndex);
                targetBand.setSpectralWavelength(spectralWavelengths[spectralBandIndex]);
                targetBand.setSpectralBandwidth(spectralBandwidths[spectralBandIndex]);
                spectralBandIndex++;
            }
        }
    }

    @Override
    protected void setGeoCoding(Product targetProduct) throws IOException {
        final TiePointGrid latGrid = targetProduct.getTiePointGrid("TP_latitude");
        if (latGrid != null) {
            final TiePointGrid lonGrid = targetProduct.getTiePointGrid("TP_longitude");
            if (lonGrid != null) {
                targetProduct.setGeoCoding(new TiePointGeoCoding(latGrid, lonGrid));
            }
        }
    }
}
