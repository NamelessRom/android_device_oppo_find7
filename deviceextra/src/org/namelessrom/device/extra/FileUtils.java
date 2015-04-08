/*
 * <!--
 *    Copyright (C) 2015 The NamelessRom Project
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 * -->
 */

package org.namelessrom.device.extra;

import android.util.Log;

import java.io.BufferedReader;
import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

public final class FileUtils {
    private static final String TAG = "FileUtils";

    private FileUtils() { }

    /**
     * Checks if the current File, given as path, exists.
     */
    public static boolean fileExists(final String path) {
        return new File(path).exists();
    }

    /**
     * Reads the first line of text from the given file
     */
    public static String readOneLine(String fileName) {
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new FileReader(fileName), 512);
            return reader.readLine();
        } catch (IOException e) {
            Log.e(TAG, "Could not read from file " + fileName, e);
        } finally {
            closeQuietly(reader);
        }

        return null;
    }

    /**
     * Writes the given value into the given file
     *
     * @return true on success, false on failure
     */
    public static boolean writeLine(String fileName, String value) {
        FileOutputStream fos = null;
        OutputStreamWriter osw = null;
        PrintWriter pw = null;
        try {
            fos = new FileOutputStream(fileName);
            osw = new OutputStreamWriter(fos);
            pw = new PrintWriter(osw);
            pw.println(value);
            pw.flush();
        } catch (IOException e) {
            Log.e(TAG, "Could not write to file " + fileName, e);
            return false;
        } finally {
            closeQuietly(pw);
            closeQuietly(osw);
            closeQuietly(fos);
        }

        return true;
    }

    private static void closeQuietly(Closeable closeable) {
        if (closeable == null) {
            return;
        }
        try {
            closeable.close();
        } catch (IOException ignored) { }
    }
}
