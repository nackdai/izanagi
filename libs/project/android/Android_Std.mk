LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE     := libStd
LOCAL_CFLAGS     := -DANDROID -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../include
LOCAL_SRC_FILES  := ../../source/std/StdColor.cpp \
../../source/std/StdKey.cpp \
../../source/std/StdLog.cpp \
../../source/std/StdProfiler.cpp \
../../source/std/StdUtf.cpp \
../../source/std/allocator/ChunkedMemoryAllocator.cpp \
../../source/std/allocator/SimpleMemoryAllocator.cpp \
../../source/std/allocator/StandardMemoryAllocator.cpp \
../../source/std/allocator/STLMemoryAllocator.cpp \
../../source/std/binding/Binding.cpp \
../../source/std/binding/BindingExpression.cpp \
../../source/std/binding/BindingOperations.cpp \
../../source/std/binding/DependencyObject.cpp \
../../source/std/binding/DependencyPropertyChangedEventArgs.cpp

include $(BUILD_STATIC_LIBRARY)
