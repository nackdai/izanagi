LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libGraph
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/graph/BufferBase.cpp \
../../source/graph/GraphicsDevice.cpp \
../../source/graph/RenderState.cpp \
../../source/graph/ShaderProgram.cpp \
../../source/graph/2d/2DShader.cpp \
../../source/graph/2d/2DRenderer.cpp \
../../source/graph/gles2/2DShader_GLES2.cpp \
../../source/graph/gles2/CubeTexture_GLES2.cpp \
../../source/graph/gles2/FrameBufferObject.cpp \
../../source/graph/gles2/GraphicsDeviceResourceFuncs.cpp \
../../source/graph/gles2/GraphicsDevice_GLES2.cpp \
../../source/graph/gles2/GraphPerf_GLES2.cpp \
../../source/graph/gles2/IndexBuffer_GLES2.cpp \
../../source/graph/gles2/ParamValueConverter_GLES2.cpp \
../../source/graph/gles2/PixelShader_GLES2.cpp \
../../source/graph/gles2/RenderState_GLES2.cpp \
../../source/graph/gles2/RenderTarget_GLES2.cpp \
../../source/graph/gles2/ShaderProgram_GLES2.cpp \
../../source/graph/gles2/Texture_GLES2.cpp \
../../source/graph/gles2/VertexBuffer_GLES2.cpp \
../../source/graph/gles2/VertexDeclaration_GLES2.cpp \
../../source/graph/gles2/VertexShader_GLES2.cpp \

include $(BUILD_STATIC_LIBRARY)
