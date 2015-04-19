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

package org.namelessrom.setupwizard.device;

import android.app.Fragment;
import android.app.FragmentManager;
import android.content.Context;
import android.content.Intent;
import android.graphics.Typeface;
import android.os.Bundle;
import android.os.UserHandle;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.style.StyleSpan;
import android.view.View;
import android.widget.CheckBox;
import android.widget.TextView;

import com.cyanogenmod.setupwizard.setup.Page;
import com.cyanogenmod.setupwizard.setup.SetupDataCallbacks;
import com.cyanogenmod.setupwizard.setup.SetupPage;
import com.cyanogenmod.setupwizard.ui.SetupPageFragment;

import org.namelessrom.setupwizard.R;
import org.namelessrom.setupwizard.SetupWizardApp;

public class Find7SpecificPage extends SetupPage {
    public static final String TAG = "Find7SpecificPage";

    public Find7SpecificPage(Context context, SetupDataCallbacks callbacks) {
        super(context, callbacks);
    }

    @Override
    public Fragment getFragment(FragmentManager fragmentManager, int action) {
        Fragment fragment = fragmentManager.findFragmentByTag(getKey());
        if (fragment == null) {
            Bundle args = new Bundle();
            args.putString(Page.KEY_PAGE_ARGUMENT, getKey());
            args.putInt(Page.KEY_PAGE_ACTION, action);
            fragment = new Find7SettingsFragment();
            fragment.setArguments(args);
        }
        return fragment;
    }

    @Override
    public String getKey() {
        return TAG;
    }

    @Override
    public int getTitleResId() {
        return R.string.setup_services_find7;
    }

    @Override
    public void onFinishSetup() { }

    public static class Find7SettingsFragment extends SetupPageFragment {
        private static final String ACTION_CONTROL = "org.namelessrom.device.extra.ACTION_CONTROL";

        private static final String EXTRA_CONTROL = "control";
        private static final String EXTRA_VALUE = "value";

        private View mColorEnhancementRow;
        private CheckBox mColorEnhancement;

        private View.OnClickListener mColorEnhancementClickListener = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                boolean checked = !mColorEnhancement.isChecked();
                mColorEnhancement.setChecked(checked);
                setColorEnhancement(checked);
            }
        };

        @Override
        protected void initializePage() {
            mColorEnhancementRow = mRootView.findViewById(R.id.find7_color_enhancement);
            mColorEnhancementRow.setOnClickListener(mColorEnhancementClickListener);
            String useColorEnhancement = getString(R.string.find7_color_enhancement);
            String useColorEnhancementSummary = getString(R.string.find7_color_enhancement_summary,
                    useColorEnhancement);
            final SpannableStringBuilder colorEnhancementSpan =
                    new SpannableStringBuilder(useColorEnhancementSummary);
            colorEnhancementSpan.setSpan(new StyleSpan(Typeface.BOLD),
                    0, useColorEnhancement.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
            TextView colorEnhancement =
                    (TextView) mRootView.findViewById(R.id.find7_enable_color_enhancement_summary);
            colorEnhancement.setText(colorEnhancementSpan);

            mColorEnhancement =
                    (CheckBox) mRootView.findViewById(R.id.find7_enable_color_enhancement_checkbox);
        }

        @Override
        protected int getLayoutResource() {
            return R.layout.setup_find7_services;
        }

        private void setColorEnhancement(boolean checked) {
            final Intent intent = new Intent(ACTION_CONTROL);
            intent.putExtra(EXTRA_CONTROL, "color_enhancement");
            intent.putExtra(EXTRA_VALUE, checked);
            SetupWizardApp.get().sendBroadcastAsUser(intent, UserHandle.CURRENT);
        }

    }

}
