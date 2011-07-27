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
