package org.esa.s3.synergy.util;

/*
 * Copyright (C) 2012 Brockmann Consult GmbH (info@brockmann-consult.de)
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

import org.esa.beam.framework.datamodel.ProductData;
import org.junit.Test;

public class DateConverterTest {

    private static final long MICROS_PER_SECOND = 1000000L;
    private static final long MICROS_PER_DAY = 24L * 60L * 60L * MICROS_PER_SECOND;

    @Test
    public void testConvertMicrosToDate() throws Exception {
        final long micros = 425124560655099L;

        final int d = days(micros);
        final int s = seconds(micros);
        final int m = micros(micros);
        System.out.println("d = " + d);
        System.out.println("s = " + s);
        System.out.println("m = " + m);
        final ProductData.UTC utc = new ProductData.UTC(d, s, m);
        System.out.println(utc);
    }

    private int days(long micros) {
        return (int) (micros / MICROS_PER_DAY);
    }

    private int seconds(long micros) {
        return (int) ((micros - days(micros) * MICROS_PER_DAY) / MICROS_PER_SECOND);
    }

    private int micros(long micros) {
        return (int) (micros - days(micros) * MICROS_PER_DAY - seconds(micros) * MICROS_PER_SECOND);
    }
}
