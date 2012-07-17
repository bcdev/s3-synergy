/*
 * Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
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

package org.esa.s3.synergy.util.dictionary;

import org.esa.s3.synergy.util.dictionary.DictionaryGenerator;
import org.junit.Test;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import static org.junit.Assert.*;

public class DictionaryGeneratorTest {

    @Test
    public void testLoadProperties() throws IOException {
        Properties segmentNames = new Properties();
        final InputStream is = DictionaryGenerator.class.getResourceAsStream("dictionary/segment_names.properties");
        assertNotNull(is);
        segmentNames.load(is);
        assertEquals(segmentNames.getProperty("L2_SYN_Surface_Directional_Reflectance"), "SYN_COLLOCATED");
    }

}
