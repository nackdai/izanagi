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
Debug_GLUT: create_folders WaterDrop/gccDebug_GLUT/WaterDrop/WaterDrop.o 
	g++ WaterDrop/gccDebug_GLUT/WaterDrop/WaterDrop.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccWaterDrop/WaterDrop.exe

# Compiles file ../../WaterDrop/WaterDrop.cpp for the Debug_GLUT configuration...
-include WaterDrop/gccDebug_GLUT/WaterDrop/WaterDrop.d
WaterDrop/gccDebug_GLUT/WaterDrop/WaterDrop.o: ../../WaterDrop/WaterDrop.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../WaterDrop/WaterDrop.cpp $(Debug_GLUT_Include_Path) -o WaterDrop/gccDebug_GLUT/WaterDrop/WaterDrop.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../WaterDrop/WaterDrop.cpp $(Debug_GLUT_Include_Path) > WaterDrop/gccDebug_GLUT/WaterDrop/WaterDrop.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p WaterDrop/gccDebug_GLUT/WaterDrop
	mkdir -p ../../gccWaterDrop

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f WaterDrop/gccDebug_GLUT/WaterDrop/*.o
	rm -f WaterDrop/gccDebug_GLUT/WaterDrop/*.d
	rm -f ../../gccWaterDrop/WaterDrop.exe
