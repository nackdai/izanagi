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
Debug_GLUT: create_folders ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/main.o ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/ProjectedTextureShadowApp.o ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/Shadow.o 
	g++ ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/main.o ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/ProjectedTextureShadowApp.o ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/Shadow.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccProjectedTextureShadow/ProjectedTextureShadow.exe

# Compiles file ../../ProjectedTextureShadow/main.cpp for the Debug_GLUT configuration...
-include ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/main.d
ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/main.o: ../../ProjectedTextureShadow/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../ProjectedTextureShadow/main.cpp $(Debug_GLUT_Include_Path) -o ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../ProjectedTextureShadow/main.cpp $(Debug_GLUT_Include_Path) > ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/main.d

# Compiles file ../../ProjectedTextureShadow/ProjectedTextureShadowApp.cpp for the Debug_GLUT configuration...
-include ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/ProjectedTextureShadowApp.d
ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/ProjectedTextureShadowApp.o: ../../ProjectedTextureShadow/ProjectedTextureShadowApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../ProjectedTextureShadow/ProjectedTextureShadowApp.cpp $(Debug_GLUT_Include_Path) -o ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/ProjectedTextureShadowApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../ProjectedTextureShadow/ProjectedTextureShadowApp.cpp $(Debug_GLUT_Include_Path) > ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/ProjectedTextureShadowApp.d

# Compiles file ../../ProjectedTextureShadow/Shadow.cpp for the Debug_GLUT configuration...
-include ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/Shadow.d
ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/Shadow.o: ../../ProjectedTextureShadow/Shadow.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../ProjectedTextureShadow/Shadow.cpp $(Debug_GLUT_Include_Path) -o ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/Shadow.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../ProjectedTextureShadow/Shadow.cpp $(Debug_GLUT_Include_Path) > ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/Shadow.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow
	mkdir -p ../../gccProjectedTextureShadow

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/*.o
	rm -f ProjectedTextureShadow/gccDebug_GLUT/ProjectedTextureShadow/*.d
	rm -f ../../gccProjectedTextureShadow/ProjectedTextureShadow.exe

