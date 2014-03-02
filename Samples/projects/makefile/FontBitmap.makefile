# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../External/glew/include" -I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"SampleKit/gccDebug_GLUT" -L"../../../External/glew/lib" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--no-as-needed -Wl,--start-group -lText -lMath -lSceneGraph -lSystem -lDebugUtil -lGraph -lStd -lSampleKit -lglut -lGLU -lGLEW -lGL -lX11 -lXxf86vm -lXi  -Wl,--end-group

# Preprocessor definitions...
Debug_GLUT_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D __IZANAGI_NO_USE_D3D__ -D __IZ_DEBUG__ -D __IZ_OGL__ -D __IZ_GLUT__ 

# Implictly linked object files...
Debug_GLUT_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLUT_Compiler_Flags=-O0 -g 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLUT 

# Builds the Debug_GLUT configuration...
.PHONY: Debug_GLUT
Debug_GLUT: create_folders FontBitmap/gccDebug_GLUT/FontBitmap/FontBitmap.o 
	g++ FontBitmap/gccDebug_GLUT/FontBitmap/FontBitmap.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccFontBitmap/FontBitmap.exe

# Compiles file ../../FontBitmap/FontBitmap.cpp for the Debug_GLUT configuration...
-include FontBitmap/gccDebug_GLUT/FontBitmap/FontBitmap.d
FontBitmap/gccDebug_GLUT/FontBitmap/FontBitmap.o: ../../FontBitmap/FontBitmap.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../FontBitmap/FontBitmap.cpp $(Debug_GLUT_Include_Path) -o FontBitmap/gccDebug_GLUT/FontBitmap/FontBitmap.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../FontBitmap/FontBitmap.cpp $(Debug_GLUT_Include_Path) > FontBitmap/gccDebug_GLUT/FontBitmap/FontBitmap.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p FontBitmap/gccDebug_GLUT/FontBitmap
	mkdir -p ../../gccFontBitmap

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f FontBitmap/gccDebug_GLUT/FontBitmap/*.o
	rm -f FontBitmap/gccDebug_GLUT/FontBitmap/*.d
	rm -f ../../gccFontBitmap/FontBitmap.exe

