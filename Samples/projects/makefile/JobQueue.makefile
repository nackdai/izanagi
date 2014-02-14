# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -lThreadModel  -Wl,--end-group

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
Debug_GLUT: create_folders JobQueue/gccDebug_GLUT/JobQueue/main.o 
	g++ JobQueue/gccDebug_GLUT/JobQueue/main.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccJobQueue/JobQueue.exe

# Compiles file ../../JobQueue/main.cpp for the Debug_GLUT configuration...
-include JobQueue/gccDebug_GLUT/JobQueue/main.d
JobQueue/gccDebug_GLUT/JobQueue/main.o: ../../JobQueue/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../JobQueue/main.cpp $(Debug_GLUT_Include_Path) -o JobQueue/gccDebug_GLUT/JobQueue/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../JobQueue/main.cpp $(Debug_GLUT_Include_Path) > JobQueue/gccDebug_GLUT/JobQueue/main.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p JobQueue/gccDebug_GLUT/JobQueue
	mkdir -p ../../gccJobQueue

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f JobQueue/gccDebug_GLUT/JobQueue/*.o
	rm -f JobQueue/gccDebug_GLUT/JobQueue/*.d
	rm -f ../../gccJobQueue/JobQueue.exe

