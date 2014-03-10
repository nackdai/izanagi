LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libMesssageQueue
LOCAL_LDLIBS     := -llog -lGLESv2 -lThreadModel -lSceneGraph -lSystem -lDebugUtil -lGraph -lStd -lpthread -lSampleKit -lMath
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D__IZANAGI_NO_USE_D3D__ -D__IZ_DEBUG__ -D__IZ_OGL__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Libs/include $(LOCAL_PATH)/../../SampleKit
LOCAL_SRC_FILES  := ../../MessageQueue/main.cpp

include $(BUILD_SHARED_LIBRARY)
