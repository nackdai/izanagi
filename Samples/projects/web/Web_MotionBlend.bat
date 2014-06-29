if exist ../lib/SampleKit.bc (
    call PreBuild.bat
) else (
    call Web_SampleKit.bat
)

if exist ../../MotionBlend/pre.js (
    set PREJS=--pre-js ../../Render2D/pre.js
) else (
    set PREJS=
)

emcc -DEMSCRIPTEN -D__IZ_GLUT__ -D_DEBUG -D__IZANAGI_NO_USE_D3D__ -D__IZ_DEBUG__ -D__IZ_OGL__  -I../../../Libs/include  -I../../SampleKit %LIB%libShader.bc %LIB%libAnimation.bc %LIB%libMath.bc %LIB%libSceneGraph.bc %LIB%libSystem.bc %LIB%libDebugUtil.bc %LIB%libGraph.bc %LIB%libStd.bc ../lib/libSampleKit.bc ../../MotionBlend/CustomInterp.cpp ../../MotionBlend/main.cpp ../../MotionBlend/MotionBlendApp.cpp -o MotionBlend.html %PREJS%