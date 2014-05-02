LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libShader
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/shader/ShaderManager.cpp \
../../source/shader/ShaderUtil.cpp \
../../source/shader/base/ShaderPass.cpp \
../../source/shader/base/ShaderBasic.cpp \
../../source/shader/base/table/ShaderAttrTable.cpp \
../../source/shader/base/table/ShaderParameterTable.cpp \
../../source/shader/base/table/ShaderPassTable.cpp \
../../source/shader/base/table/ShaderSamplerTable.cpp

include $(BUILD_STATIC_LIBRARY)
