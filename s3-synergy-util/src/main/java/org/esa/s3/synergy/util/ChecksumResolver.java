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

package org.esa.s3.synergy.util;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Locale;
import java.util.Properties;
import java.util.Scanner;

/**
 * Utility for replacing checksums in a SAFE manifest template.
 *
 * @author Ralf Quast
 */
public class ChecksumResolver {

    public static void main(String[] args) {
        final String templatePath = args[0];
        final String checksumPath = args[1];
        final String targetPath = args[2];

        try {
            final Properties properties = readChecksumFile(checksumPath);
            final TemplateResolver templateResolver = new TemplateResolver(properties);

            BufferedReader reader = null;
            BufferedWriter writer = null;
            try {
                final InputStream is = new FileInputStream(templatePath);
                reader = new BufferedReader(new InputStreamReader(is, "US-ASCII"));
                writer = new BufferedWriter(new FileWriter(targetPath));
                String line = reader.readLine();
                while (line != null) {
                    line = templateResolver.resolve(line);
                    writer.write(line + "\n");
                    line = reader.readLine();
                }
            } finally {
                try {
                    if (writer != null) {
                        writer.close();
                    }
                    if (reader != null) {
                        reader.close();
                    }
                } catch (IOException e) {
                    // ignore
                }
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static Properties readChecksumFile(String filePath) throws IOException {
        final Properties properties = new Properties();
        final Scanner scanner = new Scanner(new File(filePath), "US-ASCII");
        scanner.useLocale(Locale.US);
        try {
            while (scanner.hasNextLine()) {
                final String line = scanner.nextLine();
                String[] valueKeyPair = line.split("\\s+\\*?");
                properties.put("checksum-" + valueKeyPair[1], valueKeyPair[0]);
            }
        } finally {
            scanner.close();
        }
        return properties;
    }
}
