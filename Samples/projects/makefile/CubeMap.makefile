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
Debug_GLUT: create_folders CubeMap/gccDebug_GLUT/cubemap/CubeMapApp.o CubeMap/gccDebug_GLUT/cubemap/main.o 
	g++ CubeMap/gccDebug_GLUT/cubemap/CubeMapApp.o CubeMap/gccDebug_GLUT/cubemap/main.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccCubeMap/CubeMap.exe

# Compiles file ../../cubemap/CubeMapApp.cpp for the Debug_GLUT configuration...
-include CubeMap/gccDebug_GLUT/cubemap/CubeMapApp.d
CubeMap/gccDebug_GLUT/cubemap/CubeMapApp.o: ../../cubemap/CubeMapApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../cubemap/CubeMapApp.cpp $(Debug_GLUT_Include_Path) -o CubeMap/gccDebug_GLUT/cubemap/CubeMapApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../cubemap/CubeMapApp.cpp $(Debug_GLUT_Include_Path) > CubeMap/gccDebug_GLUT/cubemap/CubeMapApp.d

# Compiles file ../../cubemap/main.cpp for the Debug_GLUT configuration...
-include CubeMap/gccDebug_GLUT/cubemap/main.d
CubeMap/gccDebug_GLUT/cubemap/main.o: ../../cubemap/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../cubemap/main.cpp $(Debug_GLUT_Include_Path) -o CubeMap/gccDebug_GLUT/cubemap/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../cubemap/main.cpp $(Debug_GLUT_Include_Path) > CubeMap/gccDebug_GLUT/cubemap/main.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p CubeMap/gccDebug_GLUT/cubemap
	mkdir -p ../../gccCubeMap

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f CubeMap/gccDebug_GLUT/cubemap/*.o
	rm -f CubeMap/gccDebug_GLUT/cubemap/*.d
	rm -f ../../gccCubeMap/CubeMap.exe

