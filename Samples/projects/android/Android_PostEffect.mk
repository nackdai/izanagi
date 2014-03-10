LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libPostEffect
LOCAL_LDLIBS     := -llog -lGLESv2 -lShader -lPostEffect -lMath -lSceneGraph -lSystem -lDebugUtil -lGraph -lStd -lSampleKit
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D__IZANAGI_NO_USE_D3D__ -D__IZ_DEBUG__ -D__IZ_OGL__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Libs/include $(LOCAL_PATH)/../../SampleKit
LOCAL_SRC_FILES  := ../../PostEffect/main.cpp \
../../PostEffect/PostEffectApp.cpp \
../../PostEffect/PostEffectSample.cpp \
../../PostEffect/SceneRenderer.cpp \
../../PostEffect/StateBase.cpp \
../../PostEffect/StateBloomStar.cpp \
../../PostEffect/StateBloomStarMGF.cpp \
../../PostEffect/StateHDR.cpp \
../../PostEffect/StateHDR2.cpp \
../../PostEffect/StateManager.cpp \
../../PostEffect/StateMonotone.cpp \
../../PostEffect/StateNegative.cpp \
../../PostEffect/StatePostEffect.cpp \
../../PostEffect/StateSepia.cpp

include $(BUILD_SHARED_LIBRARY)
