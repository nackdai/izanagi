LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libSampleKit
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Libs/include $(LOCAL_PATH)/../../SampleKit
LOCAL_SRC_FILES  := ../../SampleKit/SampleApp.cpp \
../../SampleKit/SampleCamera.cpp \
../../SampleKit/SampleEnvBox.cpp \
../../SampleKit/SampleMain.cpp \
../../SampleKit/SampleModelRenderHandler.cpp \
../../SampleKit/SampleWindowProc.cpp

include $(BUILD_STATIC_LIBRARY)
