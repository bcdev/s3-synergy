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

package org.esa.beam.dataio.slstr;

import com.bc.ceres.glevel.MultiLevelImage;
import org.esa.beam.dataio.manifest.Manifest;
import org.esa.beam.dataio.manifest.ManifestProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.framework.datamodel.Band;
import org.esa.beam.framework.datamodel.MetadataElement;
import org.esa.beam.framework.datamodel.Product;
import org.esa.beam.framework.datamodel.RasterDataNode;
import org.esa.beam.framework.datamodel.TiePointGeoCoding;
import org.esa.beam.framework.datamodel.TiePointGrid;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Product reader responsible for reading SLSTR SST data products in SAFE format.
 *
 * @author Olaf Danne
 * @author Ralf Quast
 * @since 1.0
 */
public class SlstrSstProductReader extends SlstrL2ProductReader {

    public SlstrSstProductReader(ProductReaderPlugIn readerPlugIn) {
        super(readerPlugIn);
    }

    @Override
    protected List<String> getFileNames(Manifest manifest) {
        final List<String> fileNames = new ArrayList<String>();
        fileNames.addAll(manifest.getFileNames("N2_SST_schema"));
        fileNames.addAll(manifest.getFileNames("N3R_SST_schema"));
        fileNames.addAll(manifest.getFileNames("N3_SST_schema"));

        fileNames.addAll(manifest.getFileNames("geodeticTiepointCoordinatesSchema"));
        fileNames.addAll(manifest.getFileNames("cartesianTiepointCoordinatesSchema"));
        fileNames.addAll(manifest.getFileNames("nadirSolarViewGeometrySchema"));
        fileNames.addAll(manifest.getFileNames("meteorologicalDataSchema"));

        fileNames.addAll(manifest.getFileNames("nadirFlagsSchema"));
        fileNames.addAll(manifest.getFileNames("nadirIndicesSchema"));

        // TODO - time data are provided in a 64-bit variable, so we currently don't use them

        return fileNames;
    }
}
