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
Debug_GLUT: create_folders State/gccDebug_GLUT/State/main.o State/gccDebug_GLUT/State/StateA.o State/gccDebug_GLUT/State/StateB.o State/gccDebug_GLUT/State/StateBase.o State/gccDebug_GLUT/State/StateC.o State/gccDebug_GLUT/State/StateManager.o 
	g++ State/gccDebug_GLUT/State/main.o State/gccDebug_GLUT/State/StateA.o State/gccDebug_GLUT/State/StateB.o State/gccDebug_GLUT/State/StateBase.o State/gccDebug_GLUT/State/StateC.o State/gccDebug_GLUT/State/StateManager.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccState/State.exe

# Compiles file ../../State/main.cpp for the Debug_GLUT configuration...
-include State/gccDebug_GLUT/State/main.d
State/gccDebug_GLUT/State/main.o: ../../State/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../State/main.cpp $(Debug_GLUT_Include_Path) -o State/gccDebug_GLUT/State/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../State/main.cpp $(Debug_GLUT_Include_Path) > State/gccDebug_GLUT/State/main.d

# Compiles file ../../State/StateA.cpp for the Debug_GLUT configuration...
-include State/gccDebug_GLUT/State/StateA.d
State/gccDebug_GLUT/State/StateA.o: ../../State/StateA.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../State/StateA.cpp $(Debug_GLUT_Include_Path) -o State/gccDebug_GLUT/State/StateA.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../State/StateA.cpp $(Debug_GLUT_Include_Path) > State/gccDebug_GLUT/State/StateA.d

# Compiles file ../../State/StateB.cpp for the Debug_GLUT configuration...
-include State/gccDebug_GLUT/State/StateB.d
State/gccDebug_GLUT/State/StateB.o: ../../State/StateB.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../State/StateB.cpp $(Debug_GLUT_Include_Path) -o State/gccDebug_GLUT/State/StateB.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../State/StateB.cpp $(Debug_GLUT_Include_Path) > State/gccDebug_GLUT/State/StateB.d

# Compiles file ../../State/StateBase.cpp for the Debug_GLUT configuration...
-include State/gccDebug_GLUT/State/StateBase.d
State/gccDebug_GLUT/State/StateBase.o: ../../State/StateBase.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../State/StateBase.cpp $(Debug_GLUT_Include_Path) -o State/gccDebug_GLUT/State/StateBase.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../State/StateBase.cpp $(Debug_GLUT_Include_Path) > State/gccDebug_GLUT/State/StateBase.d

# Compiles file ../../State/StateC.cpp for the Debug_GLUT configuration...
-include State/gccDebug_GLUT/State/StateC.d
State/gccDebug_GLUT/State/StateC.o: ../../State/StateC.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../State/StateC.cpp $(Debug_GLUT_Include_Path) -o State/gccDebug_GLUT/State/StateC.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../State/StateC.cpp $(Debug_GLUT_Include_Path) > State/gccDebug_GLUT/State/StateC.d

# Compiles file ../../State/StateManager.cpp for the Debug_GLUT configuration...
-include State/gccDebug_GLUT/State/StateManager.d
State/gccDebug_GLUT/State/StateManager.o: ../../State/StateManager.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../State/StateManager.cpp $(Debug_GLUT_Include_Path) -o State/gccDebug_GLUT/State/StateManager.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../State/StateManager.cpp $(Debug_GLUT_Include_Path) > State/gccDebug_GLUT/State/StateManager.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p State/gccDebug_GLUT/State
	mkdir -p ../../gccState

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f State/gccDebug_GLUT/State/*.o
	rm -f State/gccDebug_GLUT/State/*.d
	rm -f ../../gccState/State.exe

