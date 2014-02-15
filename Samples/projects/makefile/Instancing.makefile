# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"../../../External/freeglut/lib/x86/gccDebug" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -lShader -ld3d9 -ld3dx9d -ldxguid -ldinput8 -ldxerr -lwinmm -lSampleKit  -Wl,--end-group

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
Debug_GLUT: create_folders Instancing/gccDebug_GLUT/Instancing/InstancingApp.o Instancing/gccDebug_GLUT/Instancing/main.o 
	g++ Instancing/gccDebug_GLUT/Instancing/InstancingApp.o Instancing/gccDebug_GLUT/Instancing/main.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccInstancing/Instancing.exe

# Compiles file ../../Instancing/InstancingApp.cpp for the Debug_GLUT configuration...
-include Instancing/gccDebug_GLUT/Instancing/InstancingApp.d
Instancing/gccDebug_GLUT/Instancing/InstancingApp.o: ../../Instancing/InstancingApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Instancing/InstancingApp.cpp $(Debug_GLUT_Include_Path) -o Instancing/gccDebug_GLUT/Instancing/InstancingApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Instancing/InstancingApp.cpp $(Debug_GLUT_Include_Path) > Instancing/gccDebug_GLUT/Instancing/InstancingApp.d

# Compiles file ../../Instancing/main.cpp for the Debug_GLUT configuration...
-include Instancing/gccDebug_GLUT/Instancing/main.d
Instancing/gccDebug_GLUT/Instancing/main.o: ../../Instancing/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Instancing/main.cpp $(Debug_GLUT_Include_Path) -o Instancing/gccDebug_GLUT/Instancing/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Instancing/main.cpp $(Debug_GLUT_Include_Path) > Instancing/gccDebug_GLUT/Instancing/main.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Instancing/gccDebug_GLUT/Instancing
	mkdir -p ../../gccInstancing

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Instancing/gccDebug_GLUT/Instancing/*.o
	rm -f Instancing/gccDebug_GLUT/Instancing/*.d
	rm -f ../../gccInstancing/Instancing.exe

