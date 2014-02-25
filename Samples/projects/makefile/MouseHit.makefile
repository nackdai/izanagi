# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../Animation/MyAppl" -I"../../../External/glew/include" -I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../Libs/project/lib/gccDebug_GLUT" -L"SampleKit/gccDebug_GLUT" -L"../../../External/glew/lib" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--no-as-needed -Wl,--start-group -lSceneGraph -lShader -lMath -lGLU -lglut -lSystem -lDebugUtil -lGraph -lStd -lSampleKit -lGLEW -lGL -lX11 -lXxf86vm -lXi  -Wl,--end-group

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
Debug_GLUT: create_folders MouseHit/gccDebug_GLUT/MouseHit/main.o MouseHit/gccDebug_GLUT/MouseHit/MouseHit.o 
	g++ MouseHit/gccDebug_GLUT/MouseHit/main.o MouseHit/gccDebug_GLUT/MouseHit/MouseHit.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccMouseHit/MouseHit.exe

# Compiles file ../../MouseHit/main.cpp for the Debug_GLUT configuration...
-include MouseHit/gccDebug_GLUT/MouseHit/main.d
MouseHit/gccDebug_GLUT/MouseHit/main.o: ../../MouseHit/main.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MouseHit/main.cpp $(Debug_GLUT_Include_Path) -o MouseHit/gccDebug_GLUT/MouseHit/main.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MouseHit/main.cpp $(Debug_GLUT_Include_Path) > MouseHit/gccDebug_GLUT/MouseHit/main.d

# Compiles file ../../MouseHit/MouseHit.cpp for the Debug_GLUT configuration...
-include MouseHit/gccDebug_GLUT/MouseHit/MouseHit.d
MouseHit/gccDebug_GLUT/MouseHit/MouseHit.o: ../../MouseHit/MouseHit.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../MouseHit/MouseHit.cpp $(Debug_GLUT_Include_Path) -o MouseHit/gccDebug_GLUT/MouseHit/MouseHit.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../MouseHit/MouseHit.cpp $(Debug_GLUT_Include_Path) > MouseHit/gccDebug_GLUT/MouseHit/MouseHit.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p MouseHit/gccDebug_GLUT/MouseHit
	mkdir -p ../../gccMouseHit

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f MouseHit/gccDebug_GLUT/MouseHit/*.o
	rm -f MouseHit/gccDebug_GLUT/MouseHit/*.d
	rm -f ../../gccMouseHit/MouseHit.exe

