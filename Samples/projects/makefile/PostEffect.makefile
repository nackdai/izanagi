# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"../../../External/freeglut/lib/x86/gccDebug" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -lShader -lPostEffect -ld3d9 -ld3dx9d -ldxguid -ldinput8 -ldxerr -lwinmm -lSampleKit  -Wl,--end-group

# Preprocessor definitions...
Debug_GLUT_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _WINDOWS -D __IZ_DEBUG__ -D __IZ_OGL__ -D __IZ_GLUT__ 

# Implictly linked object files...
Debug_GLUT_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLUT_Compiler_Flags=-O0 -g 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLUT 

# Builds the Debug_GLUT configuration...
.PHONY: Debug_GLUT
Debug_GLUT: create_folders PostEffect/gccDebug_GLUT/PostEffect/main.o PostEffect/gccDebug_GLUT/PostEffect/PostEffectApp.o PostEffect/gccDebug_GLUT/PostEffect/PostEffectSample.o PostEffect/gccDebug_GLUT/PostEffect/SceneRenderer.o PostEffect/gccDebug_GLUT/PostEffect/StateBase.o PostEffect/gccDebug_GLUT/PostEffect/StateBloomStar.o PostEffect/gccDebug_GLUT/PostEffect/StateBloomStarMGF.o PostEffect/gccDebug_GLUT/PostEffect/StateHDR.o PostEffect/gccDebug_GLUT/PostEffect/StateHDR2.o PostEffect/gccDebug_GLUT/PostEffect/StateManager.o PostEffect/gccDebug_GLUT/PostEffect/StateMonotone.o PostEffect/gccDebug_GLUT/PostEffect/StateNegative.o PostEffect/gccDebug_GLUT/PostEffect/StatePostEffect.o PostEffect/gccDebug_GLUT/PostEffect/StateSepia.o 
	g++ PostEffect/gccDebug_GLUT/PostEffect/main.o PostEffect/gccDebug_GLUT/PostEffect/PostEffectApp.o PostEffect/gccDebug_GLUT/PostEffect/PostEffectSample.o PostEffect/gccDebug_GLUT/PostEffect/SceneRenderer.o PostEffect/gccDebug_GLUT/PostEffect/StateBase.o PostEffect/gccDebug_GLUT/PostEffect/StateBloomStar.o PostEffect/gccDebug_GLUT/PostEffect/StateBloomStarMGF.o PostEffect/gccDebug_GLUT/PostEffect/StateHDR.o PostEffect/gccDebug_GLUT/PostEffect/StateHDR2.o PostEffect/gccDebug_GLUT/PostEffect/StateManager.o PostEffect/gccDebug_GLUT/PostEffect/StateMonotone.o PostEffect/gccDebug_GLUT/PostEffect/StateNegative.o PostEffect/gccDebug_GLUT/PostEffect/StatePostEffect.o PostEffect/gccDebug_GLUT/PostEffect/StateSepia.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccPostEffect/PostEffect.exe

# Compiles file ../../PostEffect/main.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/main.d
PostEffect/gccDebug_GLUT/PostEffect/main.o: ../../PostEffect/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/main.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/main.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/main.d

# Compiles file ../../PostEffect/PostEffectApp.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/PostEffectApp.d
PostEffect/gccDebug_GLUT/PostEffect/PostEffectApp.o: ../../PostEffect/PostEffectApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/PostEffectApp.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/PostEffectApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/PostEffectApp.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/PostEffectApp.d

# Compiles file ../../PostEffect/PostEffectSample.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/PostEffectSample.d
PostEffect/gccDebug_GLUT/PostEffect/PostEffectSample.o: ../../PostEffect/PostEffectSample.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/PostEffectSample.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/PostEffectSample.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/PostEffectSample.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/PostEffectSample.d

# Compiles file ../../PostEffect/SceneRenderer.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/SceneRenderer.d
PostEffect/gccDebug_GLUT/PostEffect/SceneRenderer.o: ../../PostEffect/SceneRenderer.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/SceneRenderer.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/SceneRenderer.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/SceneRenderer.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/SceneRenderer.d

# Compiles file ../../PostEffect/StateBase.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StateBase.d
PostEffect/gccDebug_GLUT/PostEffect/StateBase.o: ../../PostEffect/StateBase.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StateBase.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StateBase.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StateBase.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StateBase.d

# Compiles file ../../PostEffect/StateBloomStar.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StateBloomStar.d
PostEffect/gccDebug_GLUT/PostEffect/StateBloomStar.o: ../../PostEffect/StateBloomStar.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StateBloomStar.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StateBloomStar.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StateBloomStar.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StateBloomStar.d

# Compiles file ../../PostEffect/StateBloomStarMGF.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StateBloomStarMGF.d
PostEffect/gccDebug_GLUT/PostEffect/StateBloomStarMGF.o: ../../PostEffect/StateBloomStarMGF.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StateBloomStarMGF.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StateBloomStarMGF.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StateBloomStarMGF.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StateBloomStarMGF.d

# Compiles file ../../PostEffect/StateHDR.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StateHDR.d
PostEffect/gccDebug_GLUT/PostEffect/StateHDR.o: ../../PostEffect/StateHDR.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StateHDR.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StateHDR.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StateHDR.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StateHDR.d

# Compiles file ../../PostEffect/StateHDR2.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StateHDR2.d
PostEffect/gccDebug_GLUT/PostEffect/StateHDR2.o: ../../PostEffect/StateHDR2.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StateHDR2.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StateHDR2.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StateHDR2.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StateHDR2.d

# Compiles file ../../PostEffect/StateManager.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StateManager.d
PostEffect/gccDebug_GLUT/PostEffect/StateManager.o: ../../PostEffect/StateManager.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StateManager.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StateManager.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StateManager.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StateManager.d

# Compiles file ../../PostEffect/StateMonotone.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StateMonotone.d
PostEffect/gccDebug_GLUT/PostEffect/StateMonotone.o: ../../PostEffect/StateMonotone.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StateMonotone.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StateMonotone.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StateMonotone.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StateMonotone.d

# Compiles file ../../PostEffect/StateNegative.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StateNegative.d
PostEffect/gccDebug_GLUT/PostEffect/StateNegative.o: ../../PostEffect/StateNegative.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StateNegative.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StateNegative.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StateNegative.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StateNegative.d

# Compiles file ../../PostEffect/StatePostEffect.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StatePostEffect.d
PostEffect/gccDebug_GLUT/PostEffect/StatePostEffect.o: ../../PostEffect/StatePostEffect.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StatePostEffect.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StatePostEffect.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StatePostEffect.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StatePostEffect.d

# Compiles file ../../PostEffect/StateSepia.cpp for the Debug_GLUT configuration...
-include PostEffect/gccDebug_GLUT/PostEffect/StateSepia.d
PostEffect/gccDebug_GLUT/PostEffect/StateSepia.o: ../../PostEffect/StateSepia.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../PostEffect/StateSepia.cpp $(Debug_GLUT_Include_Path) -o PostEffect/gccDebug_GLUT/PostEffect/StateSepia.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../PostEffect/StateSepia.cpp $(Debug_GLUT_Include_Path) > PostEffect/gccDebug_GLUT/PostEffect/StateSepia.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p PostEffect/gccDebug_GLUT/PostEffect
	mkdir -p ../../gccPostEffect

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f PostEffect/gccDebug_GLUT/PostEffect/*.o
	rm -f PostEffect/gccDebug_GLUT/PostEffect/*.d
	rm -f ../../gccPostEffect/PostEffect.exe

