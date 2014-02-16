# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"../../../External/freeglut/lib/x86/gccDebug" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -lShader -lAnimation -ld3d9 -ld3dx9d -ldxguid -ldinput8 -ldxerr -lwinmm -lSampleKit  -Wl,--end-group

# Preprocessor definitions...
Debug_GLUT_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _WINDOWS -D __IZ_DEBUG__ -D __IZ_OGL__ -D __IZ_GLUT__ -D __IZANAGI_NO_USE_D3D__ 

# Implictly linked object files...
Debug_GLUT_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLUT_Compiler_Flags=-O0 -g 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLUT 

# Builds the Debug_GLUT configuration...
.PHONY: Debug_GLUT
Debug_GLUT: create_folders MotionBind/gccDebug_GLUT/MotionBind/CustomInterp.o MotionBind/gccDebug_GLUT/MotionBind/main.o MotionBind/gccDebug_GLUT/MotionBind/MotionBindApp.o 
	g++ MotionBind/gccDebug_GLUT/MotionBind/CustomInterp.o MotionBind/gccDebug_GLUT/MotionBind/main.o MotionBind/gccDebug_GLUT/MotionBind/MotionBindApp.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccMotionBind/MotionBind.exe

# Compiles file ../../MotionBind/CustomInterp.cpp for the Debug_GLUT configuration...
-include MotionBind/gccDebug_GLUT/MotionBind/CustomInterp.d
MotionBind/gccDebug_GLUT/MotionBind/CustomInterp.o: ../../MotionBind/CustomInterp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MotionBind/CustomInterp.cpp $(Debug_GLUT_Include_Path) -o MotionBind/gccDebug_GLUT/MotionBind/CustomInterp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MotionBind/CustomInterp.cpp $(Debug_GLUT_Include_Path) > MotionBind/gccDebug_GLUT/MotionBind/CustomInterp.d

# Compiles file ../../MotionBind/main.cpp for the Debug_GLUT configuration...
-include MotionBind/gccDebug_GLUT/MotionBind/main.d
MotionBind/gccDebug_GLUT/MotionBind/main.o: ../../MotionBind/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MotionBind/main.cpp $(Debug_GLUT_Include_Path) -o MotionBind/gccDebug_GLUT/MotionBind/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MotionBind/main.cpp $(Debug_GLUT_Include_Path) > MotionBind/gccDebug_GLUT/MotionBind/main.d

# Compiles file ../../MotionBind/MotionBindApp.cpp for the Debug_GLUT configuration...
-include MotionBind/gccDebug_GLUT/MotionBind/MotionBindApp.d
MotionBind/gccDebug_GLUT/MotionBind/MotionBindApp.o: ../../MotionBind/MotionBindApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MotionBind/MotionBindApp.cpp $(Debug_GLUT_Include_Path) -o MotionBind/gccDebug_GLUT/MotionBind/MotionBindApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MotionBind/MotionBindApp.cpp $(Debug_GLUT_Include_Path) > MotionBind/gccDebug_GLUT/MotionBind/MotionBindApp.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p MotionBind/gccDebug_GLUT/MotionBind
	mkdir -p ../../gccMotionBind

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f MotionBind/gccDebug_GLUT/MotionBind/*.o
	rm -f MotionBind/gccDebug_GLUT/MotionBind/*.d
	rm -f ../../gccMotionBind/MotionBind.exe

