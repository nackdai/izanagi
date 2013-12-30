# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLES2_Include_Path=-I"../../include" 
Release_GLES2_Include_Path=-I"../../include" 

# Library paths...
Debug_GLES2_Library_Path=
Release_GLES2_Library_Path=

# Additional libraries...
Debug_GLES2_Libraries=
Release_GLES2_Libraries=

# Preprocessor definitions...
Debug_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _LIB -D __IZ_DEBUG__ -D __IZ_GLES2__ -D __IZANAGI_NO_USE_D3D__ 
Release_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _LIB -D __IZ_GLES2__ -D __IZANAGI_NO_USE_D3D__ 

# Implictly linked object files...
Debug_GLES2_Implicitly_Linked_Objects=
Release_GLES2_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLES2_Compiler_Flags=-O0 
Release_GLES2_Compiler_Flags=-O2 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLES2 Release_GLES2 

# Builds the Debug_GLES2 configuration...
.PHONY: Debug_GLES2
Debug_GLES2: create_folders gccDebug_GLES2/source/posteffect/PostEffect.o gccDebug_GLES2/source/posteffect/PostEffectShader.o gccDebug_GLES2/source/posteffect/PostEffectShaderPass.o gccDebug_GLES2/source/posteffect/PostEffectSystem.o gccDebug_GLES2/source/posteffect/PostEffectTextureCreator.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctor.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDefault.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDOF.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorMGF.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.o gccDebug_GLES2/source/posteffect/util/PostEffectShaderUtil.o gccDebug_GLES2/source/posteffect/util/PostEffectUtilityRect.o gccDebug_GLES2/source/posteffect/vs/PostEffectVS.o gccDebug_GLES2/source/posteffect/vs/PostEffectVSManager.o 
	ar rcs ../lib/gccDebug_GLES2/libPostEffect.a gccDebug_GLES2/source/posteffect/PostEffect.o gccDebug_GLES2/source/posteffect/PostEffectShader.o gccDebug_GLES2/source/posteffect/PostEffectShaderPass.o gccDebug_GLES2/source/posteffect/PostEffectSystem.o gccDebug_GLES2/source/posteffect/PostEffectTextureCreator.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctor.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDefault.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDOF.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorMGF.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.o gccDebug_GLES2/source/posteffect/util/PostEffectShaderUtil.o gccDebug_GLES2/source/posteffect/util/PostEffectUtilityRect.o gccDebug_GLES2/source/posteffect/vs/PostEffectVS.o gccDebug_GLES2/source/posteffect/vs/PostEffectVSManager.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/posteffect/PostEffect.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/PostEffect.d
gccDebug_GLES2/source/posteffect/PostEffect.o: ../../source/posteffect/PostEffect.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffect.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/PostEffect.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffect.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/PostEffect.d

# Compiles file ../../source/posteffect/PostEffectShader.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/PostEffectShader.d
gccDebug_GLES2/source/posteffect/PostEffectShader.o: ../../source/posteffect/PostEffectShader.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffectShader.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/PostEffectShader.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffectShader.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/PostEffectShader.d

# Compiles file ../../source/posteffect/PostEffectShaderPass.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/PostEffectShaderPass.d
gccDebug_GLES2/source/posteffect/PostEffectShaderPass.o: ../../source/posteffect/PostEffectShaderPass.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffectShaderPass.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/PostEffectShaderPass.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffectShaderPass.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/PostEffectShaderPass.d

# Compiles file ../../source/posteffect/PostEffectSystem.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/PostEffectSystem.d
gccDebug_GLES2/source/posteffect/PostEffectSystem.o: ../../source/posteffect/PostEffectSystem.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffectSystem.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/PostEffectSystem.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffectSystem.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/PostEffectSystem.d

# Compiles file ../../source/posteffect/PostEffectTextureCreator.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/PostEffectTextureCreator.d
gccDebug_GLES2/source/posteffect/PostEffectTextureCreator.o: ../../source/posteffect/PostEffectTextureCreator.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffectTextureCreator.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/PostEffectTextureCreator.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffectTextureCreator.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/PostEffectTextureCreator.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctor.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/functor/PostEffectFunctor.d
gccDebug_GLES2/source/posteffect/functor/PostEffectFunctor.o: ../../source/posteffect/functor/PostEffectFunctor.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctor.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctor.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctor.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/functor/PostEffectFunctor.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorDefault.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDefault.d
gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDefault.o: ../../source/posteffect/functor/PostEffectFunctorDefault.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorDefault.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDefault.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorDefault.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDefault.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorDOF.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDOF.d
gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDOF.o: ../../source/posteffect/functor/PostEffectFunctorDOF.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorDOF.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDOF.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorDOF.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorDOF.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorMGF.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorMGF.d
gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorMGF.o: ../../source/posteffect/functor/PostEffectFunctorMGF.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorMGF.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorMGF.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorMGF.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorMGF.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.d
gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.o: ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.d
gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.o: ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.d

# Compiles file ../../source/posteffect/util/PostEffectShaderUtil.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/util/PostEffectShaderUtil.d
gccDebug_GLES2/source/posteffect/util/PostEffectShaderUtil.o: ../../source/posteffect/util/PostEffectShaderUtil.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/util/PostEffectShaderUtil.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/util/PostEffectShaderUtil.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/util/PostEffectShaderUtil.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/util/PostEffectShaderUtil.d

# Compiles file ../../source/posteffect/util/PostEffectUtilityRect.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/util/PostEffectUtilityRect.d
gccDebug_GLES2/source/posteffect/util/PostEffectUtilityRect.o: ../../source/posteffect/util/PostEffectUtilityRect.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/util/PostEffectUtilityRect.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/util/PostEffectUtilityRect.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/util/PostEffectUtilityRect.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/util/PostEffectUtilityRect.d

# Compiles file ../../source/posteffect/vs/PostEffectVS.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/vs/PostEffectVS.d
gccDebug_GLES2/source/posteffect/vs/PostEffectVS.o: ../../source/posteffect/vs/PostEffectVS.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/vs/PostEffectVS.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/vs/PostEffectVS.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/vs/PostEffectVS.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/vs/PostEffectVS.d

# Compiles file ../../source/posteffect/vs/PostEffectVSManager.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/posteffect/vs/PostEffectVSManager.d
gccDebug_GLES2/source/posteffect/vs/PostEffectVSManager.o: ../../source/posteffect/vs/PostEffectVSManager.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/posteffect/vs/PostEffectVSManager.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/posteffect/vs/PostEffectVSManager.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/posteffect/vs/PostEffectVSManager.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/posteffect/vs/PostEffectVSManager.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/source/posteffect/PostEffect.o gccRelease_GLES2/source/posteffect/PostEffectShader.o gccRelease_GLES2/source/posteffect/PostEffectShaderPass.o gccRelease_GLES2/source/posteffect/PostEffectSystem.o gccRelease_GLES2/source/posteffect/PostEffectTextureCreator.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctor.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDefault.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDOF.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorMGF.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.o gccRelease_GLES2/source/posteffect/util/PostEffectShaderUtil.o gccRelease_GLES2/source/posteffect/util/PostEffectUtilityRect.o gccRelease_GLES2/source/posteffect/vs/PostEffectVS.o gccRelease_GLES2/source/posteffect/vs/PostEffectVSManager.o 
	ar rcs ../lib/gccRelease_GLES2/libPostEffect.a gccRelease_GLES2/source/posteffect/PostEffect.o gccRelease_GLES2/source/posteffect/PostEffectShader.o gccRelease_GLES2/source/posteffect/PostEffectShaderPass.o gccRelease_GLES2/source/posteffect/PostEffectSystem.o gccRelease_GLES2/source/posteffect/PostEffectTextureCreator.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctor.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDefault.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDOF.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorMGF.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.o gccRelease_GLES2/source/posteffect/util/PostEffectShaderUtil.o gccRelease_GLES2/source/posteffect/util/PostEffectUtilityRect.o gccRelease_GLES2/source/posteffect/vs/PostEffectVS.o gccRelease_GLES2/source/posteffect/vs/PostEffectVSManager.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/posteffect/PostEffect.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/PostEffect.d
gccRelease_GLES2/source/posteffect/PostEffect.o: ../../source/posteffect/PostEffect.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffect.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/PostEffect.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffect.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/PostEffect.d

# Compiles file ../../source/posteffect/PostEffectShader.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/PostEffectShader.d
gccRelease_GLES2/source/posteffect/PostEffectShader.o: ../../source/posteffect/PostEffectShader.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffectShader.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/PostEffectShader.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffectShader.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/PostEffectShader.d

# Compiles file ../../source/posteffect/PostEffectShaderPass.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/PostEffectShaderPass.d
gccRelease_GLES2/source/posteffect/PostEffectShaderPass.o: ../../source/posteffect/PostEffectShaderPass.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffectShaderPass.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/PostEffectShaderPass.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffectShaderPass.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/PostEffectShaderPass.d

# Compiles file ../../source/posteffect/PostEffectSystem.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/PostEffectSystem.d
gccRelease_GLES2/source/posteffect/PostEffectSystem.o: ../../source/posteffect/PostEffectSystem.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffectSystem.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/PostEffectSystem.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffectSystem.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/PostEffectSystem.d

# Compiles file ../../source/posteffect/PostEffectTextureCreator.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/PostEffectTextureCreator.d
gccRelease_GLES2/source/posteffect/PostEffectTextureCreator.o: ../../source/posteffect/PostEffectTextureCreator.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/PostEffectTextureCreator.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/PostEffectTextureCreator.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/PostEffectTextureCreator.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/PostEffectTextureCreator.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctor.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/functor/PostEffectFunctor.d
gccRelease_GLES2/source/posteffect/functor/PostEffectFunctor.o: ../../source/posteffect/functor/PostEffectFunctor.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctor.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctor.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctor.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/functor/PostEffectFunctor.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorDefault.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDefault.d
gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDefault.o: ../../source/posteffect/functor/PostEffectFunctorDefault.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorDefault.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDefault.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorDefault.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDefault.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorDOF.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDOF.d
gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDOF.o: ../../source/posteffect/functor/PostEffectFunctorDOF.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorDOF.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDOF.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorDOF.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorDOF.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorMGF.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorMGF.d
gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorMGF.o: ../../source/posteffect/functor/PostEffectFunctorMGF.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorMGF.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorMGF.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorMGF.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorMGF.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.d
gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.o: ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorRenderStar.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.d
gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.o: ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/functor/PostEffectFunctorSwitchTex.d

# Compiles file ../../source/posteffect/util/PostEffectShaderUtil.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/util/PostEffectShaderUtil.d
gccRelease_GLES2/source/posteffect/util/PostEffectShaderUtil.o: ../../source/posteffect/util/PostEffectShaderUtil.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/util/PostEffectShaderUtil.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/util/PostEffectShaderUtil.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/util/PostEffectShaderUtil.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/util/PostEffectShaderUtil.d

# Compiles file ../../source/posteffect/util/PostEffectUtilityRect.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/util/PostEffectUtilityRect.d
gccRelease_GLES2/source/posteffect/util/PostEffectUtilityRect.o: ../../source/posteffect/util/PostEffectUtilityRect.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/util/PostEffectUtilityRect.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/util/PostEffectUtilityRect.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/util/PostEffectUtilityRect.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/util/PostEffectUtilityRect.d

# Compiles file ../../source/posteffect/vs/PostEffectVS.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/vs/PostEffectVS.d
gccRelease_GLES2/source/posteffect/vs/PostEffectVS.o: ../../source/posteffect/vs/PostEffectVS.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/vs/PostEffectVS.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/vs/PostEffectVS.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/vs/PostEffectVS.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/vs/PostEffectVS.d

# Compiles file ../../source/posteffect/vs/PostEffectVSManager.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/posteffect/vs/PostEffectVSManager.d
gccRelease_GLES2/source/posteffect/vs/PostEffectVSManager.o: ../../source/posteffect/vs/PostEffectVSManager.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/posteffect/vs/PostEffectVSManager.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/posteffect/vs/PostEffectVSManager.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/posteffect/vs/PostEffectVSManager.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/posteffect/vs/PostEffectVSManager.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLES2/source/posteffect
	mkdir -p gccDebug_GLES2/source/posteffect/functor
	mkdir -p gccDebug_GLES2/source/posteffect/util
	mkdir -p gccDebug_GLES2/source/posteffect/vs
	mkdir -p ../lib/gccDebug_GLES2
	mkdir -p gccRelease_GLES2/source/posteffect
	mkdir -p gccRelease_GLES2/source/posteffect/functor
	mkdir -p gccRelease_GLES2/source/posteffect/util
	mkdir -p gccRelease_GLES2/source/posteffect/vs
	mkdir -p ../lib/gccRelease_GLES2

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLES2/source/posteffect/*.o
	rm -f gccDebug_GLES2/source/posteffect/*.d
	rm -f gccDebug_GLES2/source/posteffect/functor/*.o
	rm -f gccDebug_GLES2/source/posteffect/functor/*.d
	rm -f gccDebug_GLES2/source/posteffect/util/*.o
	rm -f gccDebug_GLES2/source/posteffect/util/*.d
	rm -f gccDebug_GLES2/source/posteffect/vs/*.o
	rm -f gccDebug_GLES2/source/posteffect/vs/*.d
	rm -f ../lib/gccDebug_GLES2/libPostEffect.a
	rm -f gccRelease_GLES2/source/posteffect/*.o
	rm -f gccRelease_GLES2/source/posteffect/*.d
	rm -f gccRelease_GLES2/source/posteffect/functor/*.o
	rm -f gccRelease_GLES2/source/posteffect/functor/*.d
	rm -f gccRelease_GLES2/source/posteffect/util/*.o
	rm -f gccRelease_GLES2/source/posteffect/util/*.d
	rm -f gccRelease_GLES2/source/posteffect/vs/*.o
	rm -f gccRelease_GLES2/source/posteffect/vs/*.d
	rm -f ../lib/gccRelease_GLES2/libPostEffect.a

