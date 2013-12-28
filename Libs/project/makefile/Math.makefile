# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLES2_Include_Path=-I"../../include" 
Release_GLES2_Include_Path=-I"../../include" 

# Library paths...
Debug_GLES2_Library_Path=
Release_GLES2_Library_Path=

# Additional libraries...
Debug_GLES2_Libraries=
Release_GLES2_Libraries=

# Preprocessor definitions...
Debug_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _LIB -D __IZ_DEBUG__ -D __IZ_GLES2__ 
Release_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _LIB -D __IZ_GLES2__ 

# Implictly linked object files...
Debug_GLES2_Implicitly_Linked_Objects=
Release_GLES2_Implicitly_Linked_Objects=

# Compiler flags...
Debug_GLES2_Compiler_Flags=-O0 
Release_GLES2_Compiler_Flags=-O2 

# Builds all configurations for this project...
.PHONY: build_all_configurations
build_all_configurations: Debug_GLES2 Release_GLES2 

# Builds the Debug_GLES2 configuration...
.PHONY: Debug_GLES2
Debug_GLES2: create_folders gccDebug_GLES2/../../source/math/MathPlane.o gccDebug_GLES2/../../source/math/MathRay.o gccDebug_GLES2/../../source/math/MathRectangle.o gccDebug_GLES2/../../source/math/MathTriangle.o gccDebug_GLES2/../../source/math/MathMatrix.o gccDebug_GLES2/../../source/math/SFMT/SFMT.o gccDebug_GLES2/../../source/math/MathNumericalAnlysis.o gccDebug_GLES2/../../source/math/MathInterpolater.o gccDebug_GLES2/../../source/math/Tweener.o 
	ar rcs ../lib/gccDebug_GLES2/libMath.a gccDebug_GLES2/../../source/math/MathPlane.o gccDebug_GLES2/../../source/math/MathRay.o gccDebug_GLES2/../../source/math/MathRectangle.o gccDebug_GLES2/../../source/math/MathTriangle.o gccDebug_GLES2/../../source/math/MathMatrix.o gccDebug_GLES2/../../source/math/SFMT/SFMT.o gccDebug_GLES2/../../source/math/MathNumericalAnlysis.o gccDebug_GLES2/../../source/math/MathInterpolater.o gccDebug_GLES2/../../source/math/Tweener.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/math/MathPlane.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/math/MathPlane.d
gccDebug_GLES2/../../source/math/MathPlane.o: ../../source/math/MathPlane.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/math/MathPlane.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/math/MathPlane.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/math/MathPlane.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/math/MathPlane.d

# Compiles file ../../source/math/MathRay.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/math/MathRay.d
gccDebug_GLES2/../../source/math/MathRay.o: ../../source/math/MathRay.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/math/MathRay.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/math/MathRay.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/math/MathRay.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/math/MathRay.d

# Compiles file ../../source/math/MathRectangle.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/math/MathRectangle.d
gccDebug_GLES2/../../source/math/MathRectangle.o: ../../source/math/MathRectangle.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/math/MathRectangle.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/math/MathRectangle.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/math/MathRectangle.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/math/MathRectangle.d

# Compiles file ../../source/math/MathTriangle.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/math/MathTriangle.d
gccDebug_GLES2/../../source/math/MathTriangle.o: ../../source/math/MathTriangle.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/math/MathTriangle.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/math/MathTriangle.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/math/MathTriangle.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/math/MathTriangle.d

# Compiles file ../../source/math/MathMatrix.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/math/MathMatrix.d
gccDebug_GLES2/../../source/math/MathMatrix.o: ../../source/math/MathMatrix.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/math/MathMatrix.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/math/MathMatrix.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/math/MathMatrix.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/math/MathMatrix.d

# Compiles file ../../source/math/SFMT/SFMT.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/math/SFMT/SFMT.d
gccDebug_GLES2/../../source/math/SFMT/SFMT.o: ../../source/math/SFMT/SFMT.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/math/SFMT/SFMT.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/math/SFMT/SFMT.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/math/SFMT/SFMT.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/math/SFMT/SFMT.d

# Compiles file ../../source/math/MathNumericalAnlysis.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/math/MathNumericalAnlysis.d
gccDebug_GLES2/../../source/math/MathNumericalAnlysis.o: ../../source/math/MathNumericalAnlysis.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/math/MathNumericalAnlysis.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/math/MathNumericalAnlysis.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/math/MathNumericalAnlysis.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/math/MathNumericalAnlysis.d

# Compiles file ../../source/math/MathInterpolater.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/math/MathInterpolater.d
gccDebug_GLES2/../../source/math/MathInterpolater.o: ../../source/math/MathInterpolater.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/math/MathInterpolater.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/math/MathInterpolater.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/math/MathInterpolater.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/math/MathInterpolater.d

# Compiles file ../../source/math/Tweener.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../source/math/Tweener.d
gccDebug_GLES2/../../source/math/Tweener.o: ../../source/math/Tweener.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/math/Tweener.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../source/math/Tweener.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/math/Tweener.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../source/math/Tweener.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/../../source/math/MathPlane.o gccRelease_GLES2/../../source/math/MathRay.o gccRelease_GLES2/../../source/math/MathRectangle.o gccRelease_GLES2/../../source/math/MathTriangle.o gccRelease_GLES2/../../source/math/MathMatrix.o gccRelease_GLES2/../../source/math/SFMT/SFMT.o gccRelease_GLES2/../../source/math/MathNumericalAnlysis.o gccRelease_GLES2/../../source/math/MathInterpolater.o gccRelease_GLES2/../../source/math/Tweener.o 
	ar rcs ../lib/gccRelease_GLES2/libMath.a gccRelease_GLES2/../../source/math/MathPlane.o gccRelease_GLES2/../../source/math/MathRay.o gccRelease_GLES2/../../source/math/MathRectangle.o gccRelease_GLES2/../../source/math/MathTriangle.o gccRelease_GLES2/../../source/math/MathMatrix.o gccRelease_GLES2/../../source/math/SFMT/SFMT.o gccRelease_GLES2/../../source/math/MathNumericalAnlysis.o gccRelease_GLES2/../../source/math/MathInterpolater.o gccRelease_GLES2/../../source/math/Tweener.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/math/MathPlane.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/math/MathPlane.d
gccRelease_GLES2/../../source/math/MathPlane.o: ../../source/math/MathPlane.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/math/MathPlane.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/math/MathPlane.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/math/MathPlane.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/math/MathPlane.d

# Compiles file ../../source/math/MathRay.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/math/MathRay.d
gccRelease_GLES2/../../source/math/MathRay.o: ../../source/math/MathRay.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/math/MathRay.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/math/MathRay.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/math/MathRay.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/math/MathRay.d

# Compiles file ../../source/math/MathRectangle.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/math/MathRectangle.d
gccRelease_GLES2/../../source/math/MathRectangle.o: ../../source/math/MathRectangle.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/math/MathRectangle.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/math/MathRectangle.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/math/MathRectangle.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/math/MathRectangle.d

# Compiles file ../../source/math/MathTriangle.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/math/MathTriangle.d
gccRelease_GLES2/../../source/math/MathTriangle.o: ../../source/math/MathTriangle.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/math/MathTriangle.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/math/MathTriangle.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/math/MathTriangle.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/math/MathTriangle.d

# Compiles file ../../source/math/MathMatrix.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/math/MathMatrix.d
gccRelease_GLES2/../../source/math/MathMatrix.o: ../../source/math/MathMatrix.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/math/MathMatrix.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/math/MathMatrix.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/math/MathMatrix.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/math/MathMatrix.d

# Compiles file ../../source/math/SFMT/SFMT.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/math/SFMT/SFMT.d
gccRelease_GLES2/../../source/math/SFMT/SFMT.o: ../../source/math/SFMT/SFMT.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/math/SFMT/SFMT.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/math/SFMT/SFMT.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/math/SFMT/SFMT.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/math/SFMT/SFMT.d

# Compiles file ../../source/math/MathNumericalAnlysis.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/math/MathNumericalAnlysis.d
gccRelease_GLES2/../../source/math/MathNumericalAnlysis.o: ../../source/math/MathNumericalAnlysis.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/math/MathNumericalAnlysis.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/math/MathNumericalAnlysis.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/math/MathNumericalAnlysis.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/math/MathNumericalAnlysis.d

# Compiles file ../../source/math/MathInterpolater.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/math/MathInterpolater.d
gccRelease_GLES2/../../source/math/MathInterpolater.o: ../../source/math/MathInterpolater.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/math/MathInterpolater.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/math/MathInterpolater.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/math/MathInterpolater.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/math/MathInterpolater.d

# Compiles file ../../source/math/Tweener.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../source/math/Tweener.d
gccRelease_GLES2/../../source/math/Tweener.o: ../../source/math/Tweener.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/math/Tweener.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../source/math/Tweener.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/math/Tweener.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../source/math/Tweener.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLES2/source
	mkdir -p ../lib/gccDebug_GLES2
	mkdir -p gccRelease_GLES2/source
	mkdir -p ../lib/gccRelease_GLES2

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLES2/*.o
	rm -f gccDebug_GLES2/*.d
	rm -f ../lib/gccDebug_GLES2/*.a
	rm -f ../lib/gccDebug_GLES2/*.so
	rm -f ../lib/gccDebug_GLES2/*.dll
	rm -f ../lib/gccDebug_GLES2/*.exe
	rm -f gccRelease_GLES2/*.o
	rm -f gccRelease_GLES2/*.d
	rm -f ../lib/gccRelease_GLES2/*.a
	rm -f ../lib/gccRelease_GLES2/*.so
	rm -f ../lib/gccRelease_GLES2/*.dll
	rm -f ../lib/gccRelease_GLES2/*.exe

