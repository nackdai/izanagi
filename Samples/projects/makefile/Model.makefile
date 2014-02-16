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
Debug_GLUT: create_folders Model/gccDebug_GLUT/Model/main.o Model/gccDebug_GLUT/Model/ModelApp.o Model/gccDebug_GLUT/Model/StateBase.o Model/gccDebug_GLUT/Model/StateCollada.o Model/gccDebug_GLUT/Model/StateManager.o Model/gccDebug_GLUT/Model/StateXFile.o 
	g++ Model/gccDebug_GLUT/Model/main.o Model/gccDebug_GLUT/Model/ModelApp.o Model/gccDebug_GLUT/Model/StateBase.o Model/gccDebug_GLUT/Model/StateCollada.o Model/gccDebug_GLUT/Model/StateManager.o Model/gccDebug_GLUT/Model/StateXFile.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccModel/Model.exe

# Compiles file ../../Model/main.cpp for the Debug_GLUT configuration...
-include Model/gccDebug_GLUT/Model/main.d
Model/gccDebug_GLUT/Model/main.o: ../../Model/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Model/main.cpp $(Debug_GLUT_Include_Path) -o Model/gccDebug_GLUT/Model/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Model/main.cpp $(Debug_GLUT_Include_Path) > Model/gccDebug_GLUT/Model/main.d

# Compiles file ../../Model/ModelApp.cpp for the Debug_GLUT configuration...
-include Model/gccDebug_GLUT/Model/ModelApp.d
Model/gccDebug_GLUT/Model/ModelApp.o: ../../Model/ModelApp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Model/ModelApp.cpp $(Debug_GLUT_Include_Path) -o Model/gccDebug_GLUT/Model/ModelApp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Model/ModelApp.cpp $(Debug_GLUT_Include_Path) > Model/gccDebug_GLUT/Model/ModelApp.d

# Compiles file ../../Model/StateBase.cpp for the Debug_GLUT configuration...
-include Model/gccDebug_GLUT/Model/StateBase.d
Model/gccDebug_GLUT/Model/StateBase.o: ../../Model/StateBase.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Model/StateBase.cpp $(Debug_GLUT_Include_Path) -o Model/gccDebug_GLUT/Model/StateBase.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Model/StateBase.cpp $(Debug_GLUT_Include_Path) > Model/gccDebug_GLUT/Model/StateBase.d

# Compiles file ../../Model/StateCollada.cpp for the Debug_GLUT configuration...
-include Model/gccDebug_GLUT/Model/StateCollada.d
Model/gccDebug_GLUT/Model/StateCollada.o: ../../Model/StateCollada.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Model/StateCollada.cpp $(Debug_GLUT_Include_Path) -o Model/gccDebug_GLUT/Model/StateCollada.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Model/StateCollada.cpp $(Debug_GLUT_Include_Path) > Model/gccDebug_GLUT/Model/StateCollada.d

# Compiles file ../../Model/StateManager.cpp for the Debug_GLUT configuration...
-include Model/gccDebug_GLUT/Model/StateManager.d
Model/gccDebug_GLUT/Model/StateManager.o: ../../Model/StateManager.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Model/StateManager.cpp $(Debug_GLUT_Include_Path) -o Model/gccDebug_GLUT/Model/StateManager.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Model/StateManager.cpp $(Debug_GLUT_Include_Path) > Model/gccDebug_GLUT/Model/StateManager.d

# Compiles file ../../Model/StateXFile.cpp for the Debug_GLUT configuration...
-include Model/gccDebug_GLUT/Model/StateXFile.d
Model/gccDebug_GLUT/Model/StateXFile.o: ../../Model/StateXFile.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Model/StateXFile.cpp $(Debug_GLUT_Include_Path) -o Model/gccDebug_GLUT/Model/StateXFile.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Model/StateXFile.cpp $(Debug_GLUT_Include_Path) > Model/gccDebug_GLUT/Model/StateXFile.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Model/gccDebug_GLUT/Model
	mkdir -p ../../gccModel

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Model/gccDebug_GLUT/Model/*.o
	rm -f Model/gccDebug_GLUT/Model/*.d
	rm -f ../../gccModel/Model.exe

