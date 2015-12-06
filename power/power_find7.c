/*
 * Copyright (C) 2014 The Android Open Source Project
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
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <cutils/uevent.h>
#include <errno.h>
#include <sys/poll.h>
#include <pthread.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <stdbool.h>

#define LOG_TAG "Find7 PowerHAL"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#define NODE_MAX (64)

#define BOOSTPULSE_INTERACTIVE "/sys/devices/system/cpu/cpufreq/interactive/boostpulse"
#define NOTIFY_ON_MIGRATE "/dev/cpuctl/cpu.notify_on_migrate"

#define DEFAULT_DURATION 1000

#define INPUT_BOOST_MS_PATH "/sys/module/cpu_boost/parameters/input_boost_ms"

static int input_boost_ms = 0;
static int off_input_boost_ms = 0;
static int last_state = -1;

struct find7_power_module {
    struct power_module base;
    pthread_mutex_t lock;
    int boostpulse_fd;
    int boostpulse_warned;
};

static int socket_init(struct find7_power_module *find)
{
    char buf[80];

    pthread_mutex_lock(&find->lock);

    if (find->boostpulse_fd < 0) {
        find->boostpulse_fd = open(BOOSTPULSE_INTERACTIVE, O_WRONLY);

        if (find->boostpulse_fd < 0 && !find->boostpulse_warned) {
            strerror_r(errno, buf, sizeof(buf));
            ALOGV("Error opening boostpulse: %s\n", buf);
            find->boostpulse_warned = 1;
        } else if (find->boostpulse_fd > 0) {
            ALOGD("Opened boostpulse interface");
        }
    }

    pthread_mutex_unlock(&find->lock);
    return find->boostpulse_fd;
}

static int sysfs_read(char *path, char *s, int num_bytes)
{
    char buf[80];
    int count;
    int ret = 0;
    int fd = open(path, O_RDONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);

        return -1;
    }

    if ((count = read(fd, s, num_bytes - 1)) < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error reading %s: %s\n", path, buf);

        ret = -1;
    } else {
        s[count] = '\0';
    }

    close(fd);

    return ret;
}

static int sysfs_write(const char *path, char *s)
{
    char buf[80];
    int len;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);
        return -1;
    }

    close(fd);
    return 0;
}

static void power_init(struct power_module *module)
{
    struct find7_power_module *find = (struct find7_power_module *) module;

    ALOGI("%s", __func__);
    socket_init(find);
}

static void touch_boost(struct power_module *module, void *data, int duration)
{
    struct find7_power_module *find = (struct find7_power_module *) module;
    int len;
    char buf[80];

    if (socket_init(find) < 0) {
        ALOGV("%s: boostpulse socket not created", __func__);
        return;
    }
    if (data != NULL)
        duration = (int) data;

    snprintf(buf, sizeof(buf), "%d", duration);
    len = write(find->boostpulse_fd, buf, strlen(buf));
    ALOGV("%s: duration: %d", __func__, duration);

    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("%s: Error writing to boostpulse: %s\n", __func__, buf);

        pthread_mutex_lock(&find->lock);
        close(find->boostpulse_fd);
        find->boostpulse_fd = -1;
        find->boostpulse_warned = 0;
        pthread_mutex_unlock(&find->lock);
    }
}

static void power_set_interactive(__attribute__((unused)) struct power_module *module, int on)
{
    char tmp_str[NODE_MAX];
    int tmp;

    ALOGV("%s: %s", __func__, (on ? "ON" : "OFF"));
    if (on) {
        /* Display on */
        if (!sysfs_read(INPUT_BOOST_MS_PATH, tmp_str, NODE_MAX - 1)) {
            tmp = atoi(tmp_str);
            if (!input_boost_ms || (input_boost_ms != tmp && off_input_boost_ms != tmp)) {
                input_boost_ms = tmp;
            }

            snprintf(tmp_str, NODE_MAX, "%d", input_boost_ms);
            sysfs_write(INPUT_BOOST_MS_PATH, tmp_str);
        } else {
            ALOGE("Failed to read %s", INPUT_BOOST_MS_PATH);
        }
    } else {
        /* Display off */
        if (!sysfs_read(INPUT_BOOST_MS_PATH, tmp_str, NODE_MAX - 1)) {
            tmp = atoi(tmp_str);
            if (!input_boost_ms || (input_boost_ms != tmp && off_input_boost_ms != tmp)) {
                input_boost_ms = tmp;
            }

            snprintf(tmp_str, NODE_MAX, "%d", off_input_boost_ms);
            sysfs_write(INPUT_BOOST_MS_PATH, tmp_str);
        } else {
            ALOGE("Failed to read %s", INPUT_BOOST_MS_PATH);
        }
    }
    if (last_state == -1) {
        last_state = on;
    } else {
        if (last_state == on)
            return;
        else
            last_state = on;
    }

    sysfs_write(NOTIFY_ON_MIGRATE, on ? "1" : "0");
}

static void power_hint(struct power_module *module, power_hint_t hint, void *data)
{
    int cpu, ret;

    switch (hint) {
        case POWER_HINT_CPU_BOOST:
            ALOGV("%s: POWER_HINT_CPU_BOOST", __func__);
            touch_boost(module, data, (int) data / 1000);
            break;
        case POWER_HINT_INTERACTION:
            ALOGV("%s: POWER_HINT_INTERACTION", __func__);
            touch_boost(module, data, DEFAULT_DURATION);
            break;
        case POWER_HINT_LAUNCH_BOOST:
            ALOGV("%s: POWER_HINT_LAUNCH_BOOST", __func__);
            touch_boost(module, data, 2000);
            break;
#if 0
        case POWER_HINT_VSYNC:
            ALOGV("POWER_HINT_VSYNC %s", (data ? "ON" : "OFF"));
            break;

        case POWER_HINT_VIDEO_ENCODE:
            process_video_encode_hint(data);
            break;
#endif
        case POWER_HINT_LOW_POWER:
            ALOGV("%s: POWER_HINT_LOW_POWER handled by power profiles", __func__);
            break;
        default:
            break;
    }
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct find7_power_module HAL_MODULE_INFO_SYM = {
    .base = {
        .common = {
            .tag = HARDWARE_MODULE_TAG,
            .module_api_version = POWER_MODULE_API_VERSION_0_2,
            .hal_api_version = HARDWARE_HAL_API_VERSION,
            .id = POWER_HARDWARE_MODULE_ID,
            .name = "Find7 Power HAL",
            .author = "The NamelessRom Project",
            .methods = &power_module_methods,
        },

        .init = power_init,
        .setInteractive = power_set_interactive,
        .powerHint = power_hint,
    },

    .lock = PTHREAD_MUTEX_INITIALIZER,
    .boostpulse_fd = -1,
    .boostpulse_warned = 0,
};
