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

import org.esa.beam.framework.dataio.DecodeQualification;
import org.esa.beam.framework.dataio.ProductReader;
import org.esa.beam.framework.dataio.ProductReaderPlugIn;
import org.esa.beam.util.io.BeamFileFilter;

import java.util.Locale;

/**
 * PlugIn class which provides a Level 1 Sentinel 3 SLSTR product reader to the framework.
 *
 * @author Olaf Danne
 * @since 1.0
 */
public class SlstrLevel1ProductReaderPlugin implements ProductReaderPlugIn {
    @Override
    public DecodeQualification getDecodeQualification(Object input) {
        // todo
        return null;
    }

    @Override
    public Class[] getInputTypes() {
        // todo
        return new Class[0];
    }

    @Override
    public ProductReader createReaderInstance() {
        // todo
        return null;
    }

    @Override
    public String[] getFormatNames() {
        // todo
        return new String[0];
    }

    @Override
    public String[] getDefaultFileExtensions() {
        // todo
        return new String[0];
    }

    @Override
    public String getDescription(Locale locale) {
        // todo
        return null;
    }

    @Override
    public BeamFileFilter getProductFileFilter() {
        // todo
        return null;
    }
}
