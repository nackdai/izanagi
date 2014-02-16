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
Debug_GLUT: create_folders MotionInterp/gccDebug_GLUT/MotionInterp/CustomInterp.o MotionInterp/gccDebug_GLUT/MotionInterp/main.o MotionInterp/gccDebug_GLUT/MotionInterp/MotionInterpApp.o 
	g++ MotionInterp/gccDebug_GLUT/MotionInterp/CustomInterp.o MotionInterp/gccDebug_GLUT/MotionInterp/main.o MotionInterp/gccDebug_GLUT/MotionInterp/MotionInterpApp.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccMotionInterp/MotionInterp.exe

# Compiles file ../../MotionInterp/CustomInterp.cpp for the Debug_GLUT configuration...
-include MotionInterp/gccDebug_GLUT/MotionInterp/CustomInterp.d
MotionInterp/gccDebug_GLUT/MotionInterp/CustomInterp.o: ../../MotionInterp/CustomInterp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MotionInterp/CustomInterp.cpp $(Debug_GLUT_Include_Path) -o MotionInterp/gccDebug_GLUT/MotionInterp/CustomInterp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MotionInterp/CustomInterp.cpp $(Debug_GLUT_Include_Path) > MotionInterp/gccDebug_GLUT/MotionInterp/CustomInterp.d

# Compiles file ../../MotionInterp/main.cpp for the Debug_GLUT configuration...
-include MotionInterp/gccDebug_GLUT/MotionInterp/main.d
MotionInterp/gccDebug_GLUT/MotionInterp/main.o: ../../MotionInterp/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MotionInterp/main.cpp $(Debug_GLUT_Include_Path) -o MotionInterp/gccDebug_GLUT/MotionInterp/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MotionInterp/main.cpp $(Debug_GLUT_Include_Path) > MotionInterp/gccDebug_GLUT/MotionInterp/main.d

# Compiles file ../../MotionInterp/MotionInterpApp.cpp for the Debug_GLUT configuration...
-include MotionInterp/gccDebug_GLUT/MotionInterp/MotionInterpApp.d
MotionInterp/gccDebug_GLUT/MotionInterp/MotionInterpApp.o: ../../MotionInterp/MotionInterpApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MotionInterp/MotionInterpApp.cpp $(Debug_GLUT_Include_Path) -o MotionInterp/gccDebug_GLUT/MotionInterp/MotionInterpApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MotionInterp/MotionInterpApp.cpp $(Debug_GLUT_Include_Path) > MotionInterp/gccDebug_GLUT/MotionInterp/MotionInterpApp.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p MotionInterp/gccDebug_GLUT/MotionInterp
	mkdir -p ../../gccMotionInterp

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f MotionInterp/gccDebug_GLUT/MotionInterp/*.o
	rm -f MotionInterp/gccDebug_GLUT/MotionInterp/*.d
	rm -f ../../gccMotionInterp/MotionInterp.exe

