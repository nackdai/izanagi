LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libDebugUtil
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/debugutil/DebugFont.cpp \
../../source/debugutil/mesh/DebugMesh.cpp \
../../source/debugutil/mesh/DebugMeshAxis.cpp \
../../source/debugutil/mesh/DebugMeshBox.cpp \
../../source/debugutil/mesh/DebugMeshCylinder.cpp \
../../source/debugutil/mesh/DebugMeshFrustum.cpp \
../../source/debugutil/mesh/DebugMeshGrid.cpp \
../../source/debugutil/mesh/DebugMeshRectangle.cpp \
../../source/debugutil/mesh/DebugMeshSphere.cpp \
../../source/debugutil/mesh/DebugMeshTorus.cpp

include $(BUILD_STATIC_LIBRARY)
