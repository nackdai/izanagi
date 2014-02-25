# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../../External/freetype/include" -I"../../include" -I"../../../External/glew/include" -I"../../../External/freeglut/include" 

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
Debug_GLUT: create_folders gccDebug_GLUT/source/text/FontGlyphCache.o gccDebug_GLUT/source/text/FontGlyphCacheImpl.o gccDebug_GLUT/source/text/FontMap.o gccDebug_GLUT/source/text/FontRenderer.o gccDebug_GLUT/source/text/UString.o gccDebug_GLUT/source/text/FontHostFNT.o gccDebug_GLUT/source/text/FontHostFT.o gccDebug_GLUT/source/text/Paragraph.o gccDebug_GLUT/source/text/ParagraphGroup.o gccDebug_GLUT/source/text/TextLine.o gccDebug_GLUT/source/text/ParagraphGroupImpl.o gccDebug_GLUT/source/text/ParagraphImpl.o gccDebug_GLUT/source/text/TextLineImpl.o 
	ar rcs ../lib/gccDebug_GLUT/libText.a gccDebug_GLUT/source/text/FontGlyphCache.o gccDebug_GLUT/source/text/FontGlyphCacheImpl.o gccDebug_GLUT/source/text/FontMap.o gccDebug_GLUT/source/text/FontRenderer.o gccDebug_GLUT/source/text/UString.o gccDebug_GLUT/source/text/FontHostFNT.o gccDebug_GLUT/source/text/FontHostFT.o gccDebug_GLUT/source/text/Paragraph.o gccDebug_GLUT/source/text/ParagraphGroup.o gccDebug_GLUT/source/text/TextLine.o gccDebug_GLUT/source/text/ParagraphGroupImpl.o gccDebug_GLUT/source/text/ParagraphImpl.o gccDebug_GLUT/source/text/TextLineImpl.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/text/FontGlyphCache.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/FontGlyphCache.d
gccDebug_GLUT/source/text/FontGlyphCache.o: ../../source/text/FontGlyphCache.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/FontGlyphCache.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/FontGlyphCache.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/FontGlyphCache.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/FontGlyphCache.d

# Compiles file ../../source/text/FontGlyphCacheImpl.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/FontGlyphCacheImpl.d
gccDebug_GLUT/source/text/FontGlyphCacheImpl.o: ../../source/text/FontGlyphCacheImpl.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/FontGlyphCacheImpl.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/FontGlyphCacheImpl.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/FontGlyphCacheImpl.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/FontGlyphCacheImpl.d

# Compiles file ../../source/text/FontMap.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/FontMap.d
gccDebug_GLUT/source/text/FontMap.o: ../../source/text/FontMap.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/FontMap.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/FontMap.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/FontMap.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/FontMap.d

# Compiles file ../../source/text/FontRenderer.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/FontRenderer.d
gccDebug_GLUT/source/text/FontRenderer.o: ../../source/text/FontRenderer.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/FontRenderer.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/FontRenderer.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/FontRenderer.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/FontRenderer.d

# Compiles file ../../source/text/UString.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/UString.d
gccDebug_GLUT/source/text/UString.o: ../../source/text/UString.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/UString.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/UString.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/UString.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/UString.d

# Compiles file ../../source/text/FontHostFNT.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/FontHostFNT.d
gccDebug_GLUT/source/text/FontHostFNT.o: ../../source/text/FontHostFNT.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/FontHostFNT.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/FontHostFNT.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/FontHostFNT.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/FontHostFNT.d

# Compiles file ../../source/text/FontHostFT.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/FontHostFT.d
gccDebug_GLUT/source/text/FontHostFT.o: ../../source/text/FontHostFT.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/FontHostFT.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/FontHostFT.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/FontHostFT.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/FontHostFT.d

# Compiles file ../../source/text/Paragraph.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/Paragraph.d
gccDebug_GLUT/source/text/Paragraph.o: ../../source/text/Paragraph.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/Paragraph.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/Paragraph.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/Paragraph.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/Paragraph.d

# Compiles file ../../source/text/ParagraphGroup.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/ParagraphGroup.d
gccDebug_GLUT/source/text/ParagraphGroup.o: ../../source/text/ParagraphGroup.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/ParagraphGroup.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/ParagraphGroup.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/ParagraphGroup.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/ParagraphGroup.d

# Compiles file ../../source/text/TextLine.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/TextLine.d
gccDebug_GLUT/source/text/TextLine.o: ../../source/text/TextLine.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/TextLine.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/TextLine.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/TextLine.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/TextLine.d

# Compiles file ../../source/text/ParagraphGroupImpl.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/ParagraphGroupImpl.d
gccDebug_GLUT/source/text/ParagraphGroupImpl.o: ../../source/text/ParagraphGroupImpl.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/ParagraphGroupImpl.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/ParagraphGroupImpl.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/ParagraphGroupImpl.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/ParagraphGroupImpl.d

# Compiles file ../../source/text/ParagraphImpl.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/ParagraphImpl.d
gccDebug_GLUT/source/text/ParagraphImpl.o: ../../source/text/ParagraphImpl.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/ParagraphImpl.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/ParagraphImpl.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/ParagraphImpl.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/ParagraphImpl.d

# Compiles file ../../source/text/TextLineImpl.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/text/TextLineImpl.d
gccDebug_GLUT/source/text/TextLineImpl.o: ../../source/text/TextLineImpl.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/text/TextLineImpl.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/text/TextLineImpl.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/text/TextLineImpl.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/text/TextLineImpl.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/text
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/text/*.o
	rm -f gccDebug_GLUT/source/text/*.d
	rm -f ../lib/gccDebug_GLUT/libText.a

