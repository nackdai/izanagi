if exist ../lib/SampleKit.bc (
    call PreBuild.bat
) else (
    call Web_SampleKit.bat
)

emcc -DEMSCRIPTEN -D__IZ_GLUT__ -D__IZANAGI_NO_USE_D3D__ -D_DEBUG -D__IZ_DEBUG__ -D__IZ_OGL__  -I../../Animation/MyAppl  -I../../../Libs/include  -I../../SampleKit %LIB%libSceneGraph.bc %LIB%libShader.bc %LIB%libMath.bc %LIB%libSystem.bc %LIB%libDebugUtil.bc %LIB%libGraph.bc %LIB%libStd.bc ../lib/libSampleKit.bc ../../Decal/Cube.cpp ../../Decal/DecalApp.cpp ../../Decal/main.cpp ../../Decal/Sphere.cpp -o Decal.html