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

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Get the prebuilt list of APNs
$(call inherit-product, vendor/nameless/config/apns.mk)

# Inherit some common Nameless stuff
$(call inherit-product, vendor/nameless/config/common.mk)

# Enhanced NFC
$(call inherit-product, vendor/nameless/config/nfc_enhanced.mk)

# overlays
DEVICE_PACKAGE_OVERLAYS += $(LOCAL_PATH)/overlay

# Bootanimation and recovery
TARGET_SCREEN_HEIGHT := 1920
TARGET_SCREEN_WIDTH  := 1080

# Init
PRODUCT_PACKAGES += \
    libinit_find7 \

# Ramdisk
PRODUCT_PACKAGES += \
    fstab.qcom.lvm \
    fstab.qcom.std \
    fstab.qcom.ufd \
    lvm \
    lvm.conf \
    init.qcom.rc \
    init.qcom.usb.rc \

# Recovery
PRODUCT_PACKAGES += \
    init.recovery.target.rc \
    twrp.fstab.lvm \
    twrp.fstab.std \
    twrp.fstab.ufd \

# Audio
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/audio/audio_platform_info.xml:system/etc/audio_platform_info.xml \
    $(LOCAL_PATH)/audio/audio_policy.conf:system/etc/audio_policy.conf \
    $(LOCAL_PATH)/audio/mixer_paths.xml:system/etc/mixer_paths.xml

# Camera
PRODUCT_PACKAGES += \
    camera.msm8974

# Device extra
PRODUCT_PACKAGES += \
    DeviceExtra \

# NFC packages
PRODUCT_PACKAGES += \
    com.android.nfc_extras \
    nfc.msm8974 \
    Nfc \
    Tag \

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/com.android.nfc_extras.xml:system/etc/permissions/com.android.nfc_extras.xml \
    frameworks/native/data/etc/android.hardware.nfc.xml:system/etc/permissions/android.hardware.nfc.xml

# Power
PRODUCT_PACKAGES += \
    power.msm8974

# Reduce client buffer size for fast audio output tracks
PRODUCT_PROPERTY_OVERRIDES += \
    af.fast_track_multiplier=1

# Low latency audio buffer size in frames
PRODUCT_PROPERTY_OVERRIDES += \
    audio_hal.period_size=192

PRODUCT_NAME := nameless_find7
PRODUCT_DEVICE := find7
PRODUCT_BRAND := OPPO
PRODUCT_MANUFACTURER := OPPO
PRODUCT_MODEL := Find7

PRODUCT_GMS_CLIENTID_BASE := android-oppo

# Device uses high-density artwork where available
# Also hack it to be compatible with Find7a and Find7s
PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := 530dpi

# Build description
PRODUCT_BUILD_PROP_OVERRIDES += \
    BUILD_FINGERPRINT=5.1.1/LMY47V/1441693976:user/release-keys \
    PRIVATE_BUILD_DESC="msm8974-user 5.1.1 LMY47V 153 release-keys" \
    TARGET_DEVICE="FIND7" \

# NOTE: dalvik heap and hwui memory ARE set in init depending on device
# call dalvik heap config
#$(call inherit-product-if-exists, frameworks/native/build/phone-xxhdpi-2048-dalvik-heap.mk)
# call hwui memory config
#$(call inherit-product-if-exists, frameworks/native/build/phone-xxhdpi-2048-hwui-memory.mk)

# call the proprietary setup
$(call inherit-product-if-exists, vendor/oppo/find7/find7-vendor.mk)
$(call inherit-product-if-exists, vendor/oppo/find7-common/find7-common-vendor.mk)

# Inherit from msm8974-common
$(call inherit-product, device/oppo/msm8974-common/msm8974.mk)

# Include extras
$(call inherit-product-if-exists, vendor/extra/find7/extra.mk)
