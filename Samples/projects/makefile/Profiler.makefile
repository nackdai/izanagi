# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--start-group -lSystem  -Wl,--end-group

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
Debug_GLUT: create_folders Profiler/gccDebug_GLUT/Profiler/main.o 
	g++ Profiler/gccDebug_GLUT/Profiler/main.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccProfiler/Profiler.exe

# Compiles file ../../Profiler/main.cpp for the Debug_GLUT configuration...
-include Profiler/gccDebug_GLUT/Profiler/main.d
Profiler/gccDebug_GLUT/Profiler/main.o: ../../Profiler/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../Profiler/main.cpp $(Debug_GLUT_Include_Path) -o Profiler/gccDebug_GLUT/Profiler/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../Profiler/main.cpp $(Debug_GLUT_Include_Path) > Profiler/gccDebug_GLUT/Profiler/main.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p Profiler/gccDebug_GLUT/Profiler
	mkdir -p ../../gccProfiler

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f Profiler/gccDebug_GLUT/Profiler/*.o
	rm -f Profiler/gccDebug_GLUT/Profiler/*.d
	rm -f ../../gccProfiler/Profiler.exe

