LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libAnimation
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/animation/EasingInterpolator.cpp \
../../source/animation/Interpolator.cpp \
../../source/animation/LinearInterpolator.cpp \
../../source/animation/SplineInterpolator.cpp \
../../source/animation/StoryBoard.cpp \
../../source/animation/Timeline.cpp \
../../source/animation/Tweener.cpp \
../../source/animation/TweenerUtility.cpp

include $(BUILD_STATIC_LIBRARY)
