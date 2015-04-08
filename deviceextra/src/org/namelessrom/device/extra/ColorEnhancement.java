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

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

public class ColorEnhancement {
    public static final String TAG = "color_enhancement";

    private static final String KCAL_SAT = "/sys/devices/platform/kcal_ctrl.0/kcal_sat";
    private static final String KCAL_VAL = "/sys/devices/platform/kcal_ctrl.0/kcal_val";
    private static final String KCAL_CONT = "/sys/devices/platform/kcal_ctrl.0/kcal_cont";

    public static boolean isSupported() {
        return FileUtils.fileExists(KCAL_SAT)
                && FileUtils.fileExists(KCAL_VAL)
                && FileUtils.fileExists(KCAL_CONT);
    }

    public static void setColorEnhancement(Context context, boolean enable) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        prefs.edit().putBoolean(TAG, enable).apply();

        FileUtils.writeLine(KCAL_SAT, enable ? "275" : "255");
        FileUtils.writeLine(KCAL_VAL, enable ? "251" : "255");
        FileUtils.writeLine(KCAL_CONT, enable ? "258" : "255");
    }
}
