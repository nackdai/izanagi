call PreBuild.bat

emcc -DEMSCRIPTEN -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__ -I../../include ../../source/system/web/SysWindow_web.cpp ../../source/system/TouchInput.cpp ../../source/system/linux/FileStream_linux.cpp ../../source/system/linux/SysEnvironment_linux.cpp ../../source/system/linux/SysTimer_linux.cpp ../../source/system/linux/SysUtil_linux.cpp ../../source/system/linux/device/Pad_linux.cpp -o ../lib/libSystem.bc