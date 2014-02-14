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
Debug_GLUT: create_folders ScreenShot/gccDebug_GLUT/ScreenShot/main.o 
	g++ ScreenShot/gccDebug_GLUT/ScreenShot/main.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccScreenShot/ScreenShot.exe

# Compiles file ../../ScreenShot/main.cpp for the Debug_GLUT configuration...
-include ScreenShot/gccDebug_GLUT/ScreenShot/main.d
ScreenShot/gccDebug_GLUT/ScreenShot/main.o: ../../ScreenShot/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../ScreenShot/main.cpp $(Debug_GLUT_Include_Path) -o ScreenShot/gccDebug_GLUT/ScreenShot/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../ScreenShot/main.cpp $(Debug_GLUT_Include_Path) > ScreenShot/gccDebug_GLUT/ScreenShot/main.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p ScreenShot/gccDebug_GLUT/ScreenShot
	mkdir -p ../../gccScreenShot

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f ScreenShot/gccDebug_GLUT/ScreenShot/*.o
	rm -f ScreenShot/gccDebug_GLUT/ScreenShot/*.d
	rm -f ../../gccScreenShot/ScreenShot.exe

