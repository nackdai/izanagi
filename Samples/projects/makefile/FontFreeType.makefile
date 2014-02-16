# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../External/freetype/objs/win32/gccvc2010" -L"../../../Libs/project/lib/gccDebug_GLUT" -L"../../../External/freeglut/lib/x86/gccDebug" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -lText -lfreetype2411MT_D -ld3d9 -ld3dx9d -ldxguid -ldinput8 -ldxerr -lwinmm -lSampleKit  -Wl,--end-group

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
Debug_GLUT: create_folders FontFreeType/gccDebug_GLUT/FontFreeType/FontFreeType.o 
	g++ FontFreeType/gccDebug_GLUT/FontFreeType/FontFreeType.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccFontFreeType/FontFreeType.exe

# Compiles file ../../FontFreeType/FontFreeType.cpp for the Debug_GLUT configuration...
-include FontFreeType/gccDebug_GLUT/FontFreeType/FontFreeType.d
FontFreeType/gccDebug_GLUT/FontFreeType/FontFreeType.o: ../../FontFreeType/FontFreeType.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../FontFreeType/FontFreeType.cpp $(Debug_GLUT_Include_Path) -o FontFreeType/gccDebug_GLUT/FontFreeType/FontFreeType.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../FontFreeType/FontFreeType.cpp $(Debug_GLUT_Include_Path) > FontFreeType/gccDebug_GLUT/FontFreeType/FontFreeType.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p FontFreeType/gccDebug_GLUT/FontFreeType
	mkdir -p ../../gccFontFreeType

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f FontFreeType/gccDebug_GLUT/FontFreeType/*.o
	rm -f FontFreeType/gccDebug_GLUT/FontFreeType/*.d
	rm -f ../../gccFontFreeType/FontFreeType.exe

