LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libThreadModel
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/threadmodel/Parallel.cpp \
../../source/threadmodel/ThreadModelTask.cpp \
../../source/threadmodel/ThreadPool.cpp \
../../source/threadmodel/ThreadModelJob.cpp \
../../source/threadmodel/ThreadModelJobQueue.cpp \
../../source/threadmodel/ThreadModelJobWorker.cpp \
../../source/threadmodel/ThreadModelMessageQueue.cpp

include $(BUILD_STATIC_LIBRARY)
