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

/**
 * Serves as a pointer to a measurement data set file.
 *
 * @author Marco Peters
 * @since 1.0
 */
class DataSetPointer {

    enum Type {
        M, A
    }

    private String fileName;
    private String fileFormat;
    private Type type;


    DataSetPointer(String fileName, String fileFormat, Type type) {
        this.fileName = fileName;
        this.fileFormat = fileFormat;
        this.type = type;
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }
    public String getFileFormat() {
        return fileFormat;
    }

    public Type getType() {
        return type;
    }
}
