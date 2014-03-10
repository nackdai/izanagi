LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libSceneGraph
LOCAL_LDLIBS     := -llog -lGLESv2
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/scenegraph/Camera.cpp \
../../source/scenegraph/Decal.cpp \
../../source/scenegraph/SceneGraphUtil.cpp \
../../source/scenegraph/SceneParam.cpp \
../../source/scenegraph/SceneState.cpp \
../../source/scenegraph/VectorCamera.cpp \
../../source/scenegraph/img/izImage.cpp \
../../source/scenegraph/mdl/izModel.cpp \
../../source/scenegraph/anm/AnimationBinder.cpp \
../../source/scenegraph/anm/AnimationInterface.cpp \
../../source/scenegraph/anm/AnimationInterp.cpp \
../../source/scenegraph/anm/AnimationUtil.cpp \
../../source/scenegraph/anm/izAnimation.cpp \
../../source/scenegraph/anm/PoseUpdater.cpp \
../../source/scenegraph/anm/AnimationLinearBlender.cpp \
../../source/scenegraph/mtrl/izMaterial.cpp \
../../source/scenegraph/msh/izMesh.cpp \
../../source/scenegraph/msh/MeshInstance.cpp \
../../source/scenegraph/msh/MeshSetInstance.cpp \
../../source/scenegraph/msh/MshMeshGroup.cpp \
../../source/scenegraph/msh/MshMeshSet.cpp \
../../source/scenegraph/msh/MshPrimitiveSet.cpp \
../../source/scenegraph/skl/izSkeleton.cpp \
../../source/scenegraph/skl/SkeletonInstance.cpp \
../../source/scenegraph/skl/SkeletonUtil.cpp \
../../source/scenegraph/rendergraph/SceneRenderer.cpp \
../../source/scenegraph/rendergraph/RenderGraph.cpp

include $(BUILD_STATIC_LIBRARY)
