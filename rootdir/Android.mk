LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# Device init files

ifeq ($(filter $(TARGET_DEVICE),find7au find7u),)
  # Non unified
  include $(CLEAR_VARS)
  LOCAL_MODULE       := fstab.qcom
  LOCAL_MODULE_TAGS  := optional eng
  LOCAL_MODULE_CLASS := ETC
  LOCAL_SRC_FILES    := etc/fstab.qcom
  LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
  include $(BUILD_PREBUILT)

  include $(CLEAR_VARS)
  LOCAL_MODULE       := init.qcom.rc
  LOCAL_MODULE_TAGS  := optional eng
  LOCAL_MODULE_CLASS := ETC
  LOCAL_SRC_FILES    := etc/init.qcom.rc
  LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
  include $(BUILD_PREBUILT)

  include $(CLEAR_VARS)
  LOCAL_MODULE       := init.qcom.usb.rc
  LOCAL_MODULE_TAGS  := optional eng
  LOCAL_MODULE_CLASS := ETC
  LOCAL_SRC_FILES    := etc/init.qcom.usb.rc
  LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
  include $(BUILD_PREBUILT)
else
  # Unified
  include $(CLEAR_VARS)
  LOCAL_MODULE       := fstab.qcom
  LOCAL_MODULE_TAGS  := optional eng
  LOCAL_MODULE_CLASS := ETC
  LOCAL_SRC_FILES    := etc/unified/fstab.qcom
  LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
  include $(BUILD_PREBUILT)

  include $(CLEAR_VARS)
  LOCAL_MODULE       := init.qcom.rc
  LOCAL_MODULE_TAGS  := optional eng
  LOCAL_MODULE_CLASS := ETC
  LOCAL_SRC_FILES    := etc/unified/init.qcom.rc
  LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
  include $(BUILD_PREBUILT)

  include $(CLEAR_VARS)
  LOCAL_MODULE       := init.qcom.usb.rc
  LOCAL_MODULE_TAGS  := optional eng
  LOCAL_MODULE_CLASS := ETC
  LOCAL_SRC_FILES    := etc/unified/init.qcom.usb.rc
  LOCAL_MODULE_PATH  := $(TARGET_ROOT_OUT)
  include $(BUILD_PREBUILT)
endif
