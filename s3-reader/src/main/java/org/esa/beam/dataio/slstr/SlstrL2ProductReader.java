package org.esa.beam.dataio.slstr;

import com.bc.ceres.glevel.MultiLevelImage;
import org.esa.beam.dataio.manifest.ManifestProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.MetadataElement;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.RasterDataNode;
import org.esa.beam.framework.datamodel.TiePointGeoCoding;
import org.esa.beam.framework.datamodel.TiePointGrid;

import java.io.IOException;

public abstract class SlstrL2ProductReader extends ManifestProductReader {

    private double nadStartOffset;
    private double nadTrackOffset;
    private short[] nadResolutions;

    protected SlstrL2ProductReader(ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
    }

    @Override
    protected final void setAutoGrouping(Product[] sourceProducts, Product targetProduct) {
        final StringBuilder patternBuilder = new StringBuilder();
        for (final Product sourceProduct : sourceProducts) {
            if (sourceProduct.getAutoGrouping() != null) {
                for (final String[] groups : sourceProduct.getAutoGrouping()) {
                    if (patternBuilder.length() > 0) {
                        patternBuilder.append(":");
                    }
                    patternBuilder.append(sourceProduct.getName());
                    for (final String group : groups) {
                        patternBuilder.append("/");
                        patternBuilder.append(group);
                    }
                }
            }
            if (patternBuilder.length() > 0) {
                patternBuilder.append(":");
            }
            patternBuilder.append(sourceProduct.getName());
        }
        targetProduct.setAutoGrouping(patternBuilder.toString());
    }

    @Override
    protected final void initialize(Product[] sourceProducts, Product targetProduct) {
        final MetadataElement globalAttributes = sourceProducts[0].getMetadataRoot().getElement("Global_Attributes");
        nadStartOffset = globalAttributes.getAttributeDouble("start_offset");
        nadTrackOffset = globalAttributes.getAttributeDouble("track_offset");
        nadResolutions = (short[]) globalAttributes.getAttribute("resolution").getDataElems();
    }

    @Override
    protected final TiePointGrid addTiePointGrid(Band sourceBand, Product targetProduct) {
        final Product sourceProduct = sourceBand.getProduct();
        final MetadataElement globalAttributes = sourceProduct.getMetadataRoot().getElement("Global_Attributes");
        final double tieStartOffset = globalAttributes.getAttributeDouble("start_offset");
        final double tieTrackOffset = globalAttributes.getAttributeDouble("track_offset");
        final short[] tieResolutions = (short[]) globalAttributes.getAttribute("resolution").getDataElems();
        final MultiLevelImage sourceImage = sourceBand.getGeophysicalImage();
        final int w = sourceImage.getWidth();
        final int h = sourceImage.getHeight();
        final float[] tiePointGridData = sourceImage.getData().getSamples(0, 0, w, h, 0, new float[w * h]);

        final int subSamplingX = tieResolutions[0] / nadResolutions[0];
        final int subSamplingY;
        if (tieResolutions.length == 2) {
            subSamplingY = tieResolutions[1] / nadResolutions[1];
        } else {
            subSamplingY = tieResolutions[0] / nadResolutions[1];
        }
        final float offsetX = (float) (nadTrackOffset - tieTrackOffset * subSamplingX);
        final float offsetY = (float) (tieStartOffset * subSamplingY - nadStartOffset);
        final TiePointGrid tiePointGrid = new TiePointGrid(sourceBand.getName(), w, h, offsetX, offsetY,
                                                           subSamplingX,
                                                           subSamplingY,
                                                           tiePointGridData, true);
        targetProduct.addTiePointGrid(tiePointGrid);
        return tiePointGrid;
    }

    @Override
    protected final void configureTargetNode(Band sourceBand, RasterDataNode targetNode) {
        targetNode.setName(sourceBand.getProduct().getName() + "_" + sourceBand.getName());
    }

    @Override
    protected void setGeoCoding(Product targetProduct) throws IOException {
        final TiePointGrid latGrid = targetProduct.getTiePointGrid("latitude");
        if (latGrid != null) {
            final TiePointGrid lonGrid = targetProduct.getTiePointGrid("longitude");
            if (lonGrid != null) {
                targetProduct.setGeoCoding(new TiePointGeoCoding(latGrid, lonGrid));
            }
        }
    }
}
