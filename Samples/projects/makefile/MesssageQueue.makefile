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
Debug_GLUT: create_folders MesssageQueue/gccDebug_GLUT/MessageQueue/main.o 
	g++ MesssageQueue/gccDebug_GLUT/MessageQueue/main.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccMesssageQueue/MesssageQueue.exe

# Compiles file ../../MessageQueue/main.cpp for the Debug_GLUT configuration...
-include MesssageQueue/gccDebug_GLUT/MessageQueue/main.d
MesssageQueue/gccDebug_GLUT/MessageQueue/main.o: ../../MessageQueue/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MessageQueue/main.cpp $(Debug_GLUT_Include_Path) -o MesssageQueue/gccDebug_GLUT/MessageQueue/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MessageQueue/main.cpp $(Debug_GLUT_Include_Path) > MesssageQueue/gccDebug_GLUT/MessageQueue/main.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p MesssageQueue/gccDebug_GLUT/MessageQueue
	mkdir -p ../../gccMesssageQueue

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f MesssageQueue/gccDebug_GLUT/MessageQueue/*.o
	rm -f MesssageQueue/gccDebug_GLUT/MessageQueue/*.d
	rm -f ../../gccMesssageQueue/MesssageQueue.exe

