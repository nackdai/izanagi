# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../External/glew/include" -I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"SampleKit/gccDebug_GLUT" -L"../../../External/glew/lib" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--no-as-needed -Wl,--start-group -lShader -lAnimation -lMath -lSceneGraph -lSystem -lDebugUtil -lGraph -lStd -lSampleKit -lglut -lGLU -lGLEW -lGL -lX11 -lXxf86vm -lXi  -Wl,--end-group

# Preprocessor definitions...
Debug_GLUT_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D __IZANAGI_NO_USE_D3D__ -D __IZ_DEBUG__ -D __IZ_OGL__ -D __IZ_GLUT__ 

# Implictly linked object files...
Debug_GLUT_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLUT_Compiler_Flags=-O0 -g 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLUT 

# Builds the Debug_GLUT configuration...
.PHONY: Debug_GLUT
Debug_GLUT: create_folders MotionBlend/gccDebug_GLUT/MotionBlend/CustomInterp.o MotionBlend/gccDebug_GLUT/MotionBlend/main.o MotionBlend/gccDebug_GLUT/MotionBlend/MotionBlendApp.o 
	g++ MotionBlend/gccDebug_GLUT/MotionBlend/CustomInterp.o MotionBlend/gccDebug_GLUT/MotionBlend/main.o MotionBlend/gccDebug_GLUT/MotionBlend/MotionBlendApp.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccMotionBlend/MotionBlend.exe

# Compiles file ../../MotionBlend/CustomInterp.cpp for the Debug_GLUT configuration...
-include MotionBlend/gccDebug_GLUT/MotionBlend/CustomInterp.d
MotionBlend/gccDebug_GLUT/MotionBlend/CustomInterp.o: ../../MotionBlend/CustomInterp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MotionBlend/CustomInterp.cpp $(Debug_GLUT_Include_Path) -o MotionBlend/gccDebug_GLUT/MotionBlend/CustomInterp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MotionBlend/CustomInterp.cpp $(Debug_GLUT_Include_Path) > MotionBlend/gccDebug_GLUT/MotionBlend/CustomInterp.d

# Compiles file ../../MotionBlend/main.cpp for the Debug_GLUT configuration...
-include MotionBlend/gccDebug_GLUT/MotionBlend/main.d
MotionBlend/gccDebug_GLUT/MotionBlend/main.o: ../../MotionBlend/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MotionBlend/main.cpp $(Debug_GLUT_Include_Path) -o MotionBlend/gccDebug_GLUT/MotionBlend/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MotionBlend/main.cpp $(Debug_GLUT_Include_Path) > MotionBlend/gccDebug_GLUT/MotionBlend/main.d

# Compiles file ../../MotionBlend/MotionBlendApp.cpp for the Debug_GLUT configuration...
-include MotionBlend/gccDebug_GLUT/MotionBlend/MotionBlendApp.d
MotionBlend/gccDebug_GLUT/MotionBlend/MotionBlendApp.o: ../../MotionBlend/MotionBlendApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MotionBlend/MotionBlendApp.cpp $(Debug_GLUT_Include_Path) -o MotionBlend/gccDebug_GLUT/MotionBlend/MotionBlendApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MotionBlend/MotionBlendApp.cpp $(Debug_GLUT_Include_Path) > MotionBlend/gccDebug_GLUT/MotionBlend/MotionBlendApp.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p MotionBlend/gccDebug_GLUT/MotionBlend
	mkdir -p ../../gccMotionBlend

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f MotionBlend/gccDebug_GLUT/MotionBlend/*.o
	rm -f MotionBlend/gccDebug_GLUT/MotionBlend/*.d
	rm -f ../../gccMotionBlend/MotionBlend.exe

