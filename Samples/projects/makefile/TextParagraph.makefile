# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../External/glew/include" -I"../../../Libs/include" -I"../../SampleKit" 

# Library paths...
Debug_GLUT_Library_Path=-L"../../../External/freetype/objs/win32/gccvc2010" -L"../../../Libs/project/lib/gccDebug_GLUT" -L"../../../External/freetype/lib" -L"SampleKit/gccDebug_GLUT" -L"../../../External/glew/lib" 

# Additional libraries...
Debug_GLUT_Libraries=-Wl,--no-as-needed -Wl,--start-group -lText -lMath -lSceneGraph -lSystem -lDebugUtil -lGraph -lStd -lfreetype -lSampleKit -lglut -lGLU -lGLEW -lGL -lX11 -lXxf86vm -lXi  -Wl,--end-group

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
Debug_GLUT: create_folders TextParagraph/gccDebug_GLUT/TextParagraph/TextParagraph.o 
	g++ TextParagraph/gccDebug_GLUT/TextParagraph/TextParagraph.o  $(Debug_GLUT_Library_Path) $(Debug_GLUT_Libraries) -Wl,-rpath,./ -o ../../gccTextParagraph/TextParagraph.exe

# Compiles file ../../TextParagraph/TextParagraph.cpp for the Debug_GLUT configuration...
-include TextParagraph/gccDebug_GLUT/TextParagraph/TextParagraph.d
TextParagraph/gccDebug_GLUT/TextParagraph/TextParagraph.o: ../../TextParagraph/TextParagraph.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../TextParagraph/TextParagraph.cpp $(Debug_GLUT_Include_Path) -o TextParagraph/gccDebug_GLUT/TextParagraph/TextParagraph.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../TextParagraph/TextParagraph.cpp $(Debug_GLUT_Include_Path) > TextParagraph/gccDebug_GLUT/TextParagraph/TextParagraph.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p TextParagraph/gccDebug_GLUT/TextParagraph
	mkdir -p ../../gccTextParagraph

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f TextParagraph/gccDebug_GLUT/TextParagraph/*.o
	rm -f TextParagraph/gccDebug_GLUT/TextParagraph/*.d
	rm -f ../../gccTextParagraph/TextParagraph.exe

