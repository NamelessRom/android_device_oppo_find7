/*
   Copyright (c) 2014, The CyanogenMod Project
   Copyright (c) 2014-2015, The NamelessRom Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cerrno>
#include <fcntl.h>
#include <linux/fs.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <selinux/selinux.h>
#include <sys/ioctl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "log.h"
#include "property_service.h"
#include "util.h"
#include "vendor_init.h"

#define CONTEXT_PARAM "u:r:lvm:s0"

#define USERDATA_PATH_LVM "/dev/lvpool/userdata"
#define USERDATA_PATH_UNIFIED "/dev/block/mmcblk0p15"

#define FSTAB_TYPE_NORMAL    1
#define FSTAB_TYPE_TWRP      2
#define FSTAB_TYPE_RECOVERY  3

static void set_xxhdpi_properties()
{
    NOTICE("Setting xxhdpi properties!");

    // dalvik
    property_set("dalvik.vm.heapstartsize", "16m");
    property_set("dalvik.vm.heapgrowthlimit", "192m");
    property_set("dalvik.vm.heapsize", "512m");
    property_set("dalvik.vm.heaptargetutilization", "0.75");
    property_set("dalvik.vm.heapminfree", "2m");
    property_set("dalvik.vm.heapmaxfree", "8m");

    // hwui
    property_set("ro.hwui.texture_cache_size", "72");
    property_set("ro.hwui.layer_cache_size", "48");
    property_set("ro.hwui.r_buffer_cache_size", "8");
    property_set("ro.hwui.path_cache_size", "32");
    property_set("ro.hwui.gradient_cache_size", "1");
    property_set("ro.hwui.drop_shadow_cache_size", "6");
    property_set("ro.hwui.texture_cache_flushrate", "0.4");
    property_set("ro.hwui.text_small_cache_width", "1024");
    property_set("ro.hwui.text_small_cache_height", "1024");
    property_set("ro.hwui.text_large_cache_width", "2048");
    property_set("ro.hwui.text_large_cache_height", "1024");
}

static void set_xxxhdpi_properties()
{
    NOTICE("Setting xxxhdpi properties!");

    // dalvik
    property_set("dalvik.vm.heapstartsize", "8m");
    property_set("dalvik.vm.heapgrowthlimit", "256m");
    property_set("dalvik.vm.heapsize", "512m");
    property_set("dalvik.vm.heaptargetutilization", "0.75");
    property_set("dalvik.vm.heapminfree", "2m");
    property_set("dalvik.vm.heapmaxfree", "8m");

    // hwui
    property_set("ro.hwui.texture_cache_size", "88");
    property_set("ro.hwui.layer_cache_size", "58");
    property_set("ro.hwui.r_buffer_cache_size", "8");
    property_set("ro.hwui.path_cache_size", "32");
    property_set("ro.hwui.gradient_cache_size", "2");
    property_set("ro.hwui.drop_shadow_cache_size", "8");
    property_set("ro.hwui.shape_cache_size", "4");
    //property_set("ro.hwui.texture_cache_flushrate", "0.4");
    property_set("ro.hwui.text_small_cache_width", "2048");
    property_set("ro.hwui.text_small_cache_height", "2048");
    property_set("ro.hwui.text_large_cache_width", "4096");
    property_set("ro.hwui.text_large_cache_height", "4096");
}

static void import_kernel_nv(char *name, __unused int for_emulator)
{
    char *value = strchr(name, '=');
    int name_len = strlen(name);

    if (value == 0) return;
    *value++ = 0;
    if (name_len == 0) return;

    if (!strcmp(name, "oppo.pcb_version")) {
        property_set("ro.oppo.pcb_version", value);

        if (!strcmp(value, "20") ||
                !strcmp(value, "21") ||
                !strcmp(value, "22") ||
                !strcmp(value, "23")) {
            property_set("ro.oppo.device", "find7s");
            property_set("ro.power_profile.override", "power_profile_find7s");
            property_set("ro.sf.lcd_density", "530");
            property_set("ro.sf.lcd_density.max", "640");
            property_set("ro.sf.lcd_density.override", "640");
            set_xxxhdpi_properties();
        } else {
            property_set("ro.oppo.device", "find7a");
            property_set("ro.power_profile.override", "power_profile_find7a");
            property_set("ro.sf.lcd_density", "480");
            property_set("ro.sf.lcd_density.max", "560");
            property_set("ro.sf.lcd_density.override", "480");
            set_xxhdpi_properties();
        }
    } else if (!strcmp(name,"oppo.rf_version")) {
        property_set("ro.oppo.rf_version", value);
    }
}


bool has_unified_layout()
{
    uint64_t size = 0;
    uint64_t border = 7 * pow(10, 9);
    bool unified = false;

    int fd = open(USERDATA_PATH_UNIFIED, O_RDONLY);
    if (fd < 0 ) {
        ERROR("could not open %s for reading: %s\n", USERDATA_PATH_UNIFIED, strerror(errno));
        goto out;
    }
    if (ioctl(fd, BLKGETSIZE64, &size) < 0) {
        ERROR("could not determine size of %s: %s\n", USERDATA_PATH_UNIFIED, strerror(errno));
        goto cleanup;
    }

    // if the data partition is larger than 7GB we probably have unified layout
    if (size > border) {
        unified = true;
    }

cleanup:
    close(fd);
out:
    return unified;
}

bool has_lvm()
{
  return (access(USERDATA_PATH_LVM, F_OK) == 0);
}

void create_fstab(std::string ending, int type)
{
    std::string fstab_dest;
    std::string fstab_source;
    struct stat stat_source;

    switch (type) {
        case FSTAB_TYPE_TWRP: {
            fstab_dest = std::string("/etc/twrp.fstab");
            fstab_source = std::string("/etc/twrp.fstab." + ending);
            break;
        }
        case FSTAB_TYPE_RECOVERY: {
            fstab_dest = std::string("/etc/recovery.fstab");
            fstab_source = std::string("/fstab.qcom." + ending);
            break;
        }
        default:
        case FSTAB_TYPE_NORMAL: {
            fstab_dest = std::string("/fstab.qcom");
            fstab_source = std::string("/fstab.qcom." + ending);
            break;
        }
    }

    int source = open(fstab_source.c_str(), O_RDONLY, 0);
    int dest = open(fstab_dest.c_str(), O_WRONLY | O_CREAT , 0644);
    fstat(source, &stat_source);
    sendfile(dest, source, 0, stat_source.st_size);
    if (source) {
        close(source);
    }
    if (dest) {
        close(dest);
    }

    NOTICE("Built fstab \"%s\" from source \"%s\"\n", fstab_dest.c_str(), fstab_source.c_str());
}

int do_exec_context(char * const command[])
{
    pid_t pid;
    int status;

    pid = fork();
    if (!pid) {
        char tmp[32];
        int fd, sz;
        get_property_workspace(&fd, &sz);
        sprintf(tmp, "%d,%d", dup(fd), sz);
        setenv("ANDROID_PROPERTY_WORKSPACE", tmp, 1);

        if (is_selinux_enabled() > 0 && setexeccon(CONTEXT_PARAM) < 0) {
            ERROR("cannot setexeccon('%s'): %s\n", CONTEXT_PARAM, strerror(errno));
            _exit(127);
        }
        execve(command[0], command, environ);
        exit(0);
    } else {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0) {
            ERROR("exec: pid %1d exited with return code %d: %s", (int)pid, WEXITSTATUS(status), strerror(status));
        }
    }
    return 0;
}

void set_oppo_layout()
{
    Timer t;
    bool is_emulated;
    bool is_twrp;
    bool is_recovery;
    std::string ending;

    if (has_lvm()) {
        property_set("ro.oppo.layout", "lvm");
        ending = "lvm";
        is_emulated = true;
    } else if (has_unified_layout()) {
        property_set("ro.oppo.layout", "unified");
        ending = "ufd";
        is_emulated = true;
    } else {
        property_set("ro.oppo.layout", "standard");
        ending = "std";
        is_emulated = false;
    }

    // check if we are inside TWRP or normal recovery
    is_twrp = (access("/etc/twrp.fstab.std", F_OK) == 0);
    is_recovery = (access("/etc/recovery.fstab", F_OK) == 0);

    // always create qcom fstab
    create_fstab(ending, FSTAB_TYPE_NORMAL);
    if (is_twrp) {
        create_fstab(ending, FSTAB_TYPE_TWRP);
    }
    if (is_recovery) {
        create_fstab(ending, FSTAB_TYPE_RECOVERY);
    }

    if (is_emulated) {
        property_set("ro.crypto.fuse_sdcard", "true");
    } else {
        property_set("ro.vold.primary_physical", "1");
    }

    NOTICE("Setting OPPO storage layout took %.2fs.\n", t.duration());
}

void scan_for_lvm()
{
    Timer t;
    char * const first_command[] = { "/sbin/lvm", "vgscan", "--mknodes", "--ignorelockingfailure", nullptr };
    char * const second_command[] = { "/sbin/lvm", "vgchange", "-aly", "--ignorelockingfailure", nullptr };

    do_exec_context(first_command);
    do_exec_context(second_command);
    NOTICE("Scanning for LVM took %.2fs.\n", t.duration());
}

int vendor_start_pre_init()
{
    Timer t;

    NOTICE("Waiting for %s...\n", USERDATA_PATH_UNIFIED);
    if (wait_for_file(USERDATA_PATH_UNIFIED, 30)) {
        ERROR("Timed out waiting for %s\n", USERDATA_PATH_UNIFIED);
    }
    NOTICE("Waiting for %s took %.2fs.\n", USERDATA_PATH_UNIFIED, t.duration());

    NOTICE("Scanning for LVM...");
    scan_for_lvm();

    NOTICE("Setting OPPO storage layout...");
    set_oppo_layout();

    NOTICE("Alex and Marc are awesome");
    return 0;
}

void vendor_load_properties()
{
    import_kernel_cmdline(0, import_kernel_nv);
}
