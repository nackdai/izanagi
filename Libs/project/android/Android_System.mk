LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libSystem
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/system/android/SysWindow_android.cpp \
../../source/system/TouchInput.cpp \
../../source/system/android/FileStream_android.cpp \
../../source/system/linux/SysEnvironment_linux.cpp \
../../source/system/linux/SysTimer_linux.cpp \
../../source/system/linux/SysUtil_linux.cpp \
../../source/system/linux/device/Pad_linux.cpp \
../../source/system/linux/thread/SysEvent_pthread.cpp \
../../source/system/linux/thread/SysMutex_pthread.cpp \
../../source/system/linux/thread/SysSemaphore_pthread.cpp \
../../source/system/linux/thread/SysThread_pthread.cpp

include $(BUILD_STATIC_LIBRARY)
