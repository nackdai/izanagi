LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libText
LOCAL_LDLIBS     := -llog -lGLESv2
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../External/freetype/include $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/text/FontGlyphCache.cpp \
../../source/text/FontGlyphCacheImpl.cpp \
../../source/text/FontMap.cpp \
../../source/text/FontRenderer.cpp \
../../source/text/UString.cpp \
../../source/text/FontHostFNT.cpp \
../../source/text/FontHostFT.cpp \
../../source/text/Paragraph.cpp \
../../source/text/ParagraphGroup.cpp \
../../source/text/TextLine.cpp \
../../source/text/ParagraphGroupImpl.cpp \
../../source/text/ParagraphImpl.cpp \
../../source/text/TextLineImpl.cpp

include $(BUILD_STATIC_LIBRARY)
