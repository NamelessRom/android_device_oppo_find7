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

# overlays
DEVICE_PACKAGE_OVERLAYS += $(LOCAL_PATH)/overlay

# Init
PRODUCT_PACKAGES += \
    libinit_find7 \

# Ramdisk
PRODUCT_PACKAGES += \
    fstab.qcom.std \
    fstab.qcom.ufd \
    init.fs.rc.std \
    init.fs.rc.ufd \
    init.qcom.rc \
    init.qcom.usb.rc \
    storage_earlyinit.sh \
    storage_init.sh

# Audio
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/audio/audio_policy.conf:system/etc/audio_policy.conf \
    $(LOCAL_PATH)/audio/mixer_paths.xml:system/etc/mixer_paths.xml

# Camera
PRODUCT_PACKAGES += \
    camera.msm8974

# NFC packages
PRODUCT_PACKAGES += \
    nfc.msm8974 \
    libnfc \
    libnfc_jni \
    Nfc \
    Tag \
    com.android.nfc_extras

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/base/nfc-extras/com.android.nfc_extras.xml:system/etc/permissions/com.android.nfc_extras.xml \
    frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml

# Build description
PRODUCT_BUILD_PROP_OVERRIDES += BUILD_FINGERPRINT=4.4.2/KVT49L/1390465867:user/release-keys PRIVATE_BUILD_DESC="msm8974-user 4.4.2 KVT49L eng.root.20141017.144947 release-keys"

PRODUCT_TAGS += dalvik.gc.type-precise

# call the proprietary setup
$(call inherit-product-if-exists, vendor/oppo/find7/find7-vendor.mk)

# Inherit from msm8974-common
$(call inherit-product, device/oppo/msm8974-common/msm8974.mk)
