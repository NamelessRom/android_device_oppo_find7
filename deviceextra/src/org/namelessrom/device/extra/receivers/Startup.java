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

package org.namelessrom.device.extra.receivers;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

import org.namelessrom.device.extra.hardware.ColorEnhancement;
import org.namelessrom.device.extra.hardware.ResetOnSuspend;

public class Startup extends BroadcastReceiver {
    @Override
    public void onReceive(final Context context, final Intent intent) {
        if (intent == null || !Intent.ACTION_BOOT_COMPLETED.equals(intent.getAction())) {
            // get out of here
            return;
        }

        // get our preferences to restore values
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);

        // restore color enhancement
        boolean useColorEnhancement = prefs.getBoolean(ColorEnhancement.TAG, true);
        ColorEnhancement.setColorEnhancement(context, useColorEnhancement);

        // restore reset on suspend
        boolean useResetOnSuspend = prefs.getBoolean(ResetOnSuspend.TAG, false);
        ResetOnSuspend.setEnabled(context, useResetOnSuspend);
    }

}
