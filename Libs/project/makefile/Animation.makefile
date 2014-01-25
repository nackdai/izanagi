# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../include" 

# Library paths...
Debug_GLUT_Library_Path=

# Additional libraries...
Debug_GLUT_Libraries=

# Preprocessor definitions...
Debug_GLUT_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _LIB -D __IZ_DEBUG__ -D __IZ_OGL__ -D __IZ_GLUT__ -D __IZANAGI_NO_USE_D3D__ 

# Implictly linked object files...
Debug_GLUT_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLUT_Compiler_Flags=-O0 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLUT 

# Builds the Debug_GLUT configuration...
.PHONY: Debug_GLUT
Debug_GLUT: create_folders gccDebug_GLUT/source/animation/FuncCurveInterpolater.o gccDebug_GLUT/source/animation/Timeline.o gccDebug_GLUT/source/animation/TweenerInterpolater.o 
	ar rcs ../lib/gccDebug_GLUT/libAnimation.a gccDebug_GLUT/source/animation/FuncCurveInterpolater.o gccDebug_GLUT/source/animation/Timeline.o gccDebug_GLUT/source/animation/TweenerInterpolater.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/animation/FuncCurveInterpolater.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/animation/FuncCurveInterpolater.d
gccDebug_GLUT/source/animation/FuncCurveInterpolater.o: ../../source/animation/FuncCurveInterpolater.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/animation/FuncCurveInterpolater.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/animation/FuncCurveInterpolater.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/animation/FuncCurveInterpolater.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/animation/FuncCurveInterpolater.d

# Compiles file ../../source/animation/Timeline.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/animation/Timeline.d
gccDebug_GLUT/source/animation/Timeline.o: ../../source/animation/Timeline.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/animation/Timeline.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/animation/Timeline.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/animation/Timeline.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/animation/Timeline.d

# Compiles file ../../source/animation/TweenerInterpolater.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/animation/TweenerInterpolater.d
gccDebug_GLUT/source/animation/TweenerInterpolater.o: ../../source/animation/TweenerInterpolater.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/animation/TweenerInterpolater.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/animation/TweenerInterpolater.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/animation/TweenerInterpolater.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/animation/TweenerInterpolater.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/animation
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/animation/*.o
	rm -f gccDebug_GLUT/source/animation/*.d
	rm -f ../lib/gccDebug_GLUT/libAnimation.a

