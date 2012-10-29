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

package org.esa.s3.synergy.util.adf;

import org.esa.beam.framework.datamodel.ProductData;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Ignore;
import org.junit.Test;

import java.io.IOException;
import java.text.DateFormat;
import java.util.Date;

public class LookupTableGeneratorTest {

    private LookupTableGenerator generator;

    @BeforeClass
    public static void setCreationTime() {
        final DateFormat dateFormat = ProductData.UTC.createDateFormat("yyyyMMdd'T'HHmmss");
        final String creationTime = dateFormat.format(new Date());
        System.setProperty("CREATION_TIME", creationTime);
    }

    @Before
    public void setUp() {
        try {
            generator = new LookupTableGenerator();
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    @Test
    public void writeSynL2RadiativeTransferSimulationDataset() throws Exception {
        generator.writeSynL2RadiativeTransferSimulationDataset();
    }

    @Test
    public void writeSynL2ConfigurationParametersDataset() throws Exception {
        generator.writeSynL2ConfigurationParametersDataset();
    }

    @Test
    @Ignore
    public void writeVgtPRadiativeTransferSimulationDataset() throws Exception {
        generator.writeVgtPRadiativeTransferSimulationDataset();
    }

    @Test
    @Ignore
    public void writeVgtPSpectralResponseDataset() throws Exception {
        generator.writeVgtPSpectralResponseDataset();
    }

    @Test
    @Ignore
    public void writeVgtPConfigurationParametersDataset() throws Exception {
        generator.writeVgtPConfigurationParametersDataset();
    }

    @Test
    public void writeVgtSRadiativeTransferSimulationDataset() throws Exception {
        generator.writeVgtSRadiativeTransferSimulationDataset();
    }

    @Test
    public void writeVgtSConfigurationParametersDataset() throws Exception {
        generator.writeVgtSConfigurationParametersDataset();
    }
}
