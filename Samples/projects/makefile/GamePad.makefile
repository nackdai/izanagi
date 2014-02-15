# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"../../../External/freeglut/lib/x86/gccDebug" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -ld3d9 -ld3dx9d -ldxguid -ldinput8 -ldxerr -lwinmm -lSampleKit  -Wl,--end-group

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
Debug_GLUT: create_folders GamePad/gccDebug_GLUT/GamePad/GamePad.o 
	g++ GamePad/gccDebug_GLUT/GamePad/GamePad.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccGamePad/GamePad.exe

# Compiles file ../../GamePad/GamePad.cpp for the Debug_GLUT configuration...
-include GamePad/gccDebug_GLUT/GamePad/GamePad.d
GamePad/gccDebug_GLUT/GamePad/GamePad.o: ../../GamePad/GamePad.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../GamePad/GamePad.cpp $(Debug_GLUT_Include_Path) -o GamePad/gccDebug_GLUT/GamePad/GamePad.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../GamePad/GamePad.cpp $(Debug_GLUT_Include_Path) > GamePad/gccDebug_GLUT/GamePad/GamePad.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p GamePad/gccDebug_GLUT/GamePad
	mkdir -p ../../gccGamePad

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f GamePad/gccDebug_GLUT/GamePad/*.o
	rm -f GamePad/gccDebug_GLUT/GamePad/*.d
	rm -f ../../gccGamePad/GamePad.exe

