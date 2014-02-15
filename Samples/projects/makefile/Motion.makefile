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
Debug_GLUT: create_folders Motion/gccDebug_GLUT/Motion/MotionApp.o Motion/gccDebug_GLUT/Motion/main.o Motion/gccDebug_GLUT/Motion/StateBase.o Motion/gccDebug_GLUT/Motion/StateCollada.o Motion/gccDebug_GLUT/Motion/StateManager.o Motion/gccDebug_GLUT/Motion/StateXFile.o 
	g++ Motion/gccDebug_GLUT/Motion/MotionApp.o Motion/gccDebug_GLUT/Motion/main.o Motion/gccDebug_GLUT/Motion/StateBase.o Motion/gccDebug_GLUT/Motion/StateCollada.o Motion/gccDebug_GLUT/Motion/StateManager.o Motion/gccDebug_GLUT/Motion/StateXFile.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccMotion/Motion.exe

# Compiles file ../../Motion/MotionApp.cpp for the Debug_GLUT configuration...
-include Motion/gccDebug_GLUT/Motion/MotionApp.d
Motion/gccDebug_GLUT/Motion/MotionApp.o: ../../Motion/MotionApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Motion/MotionApp.cpp $(Debug_GLUT_Include_Path) -o Motion/gccDebug_GLUT/Motion/MotionApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Motion/MotionApp.cpp $(Debug_GLUT_Include_Path) > Motion/gccDebug_GLUT/Motion/MotionApp.d

# Compiles file ../../Motion/main.cpp for the Debug_GLUT configuration...
-include Motion/gccDebug_GLUT/Motion/main.d
Motion/gccDebug_GLUT/Motion/main.o: ../../Motion/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Motion/main.cpp $(Debug_GLUT_Include_Path) -o Motion/gccDebug_GLUT/Motion/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Motion/main.cpp $(Debug_GLUT_Include_Path) > Motion/gccDebug_GLUT/Motion/main.d

# Compiles file ../../Motion/StateBase.cpp for the Debug_GLUT configuration...
-include Motion/gccDebug_GLUT/Motion/StateBase.d
Motion/gccDebug_GLUT/Motion/StateBase.o: ../../Motion/StateBase.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Motion/StateBase.cpp $(Debug_GLUT_Include_Path) -o Motion/gccDebug_GLUT/Motion/StateBase.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Motion/StateBase.cpp $(Debug_GLUT_Include_Path) > Motion/gccDebug_GLUT/Motion/StateBase.d

# Compiles file ../../Motion/StateCollada.cpp for the Debug_GLUT configuration...
-include Motion/gccDebug_GLUT/Motion/StateCollada.d
Motion/gccDebug_GLUT/Motion/StateCollada.o: ../../Motion/StateCollada.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Motion/StateCollada.cpp $(Debug_GLUT_Include_Path) -o Motion/gccDebug_GLUT/Motion/StateCollada.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Motion/StateCollada.cpp $(Debug_GLUT_Include_Path) > Motion/gccDebug_GLUT/Motion/StateCollada.d

# Compiles file ../../Motion/StateManager.cpp for the Debug_GLUT configuration...
-include Motion/gccDebug_GLUT/Motion/StateManager.d
Motion/gccDebug_GLUT/Motion/StateManager.o: ../../Motion/StateManager.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Motion/StateManager.cpp $(Debug_GLUT_Include_Path) -o Motion/gccDebug_GLUT/Motion/StateManager.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Motion/StateManager.cpp $(Debug_GLUT_Include_Path) > Motion/gccDebug_GLUT/Motion/StateManager.d

# Compiles file ../../Motion/StateXFile.cpp for the Debug_GLUT configuration...
-include Motion/gccDebug_GLUT/Motion/StateXFile.d
Motion/gccDebug_GLUT/Motion/StateXFile.o: ../../Motion/StateXFile.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Motion/StateXFile.cpp $(Debug_GLUT_Include_Path) -o Motion/gccDebug_GLUT/Motion/StateXFile.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Motion/StateXFile.cpp $(Debug_GLUT_Include_Path) > Motion/gccDebug_GLUT/Motion/StateXFile.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Motion/gccDebug_GLUT/Motion
	mkdir -p ../../gccMotion

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Motion/gccDebug_GLUT/Motion/*.o
	rm -f Motion/gccDebug_GLUT/Motion/*.d
	rm -f ../../gccMotion/Motion.exe

