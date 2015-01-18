call PreBuild.bat

emcc -DEMSCRIPTEN -D__IZ_GLUT__ -D_DEBUG -D_LIB -D__IZ_DEBUG__ -D__IZ_OGL__ -D__IZANAGI_NO_USE_D3D__ -I../../include ../../source/resource/Resource.cpp ../../source/resource/ResourceManager.cpp ../../source/resource/archive/Archive.cpp ../../source/resource/archive/ArchiveTarball.cpp -o ../lib/libResource.bc