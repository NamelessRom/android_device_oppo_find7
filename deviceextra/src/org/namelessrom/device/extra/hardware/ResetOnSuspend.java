package org.namelessrom.device.extra.hardware;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;

import org.namelessrom.device.extra.FileUtils;

import java.io.File;

public class ResetOnSuspend {
    public static final String TAG = "reset_on_suspend";

    private static final String PATH_RESET_ON_SUSPEND = "/sys/class/input/input0/reset_on_suspend";

    public static boolean isSupported() {
        final File f = new File(PATH_RESET_ON_SUSPEND);
        return f.exists() && f.canWrite();
    }

    public static boolean isEnabled() {
        final String s = FileUtils.readOneLine(PATH_RESET_ON_SUSPEND);
        return "1".equals(s);
    }

    public static void setEnabled(Context context, boolean enable) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        prefs.edit().putBoolean(TAG, enable).apply();

        FileUtils.writeLine(PATH_RESET_ON_SUSPEND, enable ? "1" : "0");
    }
}
