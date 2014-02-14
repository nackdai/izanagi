# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=

# Additional libraries...
Debug_GLUT_Libraries=

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
Debug_GLUT: create_folders Render2D/gccDebug_GLUT/Render2D/Render2D.o 
	g++ Render2D/gccDebug_GLUT/Render2D/Render2D.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccRender2D/Render2D.exe

# Compiles file ../../Render2D/Render2D.cpp for the Debug_GLUT configuration...
-include Render2D/gccDebug_GLUT/Render2D/Render2D.d
Render2D/gccDebug_GLUT/Render2D/Render2D.o: ../../Render2D/Render2D.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Render2D/Render2D.cpp $(Debug_GLUT_Include_Path) -o Render2D/gccDebug_GLUT/Render2D/Render2D.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Render2D/Render2D.cpp $(Debug_GLUT_Include_Path) > Render2D/gccDebug_GLUT/Render2D/Render2D.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Render2D/gccDebug_GLUT/Render2D
	mkdir -p ../../gccRender2D

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Render2D/gccDebug_GLUT/Render2D/*.o
	rm -f Render2D/gccDebug_GLUT/Render2D/*.d
	rm -f ../../gccRender2D/Render2D.exe

