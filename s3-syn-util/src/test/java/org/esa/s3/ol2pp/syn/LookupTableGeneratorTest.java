/*
 * Copyright (C) 2010 Brockmann Consult GmbH (info@brockmann-consult.de)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see http://www.gnu.org/licenses/
 */

package org.esa.s3.ol2pp.syn;

import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

import java.io.IOException;

import static org.junit.Assert.assertNotNull;

public class LookupTableGeneratorTest {

    private LookupTableGenerator generator;

    @Before
    public void setUp() {
        try {
            generator = new LookupTableGenerator();
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }


    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeSynL2RadiativeTransferSimulationDataset() throws Exception {
        generator.writeSynL2RadiativeTransferSimulationDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeSynL2ConfigurationParametersDataset() throws Exception {
        generator.writeSynL2ConfigurationParametersDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeVgtPRadiativeTransferSimulationDataset() throws Exception {
        generator.writeVgtPRadiativeTransferSimulationDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeVgtPSpectralResponseDataset() throws Exception {
        generator.writeVgtPSpectralResponseDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeVgtPConfigurationParametersDataset() throws Exception {
        generator.writeVgtPConfigurationParametersDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeVgtSConfigurationParametersDataset() throws Exception {
        generator.writeVgtSConfigurationParametersDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeSynVgsRadiativeTransferSimulationDataset() throws Exception {
        generator.writeVgtSRadiativeTransferSimulationDataset();
    }
}
