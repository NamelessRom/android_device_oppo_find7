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
import android.text.TextUtils;

import org.namelessrom.device.extra.ColorEnhancement;

public class ControlReceiver extends BroadcastReceiver {
    private static final String ACTION_CONTROL = "org.namelessrom.device.extra.ACTION_CONTROL";

    private static final String EXTRA_CONTROL = "control";
    private static final String EXTRA_VALUE = "value";

    @Override
    public void onReceive(final Context context, final Intent intent) {
        if (intent == null || !ACTION_CONTROL.equals(intent.getAction())) {
            // get out of here
            return;
        }

        // get the control which should get edited
        String control = intent.getStringExtra(EXTRA_CONTROL);
        if (!TextUtils.isEmpty(control)) {
            switch (control) {
                case ColorEnhancement.TAG: {
                    boolean useColorEnhancement = intent.getBooleanExtra(EXTRA_VALUE, false);
                    ColorEnhancement.setColorEnhancement(context, useColorEnhancement);
                    break;
                }
            }
        }
    }

}
