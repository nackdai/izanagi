# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"../../../External/freeglut/lib/x86/gccDebug" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -lResource -lShader -ld3d9 -ld3dx9d -ldxguid -ldinput8 -ldxerr -lwinmm -lSampleKit  -Wl,--end-group

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
Debug_GLUT: create_folders Archive/gccDebug_GLUT/Archive/main.o Archive/gccDebug_GLUT/Archive/Archive.o 
	g++ Archive/gccDebug_GLUT/Archive/main.o Archive/gccDebug_GLUT/Archive/Archive.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccArchive/Archive.exe

# Compiles file ../../Archive/main.cpp for the Debug_GLUT configuration...
-include Archive/gccDebug_GLUT/Archive/main.d
Archive/gccDebug_GLUT/Archive/main.o: ../../Archive/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Archive/main.cpp $(Debug_GLUT_Include_Path) -o Archive/gccDebug_GLUT/Archive/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Archive/main.cpp $(Debug_GLUT_Include_Path) > Archive/gccDebug_GLUT/Archive/main.d

# Compiles file ../../Archive/Archive.cpp for the Debug_GLUT configuration...
-include Archive/gccDebug_GLUT/Archive/Archive.d
Archive/gccDebug_GLUT/Archive/Archive.o: ../../Archive/Archive.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Archive/Archive.cpp $(Debug_GLUT_Include_Path) -o Archive/gccDebug_GLUT/Archive/Archive.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Archive/Archive.cpp $(Debug_GLUT_Include_Path) > Archive/gccDebug_GLUT/Archive/Archive.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Archive/gccDebug_GLUT/Archive
	mkdir -p ../../gccArchive

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Archive/gccDebug_GLUT/Archive/*.o
	rm -f Archive/gccDebug_GLUT/Archive/*.d
	rm -f ../../gccArchive/Archive.exe

