/*
 * Copyright (C) 2012-2013 The CyanogenMod Project
 * Poritions Copyright (C) 2015 The Evervolv Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.evervolv.toolbox.fragments;

import android.content.Context;
import android.content.res.Resources;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.preference.PreferenceScreen;
import android.util.Log;

import com.evervolv.toolbox.R;
import com.evervolv.toolbox.Toolbox;
import com.evervolv.toolbox.custom.VibratorTuningPreference;
import com.evervolv.toolbox.misc.FileUtil;

public class DeviceHaptic extends DeviceSettingFragment {

    private static final String TAG = "DeviceSettings_Haptic";
    public static final String KEY_VIBRATOR_TUNING = "vibrator_tuning";

    private VibratorTuningPreference mVibratorTuning;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Resources res = getResources();

        addPreferencesFromResource(R.xml.device_haptic);

        mVibratorTuning = (VibratorTuningPreference) findPreference(
                KEY_VIBRATOR_TUNING);
        mVibratorTuning.setEnabled(Toolbox.isEnabled(getActivity()));
    }

    @Override
    public void restore(Context context, boolean toolboxEnabled) {
        if (mVibratorTuning != null) {
            mVibratorTuning.setEnabled(toolboxEnabled);
        }
        if (toolboxEnabled) {
            VibratorTuningPreference.restore(context);
        } else {
            FileUtil.fileWriteOneLine(VibratorTuningPreference.FILE_PATH,
                    Integer.toString(VibratorTuningPreference.DEFAULT_VALUE));
        }
    }

}
