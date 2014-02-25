# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../Animation/MyAppl" -I"../../../External/glew/include" -I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"SampleKit/gccDebug_GLUT" -L"../../../External/glew/lib" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--no-as-needed -Wl,--start-group -lSceneGraph -lShader -lMath -lGLU -lglut -lSystem -lDebugUtil -lGraph -lStd -lSampleKit -lGLEW -lGL -lX11 -lXxf86vm -lXi  -Wl,--end-group

# Preprocessor definitions...
Debug_GLUT_Preprocessor_Definitions=-D GCC_BUILD -D _WINDOWS -D _DEBUG -D __IZ_DEBUG__ -D __IZ_OGL__ -D __IZ_GLUT__ -D __IZANAGI_NO_USE_D3D__ 

# Implictly linked object files...
Debug_GLUT_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLUT_Compiler_Flags=-O0 -g 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLUT 

# Builds the Debug_GLUT configuration...
.PHONY: Debug_GLUT
Debug_GLUT: create_folders Decal/gccDebug_GLUT/Decal/Cube.o Decal/gccDebug_GLUT/Decal/DecalApp.o Decal/gccDebug_GLUT/Decal/main.o Decal/gccDebug_GLUT/Decal/Sphere.o 
	g++ Decal/gccDebug_GLUT/Decal/Cube.o Decal/gccDebug_GLUT/Decal/DecalApp.o Decal/gccDebug_GLUT/Decal/main.o Decal/gccDebug_GLUT/Decal/Sphere.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccDecal/Decal.exe

# Compiles file ../../Decal/Cube.cpp for the Debug_GLUT configuration...
-include Decal/gccDebug_GLUT/Decal/Cube.d
Decal/gccDebug_GLUT/Decal/Cube.o: ../../Decal/Cube.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Decal/Cube.cpp $(Debug_GLUT_Include_Path) -o Decal/gccDebug_GLUT/Decal/Cube.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Decal/Cube.cpp $(Debug_GLUT_Include_Path) > Decal/gccDebug_GLUT/Decal/Cube.d

# Compiles file ../../Decal/DecalApp.cpp for the Debug_GLUT configuration...
-include Decal/gccDebug_GLUT/Decal/DecalApp.d
Decal/gccDebug_GLUT/Decal/DecalApp.o: ../../Decal/DecalApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Decal/DecalApp.cpp $(Debug_GLUT_Include_Path) -o Decal/gccDebug_GLUT/Decal/DecalApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Decal/DecalApp.cpp $(Debug_GLUT_Include_Path) > Decal/gccDebug_GLUT/Decal/DecalApp.d

# Compiles file ../../Decal/main.cpp for the Debug_GLUT configuration...
-include Decal/gccDebug_GLUT/Decal/main.d
Decal/gccDebug_GLUT/Decal/main.o: ../../Decal/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Decal/main.cpp $(Debug_GLUT_Include_Path) -o Decal/gccDebug_GLUT/Decal/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Decal/main.cpp $(Debug_GLUT_Include_Path) > Decal/gccDebug_GLUT/Decal/main.d

# Compiles file ../../Decal/Sphere.cpp for the Debug_GLUT configuration...
-include Decal/gccDebug_GLUT/Decal/Sphere.d
Decal/gccDebug_GLUT/Decal/Sphere.o: ../../Decal/Sphere.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Decal/Sphere.cpp $(Debug_GLUT_Include_Path) -o Decal/gccDebug_GLUT/Decal/Sphere.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Decal/Sphere.cpp $(Debug_GLUT_Include_Path) > Decal/gccDebug_GLUT/Decal/Sphere.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Decal/gccDebug_GLUT/Decal
	mkdir -p ../../gccDecal

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Decal/gccDebug_GLUT/Decal/*.o
	rm -f Decal/gccDebug_GLUT/Decal/*.d
	rm -f ../../gccDecal/Decal.exe

