#
# Copyright (C) 2014 The CyanogenMod Project
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
-include device/oppo/msm8974-common/BoardConfigCommon.mk

# Kernel
TARGET_KERNEL_CONFIG := custom_find7_defconfig
BOARD_KERNEL_CMDLINE := console=ttyHSL0,115200,n8 androidboot.hardware=qcom user_debug=31 msm_rtb.filter=0x3F ehci-hcd.park=3
BOARD_CUSTOM_BOOTIMG_MK := device/oppo/find7/mkbootimg.mk

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := device/oppo/find7/bluetooth

# Camera
USE_DEVICE_SPECIFIC_CAMERA := true
COMMON_GLOBAL_CFLAGS += -DOPPO_CAMERA_HARDWARE

# Filesystem
BOARD_BOOTIMAGE_PARTITION_SIZE     := 16777216
BOARD_CACHEIMAGE_PARTITION_SIZE    := 536870912
BOARD_PERSISTIMAGE_PARTITION_SIZE  := 33554432
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 16777216
BOARD_SYSTEMIMAGE_PARTITION_SIZE   := 1364320256
BOARD_USERDATAIMAGE_PARTITION_SIZE := 13271448576
#BOARD_USERDATAIMAGE_PARTITION_SIZE := 3221225472

# Init
TARGET_INIT_VENDOR_LIB := libinit_find7

# Workaround for factory issue
BOARD_VOLD_CRYPTFS_MIGRATE := true

# Recovery
TARGET_RECOVERY_FSTAB := device/oppo/find7/rootdir/etc/fstab.qcom
RECOVERY_VARIANT      := philz

# Assert
TARGET_OTA_ASSERT_DEVICE := find7,find7a,X9007,X9006,FIND7

# Releasetools
TARGET_RELEASETOOLS_EXTENSIONS := device/oppo/find7

# inherit from the proprietary version
-include vendor/oppo/find7/BoardConfigVendor.mk
