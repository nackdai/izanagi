# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -lShader  -Wl,--end-group

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
Debug_GLUT: create_folders Material/gccDebug_GLUT/Material/main.o Material/gccDebug_GLUT/Material/MaterialApp.o 
	g++ Material/gccDebug_GLUT/Material/main.o Material/gccDebug_GLUT/Material/MaterialApp.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccMaterial/Material.exe

# Compiles file ../../Material/main.cpp for the Debug_GLUT configuration...
-include Material/gccDebug_GLUT/Material/main.d
Material/gccDebug_GLUT/Material/main.o: ../../Material/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Material/main.cpp $(Debug_GLUT_Include_Path) -o Material/gccDebug_GLUT/Material/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Material/main.cpp $(Debug_GLUT_Include_Path) > Material/gccDebug_GLUT/Material/main.d

# Compiles file ../../Material/MaterialApp.cpp for the Debug_GLUT configuration...
-include Material/gccDebug_GLUT/Material/MaterialApp.d
Material/gccDebug_GLUT/Material/MaterialApp.o: ../../Material/MaterialApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Material/MaterialApp.cpp $(Debug_GLUT_Include_Path) -o Material/gccDebug_GLUT/Material/MaterialApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Material/MaterialApp.cpp $(Debug_GLUT_Include_Path) > Material/gccDebug_GLUT/Material/MaterialApp.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Material/gccDebug_GLUT/Material
	mkdir -p ../../gccMaterial

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Material/gccDebug_GLUT/Material/*.o
	rm -f Material/gccDebug_GLUT/Material/*.d
	rm -f ../../gccMaterial/Material.exe

