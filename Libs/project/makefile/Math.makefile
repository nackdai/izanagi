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
Debug_GLUT: create_folders gccDebug_GLUT/source/math/MathPlane.o gccDebug_GLUT/source/math/MathRay.o gccDebug_GLUT/source/math/MathRectangle.o gccDebug_GLUT/source/math/MathTriangle.o gccDebug_GLUT/source/math/MathMatrix.o gccDebug_GLUT/source/math/SFMT/SFMT.o gccDebug_GLUT/source/math/MathNumericalAnlysis.o gccDebug_GLUT/source/math/MathInterpolater.o gccDebug_GLUT/source/math/Tweener.o 
	ar rcs ../lib/gccDebug_GLUT/libMath.a gccDebug_GLUT/source/math/MathPlane.o gccDebug_GLUT/source/math/MathRay.o gccDebug_GLUT/source/math/MathRectangle.o gccDebug_GLUT/source/math/MathTriangle.o gccDebug_GLUT/source/math/MathMatrix.o gccDebug_GLUT/source/math/SFMT/SFMT.o gccDebug_GLUT/source/math/MathNumericalAnlysis.o gccDebug_GLUT/source/math/MathInterpolater.o gccDebug_GLUT/source/math/Tweener.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/math/MathPlane.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/math/MathPlane.d
gccDebug_GLUT/source/math/MathPlane.o: ../../source/math/MathPlane.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/math/MathPlane.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/math/MathPlane.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/math/MathPlane.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/math/MathPlane.d

# Compiles file ../../source/math/MathRay.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/math/MathRay.d
gccDebug_GLUT/source/math/MathRay.o: ../../source/math/MathRay.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/math/MathRay.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/math/MathRay.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/math/MathRay.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/math/MathRay.d

# Compiles file ../../source/math/MathRectangle.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/math/MathRectangle.d
gccDebug_GLUT/source/math/MathRectangle.o: ../../source/math/MathRectangle.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/math/MathRectangle.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/math/MathRectangle.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/math/MathRectangle.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/math/MathRectangle.d

# Compiles file ../../source/math/MathTriangle.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/math/MathTriangle.d
gccDebug_GLUT/source/math/MathTriangle.o: ../../source/math/MathTriangle.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/math/MathTriangle.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/math/MathTriangle.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/math/MathTriangle.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/math/MathTriangle.d

# Compiles file ../../source/math/MathMatrix.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/math/MathMatrix.d
gccDebug_GLUT/source/math/MathMatrix.o: ../../source/math/MathMatrix.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/math/MathMatrix.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/math/MathMatrix.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/math/MathMatrix.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/math/MathMatrix.d

# Compiles file ../../source/math/SFMT/SFMT.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/math/SFMT/SFMT.d
gccDebug_GLUT/source/math/SFMT/SFMT.o: ../../source/math/SFMT/SFMT.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/math/SFMT/SFMT.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/math/SFMT/SFMT.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/math/SFMT/SFMT.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/math/SFMT/SFMT.d

# Compiles file ../../source/math/MathNumericalAnlysis.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/math/MathNumericalAnlysis.d
gccDebug_GLUT/source/math/MathNumericalAnlysis.o: ../../source/math/MathNumericalAnlysis.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/math/MathNumericalAnlysis.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/math/MathNumericalAnlysis.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/math/MathNumericalAnlysis.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/math/MathNumericalAnlysis.d

# Compiles file ../../source/math/MathInterpolater.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/math/MathInterpolater.d
gccDebug_GLUT/source/math/MathInterpolater.o: ../../source/math/MathInterpolater.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/math/MathInterpolater.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/math/MathInterpolater.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/math/MathInterpolater.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/math/MathInterpolater.d

# Compiles file ../../source/math/Tweener.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/math/Tweener.d
gccDebug_GLUT/source/math/Tweener.o: ../../source/math/Tweener.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/math/Tweener.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/math/Tweener.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/math/Tweener.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/math/Tweener.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/math
	mkdir -p gccDebug_GLUT/source/math/SFMT
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/math/*.o
	rm -f gccDebug_GLUT/source/math/*.d
	rm -f gccDebug_GLUT/source/math/SFMT/*.o
	rm -f gccDebug_GLUT/source/math/SFMT/*.d
	rm -f ../lib/gccDebug_GLUT/libMath.a

