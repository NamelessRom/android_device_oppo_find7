LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES := system/core/init
LOCAL_CFLAGS := -Wall -Wno-writable-strings
LOCAL_CPPFLAGS := -Wall -Wno-writable-strings
LOCAL_SRC_FILES := init_find7.cpp
LOCAL_MODULE := libinit_find7

include $(BUILD_STATIC_LIBRARY)
