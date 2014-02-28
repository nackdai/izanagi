# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../External/glew/include" -I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"SampleKit/gccDebug_GLUT" -L"../../../External/glew/lib" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--no-as-needed -Wl,--start-group -lThreadModel -lSceneGraph -lSystem -lDebugUtil -lGraph -lStd -lpthread -lSampleKit -lMath -lglut -lGLU -lGLEW -lGL -lX11 -lXxf86vm -lXi  -Wl,--end-group

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
Debug_GLUT: create_folders Parallel/gccDebug_GLUT/Parallel/main.o 
	g++ Parallel/gccDebug_GLUT/Parallel/main.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccParallel/Parallel.exe

# Compiles file ../../Parallel/main.cpp for the Debug_GLUT configuration...
-include Parallel/gccDebug_GLUT/Parallel/main.d
Parallel/gccDebug_GLUT/Parallel/main.o: ../../Parallel/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Parallel/main.cpp $(Debug_GLUT_Include_Path) -o Parallel/gccDebug_GLUT/Parallel/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Parallel/main.cpp $(Debug_GLUT_Include_Path) > Parallel/gccDebug_GLUT/Parallel/main.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Parallel/gccDebug_GLUT/Parallel
	mkdir -p ../../gccParallel

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Parallel/gccDebug_GLUT/Parallel/*.o
	rm -f Parallel/gccDebug_GLUT/Parallel/*.d
	rm -f ../../gccParallel/Parallel.exe

