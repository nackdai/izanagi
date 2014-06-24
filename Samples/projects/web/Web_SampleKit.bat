call PreBuild.bat

emcc -DEMSCRIPTEN -D__IZ_GLUT__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__ -I../../../Libs/include -I../../SampleKit ../../SampleKit/SampleApp.cpp ../../SampleKit/SampleCamera.cpp ../../SampleKit/SampleEnvBox.cpp ../../SampleKit/SampleMain.cpp ../../SampleKit/SampleModelRenderHandler.cpp ../../SampleKit/SampleWindowProc.cpp -o ../lib/libSampleKit.bc