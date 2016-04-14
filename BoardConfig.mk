#
# Copyright (C) 2014 The CyanogenMod Project
# Copyright (C) 2014 The NamelessRom Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Inherit from MSM8974 common
include device/oppo/msm8974-common/BoardConfigCommon.mk

# Init
TARGET_INIT_VENDOR_LIB := libinit_find7
TARGET_NEEDS_PRE_INIT := true

# Filesystem
BOARD_BOOTIMAGE_PARTITION_SIZE     := 16777216
BOARD_CACHEIMAGE_PARTITION_SIZE    := 536870912
BOARD_PERSISTIMAGE_PARTITION_SIZE  := 33554432
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 16777216
BOARD_SYSTEMIMAGE_PARTITION_SIZE   := 1364320256
BOARD_USERDATAIMAGE_PARTITION_SIZE := 3221225472
TARGET_USERIMAGES_USE_F2FS := true

# Include path
TARGET_SPECIFIC_HEADER_PATH += device/oppo/find7/include

# Kernel
BOARD_KERNEL_CMDLINE := console=ttyHSL0,115200,n8 androidboot.hardware=qcom user_debug=31 msm_rtb.filter=0x37 ehci-hcd.park=3 androidboot.bootdevice=msm_sdcc.1 zcache.enabled=1 androidboot.selinux=permissive
TARGET_KERNEL_CONFIG := custom_find7_defconfig
TARGET_KERNEL_CROSS_COMPILE_PREFIX := arm-linux-androideabi-

# Audio
AUDIO_FEATURE_LOW_LATENCY_PRIMARY := true

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := device/oppo/find7/bluetooth

# Camera
USE_DEVICE_SPECIFIC_CAMERA := true
COMMON_GLOBAL_CFLAGS += -DOPPO_CAMERA_HARDWARE

# Power
TARGET_POWERHAL_VARIANT := find7

# Setupwizard
BOARD_SETUP_WIZARD_CLASS     := device/oppo/find7/setupwizard/src
BOARD_SETUP_WIZARD_RESOURCES += device/oppo/find7/setupwizard/res

### RECOVERY START
RECOVERY_VARIANT := twrp

# Assert
TARGET_OTA_ASSERT_DEVICE := FIND7,find7,find7u,find7a,find7au,find7s,find7su

# Dummy
TARGET_RECOVERY_FSTAB := device/oppo/find7/rootdir/etc/fstab.qcom

# TWRP specific build flags
TW_THEME := portrait_hdpi
RECOVERY_GRAPHICS_USE_LINELENGTH := true
TW_NO_USB_STORAGE := true
TW_INCLUDE_JB_CRYPTO := false
TW_NO_SCREEN_BLANK := true
TW_EXCLUDE_ENCRYPTED_BACKUPS := true
TW_INCLUDE_L_CRYPTO := true
TARGET_RECOVERY_PIXEL_FORMAT := "RGBX_8888"
### RECOVERY END

# SELinux policies
BOARD_SEPOLICY_DIRS += device/oppo/find7/sepolicy

RED_LED_PATH := "/sys/class/leds/led:rgb_red/brightness"
GREEN_LED_PATH := "/sys/class/leds/led:rgb_green/brightness"
BLUE_LED_PATH := "/sys/class/leds/led:rgb_blue/brightness"

# inherit from the proprietary version
-include vendor/oppo/find7/BoardConfigVendor.mk
-include vendor/oppo/find7-common/BoardConfigVendor.mk
