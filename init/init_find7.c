/*
   Copyright (c) 2014, The CyanogenMod Project
   Copyright (c) 2014, The NamelessRom Project

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

#include <fcntl.h>
#include <linux/fs.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

static void set_xxhdpi_properties()
{
    INFO("Setting xxhdpi properties!");

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
    INFO("Setting xxxhdpi properties!");

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

static void import_kernel_nv(char *name, int for_emulator)
{
    char *value = strchr(name, '=');
    int name_len = strlen(name);

    if (value == 0) return;
    *value++ = 0;
    if (name_len == 0) return;

    if (!strcmp(name, "oppo.pcb_version")) {
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
    }
}

static bool has_unified_layout()
{
    const char* datadevice="/dev/block/mmcblk0p15";
    uint64_t size = 0;
    uint64_t border = 7 * pow(10, 9);
    bool unified = false;

    int fd = open(datadevice, O_RDONLY);
    if (fd < 0 ) {
        ERROR("could not open %s for reading: %s\n", datadevice, strerror(errno));
        goto out;
    }
    if (ioctl(fd, BLKGETSIZE64, &size) < 0) {
        ERROR("could not determine size of %s: %s\n", datadevice, strerror(errno));
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

static bool has_lvm()
{
  return ( access("/dev/lvpool/userdata", F_OK ) == 0 );
}

static void set_oppo_layout()
{
    if (has_lvm()) {
        property_set("ro.oppo.layout", "lvm");
    } else if (has_unified_layout()) {
        property_set("ro.oppo.layout", "unified");
    } else {
        property_set("ro.oppo.layout", "standard");
    }
}

void vendor_load_properties()
{
    import_kernel_cmdline(0, import_kernel_nv);
    set_oppo_layout();
}
