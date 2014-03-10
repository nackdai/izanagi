LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libMakefile
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := 
include $(BUILD_STATIC_LIBRARY)
