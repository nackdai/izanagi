# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../include" -I"../../../External/glew/include" -I"../../../External/freeglut/include" 

# Library paths...
Debug_GLUT_Library_Path=

# Additional libraries...
Debug_GLUT_Libraries=

# Preprocessor definitions...
Debug_GLUT_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _LIB -D __IZ_DEBUG__ -D __IZ_OGL__ -D __IZ_GLUT__ -D __IZANAGI_NO_USE_D3D__ 

# Implictly linked object files...
Debug_GLUT_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLUT_Compiler_Flags=-O0 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLUT 

# Builds the Debug_GLUT configuration...
.PHONY: Debug_GLUT
Debug_GLUT: create_folders gccDebug_GLUT/source/posteffect/PostEffect.o gccDebug_GLUT/source/posteffect/PostEffectShader.o gccDebug_GLUT/source/posteffect/PostEffectShaderPass.o gccDebug_GLUT/source/posteffect/PostEffectSystem.o gccDebug_GLUT/source/posteffect/PostEffectTextureCreator.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctor.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDefault.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDOF.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorMGF.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorRenderStar.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorSwitchTex.o gccDebug_GLUT/source/posteffect/util/PostEffectShaderUtil.o gccDebug_GLUT/source/posteffect/util/PostEffectUtilityRect.o gccDebug_GLUT/source/posteffect/vs/PostEffectVS.o gccDebug_GLUT/source/posteffect/vs/PostEffectVSManager.o 
	ar rcs ../lib/gccDebug_GLUT/libPostEffect.a gccDebug_GLUT/source/posteffect/PostEffect.o gccDebug_GLUT/source/posteffect/PostEffectShader.o gccDebug_GLUT/source/posteffect/PostEffectShaderPass.o gccDebug_GLUT/source/posteffect/PostEffectSystem.o gccDebug_GLUT/source/posteffect/PostEffectTextureCreator.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctor.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDefault.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDOF.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorMGF.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorRenderStar.o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorSwitchTex.o gccDebug_GLUT/source/posteffect/util/PostEffectShaderUtil.o gccDebug_GLUT/source/posteffect/util/PostEffectUtilityRect.o gccDebug_GLUT/source/posteffect/vs/PostEffectVS.o gccDebug_GLUT/source/posteffect/vs/PostEffectVSManager.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/posteffect/PostEffect.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/PostEffect.d
gccDebug_GLUT/source/posteffect/PostEffect.o: ../../source/posteffect/PostEffect.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/PostEffect.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/PostEffect.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/PostEffect.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/PostEffect.d

# Compiles file ../../source/posteffect/PostEffectShader.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/PostEffectShader.d
gccDebug_GLUT/source/posteffect/PostEffectShader.o: ../../source/posteffect/PostEffectShader.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/PostEffectShader.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/PostEffectShader.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/PostEffectShader.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/PostEffectShader.d

# Compiles file ../../source/posteffect/PostEffectShaderPass.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/PostEffectShaderPass.d
gccDebug_GLUT/source/posteffect/PostEffectShaderPass.o: ../../source/posteffect/PostEffectShaderPass.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/PostEffectShaderPass.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/PostEffectShaderPass.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/PostEffectShaderPass.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/PostEffectShaderPass.d

# Compiles file ../../source/posteffect/PostEffectSystem.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/PostEffectSystem.d
gccDebug_GLUT/source/posteffect/PostEffectSystem.o: ../../source/posteffect/PostEffectSystem.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/PostEffectSystem.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/PostEffectSystem.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/PostEffectSystem.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/PostEffectSystem.d

# Compiles file ../../source/posteffect/PostEffectTextureCreator.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/PostEffectTextureCreator.d
gccDebug_GLUT/source/posteffect/PostEffectTextureCreator.o: ../../source/posteffect/PostEffectTextureCreator.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/PostEffectTextureCreator.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/PostEffectTextureCreator.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/PostEffectTextureCreator.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/PostEffectTextureCreator.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctor.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/functor/PostEffectFunctor.d
gccDebug_GLUT/source/posteffect/functor/PostEffectFunctor.o: ../../source/posteffect/functor/PostEffectFunctor.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctor.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctor.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctor.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/functor/PostEffectFunctor.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorDefault.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDefault.d
gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDefault.o: ../../source/posteffect/functor/PostEffectFunctorDefault.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorDefault.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDefault.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorDefault.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDefault.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorDOF.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDOF.d
gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDOF.o: ../../source/posteffect/functor/PostEffectFunctorDOF.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorDOF.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDOF.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorDOF.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorDOF.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorMGF.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorMGF.d
gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorMGF.o: ../../source/posteffect/functor/PostEffectFunctorMGF.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorMGF.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorMGF.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorMGF.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorMGF.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorRenderStar.d
gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorRenderStar.o: ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorRenderStar.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorRenderStar.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorRenderStar.d

# Compiles file ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorSwitchTex.d
gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorSwitchTex.o: ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorSwitchTex.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/functor/PostEffectFunctorSwitchTex.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/functor/PostEffectFunctorSwitchTex.d

# Compiles file ../../source/posteffect/util/PostEffectShaderUtil.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/util/PostEffectShaderUtil.d
gccDebug_GLUT/source/posteffect/util/PostEffectShaderUtil.o: ../../source/posteffect/util/PostEffectShaderUtil.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/util/PostEffectShaderUtil.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/util/PostEffectShaderUtil.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/util/PostEffectShaderUtil.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/util/PostEffectShaderUtil.d

# Compiles file ../../source/posteffect/util/PostEffectUtilityRect.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/util/PostEffectUtilityRect.d
gccDebug_GLUT/source/posteffect/util/PostEffectUtilityRect.o: ../../source/posteffect/util/PostEffectUtilityRect.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/util/PostEffectUtilityRect.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/util/PostEffectUtilityRect.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/util/PostEffectUtilityRect.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/util/PostEffectUtilityRect.d

# Compiles file ../../source/posteffect/vs/PostEffectVS.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/vs/PostEffectVS.d
gccDebug_GLUT/source/posteffect/vs/PostEffectVS.o: ../../source/posteffect/vs/PostEffectVS.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/vs/PostEffectVS.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/vs/PostEffectVS.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/vs/PostEffectVS.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/vs/PostEffectVS.d

# Compiles file ../../source/posteffect/vs/PostEffectVSManager.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/posteffect/vs/PostEffectVSManager.d
gccDebug_GLUT/source/posteffect/vs/PostEffectVSManager.o: ../../source/posteffect/vs/PostEffectVSManager.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/posteffect/vs/PostEffectVSManager.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/posteffect/vs/PostEffectVSManager.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/posteffect/vs/PostEffectVSManager.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/posteffect/vs/PostEffectVSManager.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/posteffect
	mkdir -p gccDebug_GLUT/source/posteffect/functor
	mkdir -p gccDebug_GLUT/source/posteffect/util
	mkdir -p gccDebug_GLUT/source/posteffect/vs
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/posteffect/*.o
	rm -f gccDebug_GLUT/source/posteffect/*.d
	rm -f gccDebug_GLUT/source/posteffect/functor/*.o
	rm -f gccDebug_GLUT/source/posteffect/functor/*.d
	rm -f gccDebug_GLUT/source/posteffect/util/*.o
	rm -f gccDebug_GLUT/source/posteffect/util/*.d
	rm -f gccDebug_GLUT/source/posteffect/vs/*.o
	rm -f gccDebug_GLUT/source/posteffect/vs/*.d
	rm -f ../lib/gccDebug_GLUT/libPostEffect.a

