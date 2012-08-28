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

import org.junit.Before;
import org.junit.Ignore;
import org.junit.Test;

import java.io.IOException;

@Ignore
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
        generator.writeFinalSynL2RadiativeTransferSimulationDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeSynL2ConfigurationParametersDataset() throws Exception {
        generator.writeDummySynL2ConfigurationParametersDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeVgtPRadiativeTransferSimulationDataset() throws Exception {
        generator.writeDummyVgtPRadiativeTransferSimulationDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeVgtPSpectralResponseDataset() throws Exception {
        generator.writeDummyVgtPSpectralResponseDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeVgtPConfigurationParametersDataset() throws Exception {
        generator.writeDummyVgtPConfigurationParametersDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeVgtSRadiativeTransferSimulationDataset() throws Exception {
        generator.writeDummyVgtSRadiativeTransferSimulationDataset();
    }

    @SuppressWarnings({"ForLoopReplaceableByForEach"})
    @Test
    public void writeVgtSConfigurationParametersDataset() throws Exception {
        generator.writeDummyVgtSConfigurationParametersDataset();
    }
}
