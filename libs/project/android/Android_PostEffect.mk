LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libPostEffect
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/posteffect/PostEffect.cpp \
../../source/posteffect/PostEffectShader.cpp \
../../source/posteffect/PostEffectShaderPass.cpp \
../../source/posteffect/PostEffectSystem.cpp \
../../source/posteffect/PostEffectTextureCreator.cpp \
../../source/posteffect/functor/PostEffectFunctor.cpp \
../../source/posteffect/functor/PostEffectFunctorDefault.cpp \
../../source/posteffect/functor/PostEffectFunctorDOF.cpp \
../../source/posteffect/functor/PostEffectFunctorMGF.cpp \
../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp \
../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp \
../../source/posteffect/util/PostEffectShaderUtil.cpp \
../../source/posteffect/util/PostEffectUtilityRect.cpp \
../../source/posteffect/vs/PostEffectVS.cpp \
../../source/posteffect/vs/PostEffectVSManager.cpp

include $(BUILD_STATIC_LIBRARY)
