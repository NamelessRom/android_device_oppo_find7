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

import android.app.ActionBar;
import android.os.Bundle;
import android.os.SystemProperties;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceCategory;
import android.preference.SwitchPreference;
import android.text.TextUtils;
import android.view.MenuItem;

@SuppressWarnings("deprecation")
public class SettingsActivity extends PreferenceActivity implements Preference.OnPreferenceChangeListener {
    private static final String CATEGORY_INFO = "cat_info";

    private static final String KEY_COLOR_ENHANCEMENT = "color_enhancement";

    private static final String KEY_INFO_DEVICE = "info_device";
    private static final String KEY_INFO_STORAGE_LAYOUT = "info_storage_layout";
    private static final String KEY_INFO_PCB_VERSION = "info_pcb_version";
    private static final String KEY_INFO_RF_VERSION = "info_rf_version";

    private static final String PROP_DEVICE = "ro.oppo.device";
    private static final String PROP_STORAGE_LAYOUT = "ro.oppo.layout";
    private static final String PROP_PCB_VERSION = "ro.oppo.pcb_version";
    private static final String PROP_RF_VERSION = "ro.oppo.rf_version";

    private static final String VALUE_UNKNOWN = "-";
    private static final String VALUE_LVM = "lvm";
    private static final String VALUE_UNIFIED = "ufd";
    private static final String VALUE_STANDARD = "std";

    private static final String VALUE_FIND7_A = "find7a";
    private static final String VALUE_FIND7_S = "find7s";

    private SwitchPreference mColorEnhancement;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.device_settings);

        final ActionBar actionBar = getActionBar();
        if (actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
        }

        mColorEnhancement = (SwitchPreference) findPreference(KEY_COLOR_ENHANCEMENT);
        if (ColorEnhancement.isSupported()) {
            mColorEnhancement.setOnPreferenceChangeListener(this);
        } else {
            mColorEnhancement.setEnabled(false);
        }

        final PreferenceCategory catInfo = (PreferenceCategory) findPreference(CATEGORY_INFO);
        String tmp;

        // Device
        final Preference infoDevice = findPreference(KEY_INFO_DEVICE);
        tmp = SystemProperties.get(PROP_DEVICE, VALUE_UNKNOWN);
        if (TextUtils.isEmpty(tmp) || VALUE_UNKNOWN.equals(tmp)) {
            catInfo.removePreference(infoDevice);
        } else {
            switch (tmp) {
                case VALUE_FIND7_A: {
                    infoDevice.setSummary(R.string.device_find7_a);
                    break;
                }
                case VALUE_FIND7_S: {
                    infoDevice.setSummary(R.string.device_find7_s);
                    break;
                }
                default: {
                    catInfo.removePreference(infoDevice);
                    break;
                }
            }
        }

        // Storage layout
        final Preference infoStorageLayout = findPreference(KEY_INFO_STORAGE_LAYOUT);
        tmp = SystemProperties.get(PROP_STORAGE_LAYOUT, VALUE_UNKNOWN);
        if (TextUtils.isEmpty(tmp) || VALUE_UNKNOWN.equals(tmp)) {
            catInfo.removePreference(infoStorageLayout);
        } else {
            switch (tmp) {
                case VALUE_LVM: {
                    infoStorageLayout.setSummary(R.string.storage_layout_lvm);
                    break;
                }
                case VALUE_UNIFIED: {
                    infoStorageLayout.setSummary(R.string.storage_layout_unified);
                    break;
                }
                case VALUE_STANDARD: {
                    infoStorageLayout.setSummary(R.string.storage_layout_standard);
                    break;
                }
                default: {
                    catInfo.removePreference(infoStorageLayout);
                    break;
                }
            }
        }

        // PCB version
        final Preference infoPcb = findPreference(KEY_INFO_PCB_VERSION);
        tmp = SystemProperties.get(PROP_PCB_VERSION, VALUE_UNKNOWN);
        if (TextUtils.isEmpty(tmp) || VALUE_UNKNOWN.equals(tmp)) {
            catInfo.removePreference(infoPcb);
        } else {
            infoPcb.setSummary(tmp);
        }

        // RF version
        final Preference infoRf = findPreference(KEY_INFO_RF_VERSION);
        tmp = SystemProperties.get(PROP_RF_VERSION, VALUE_UNKNOWN);
        if (TextUtils.isEmpty(tmp) || VALUE_UNKNOWN.equals(tmp)) {
            catInfo.removePreference(infoRf);
        } else {
            infoRf.setSummary(tmp);
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home: {
                finish();
                return true;
            }
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object o) {
        if (preference == mColorEnhancement) {
            boolean value = (Boolean) o;
            ColorEnhancement.setColorEnhancement(this, value);
            return true;
        }

        return false;
    }
}
