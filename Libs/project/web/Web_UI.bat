call PreBuild.bat

emcc -DEMSCRIPTEN -D__IZ_GLES2__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__ -I../../include ../../source/ui/GestureDetector.cpp ../../source/ui/VelocityTracker.cpp -o ../lib/libUI.bc