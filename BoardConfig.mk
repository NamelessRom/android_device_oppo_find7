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

# Inherit from Find7 common
-include device/oppo/find7-common/BoardConfigCommon.mk

# Filesystem
BOARD_BOOTIMAGE_PARTITION_SIZE     := 16777216
BOARD_CACHEIMAGE_PARTITION_SIZE    := 536870912
BOARD_PERSISTIMAGE_PARTITION_SIZE  := 33554432
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 16777216
BOARD_SYSTEMIMAGE_PARTITION_SIZE   := 1364320256
BOARD_USERDATAIMAGE_PARTITION_SIZE := 3221225472

TARGET_RECOVERY_FSTAB := device/oppo/find7/rootdir/etc/fstab.qcom
RECOVERY_VARIANT      := philz

# Assert
TARGET_OTA_ASSERT_DEVICE := FIND7,find7,find7s

# inherit from the proprietary version
-include vendor/oppo/find7/BoardConfigVendor.mk
