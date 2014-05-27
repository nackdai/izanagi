LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libMath
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/math/MathPlane.cpp \
../../source/math/MathRay.cpp \
../../source/math/MathRectangle.cpp \
../../source/math/MathTriangle.cpp \
../../source/math/MathMatrix.cpp \
../../source/math/SFMT/SFMT.cpp \
../../source/math/MathNumericalAnlysis.cpp \
../../source/math/MathCurve.cpp

include $(BUILD_STATIC_LIBRARY)
