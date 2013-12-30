# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLES2_Include_Path=-I"../../../External/freetype/include" -I"../../include" 
Release_GLES2_Include_Path=-I"../../../External/freetype/include" -I"../../include" 

# Library paths...
Debug_GLES2_Library_Path=
Release_GLES2_Library_Path=

# Additional libraries...
Debug_GLES2_Libraries=
Release_GLES2_Libraries=

# Preprocessor definitions...
Debug_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D _DEBUG -D _LIB -D __IZ_DEBUG__ -D __IZ_GLES2__ -D __IZANAGI_NO_USE_D3D__ 
Release_GLES2_Preprocessor_Definitions=-D GCC_BUILD -D NDEBUG -D _LIB -D __IZ_GLES2__ -D __IZANAGI_NO_USE_D3D__ 

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
Debug_GLES2: create_folders gccDebug_GLES2/source/text/FontGlyphCache.o gccDebug_GLES2/source/text/FontGlyphCacheImpl.o gccDebug_GLES2/source/text/FontMap.o gccDebug_GLES2/source/text/FontRenderer.o gccDebug_GLES2/source/text/UString.o gccDebug_GLES2/source/text/FontHostFNT.o gccDebug_GLES2/source/text/FontHostFT.o gccDebug_GLES2/source/text/Paragraph.o gccDebug_GLES2/source/text/ParagraphGroup.o gccDebug_GLES2/source/text/TextLine.o gccDebug_GLES2/source/text/ParagraphGroupImpl.o gccDebug_GLES2/source/text/ParagraphImpl.o gccDebug_GLES2/source/text/TextLineImpl.o 
	ar rcs ../lib/gccDebug_GLES2/libText.a gccDebug_GLES2/source/text/FontGlyphCache.o gccDebug_GLES2/source/text/FontGlyphCacheImpl.o gccDebug_GLES2/source/text/FontMap.o gccDebug_GLES2/source/text/FontRenderer.o gccDebug_GLES2/source/text/UString.o gccDebug_GLES2/source/text/FontHostFNT.o gccDebug_GLES2/source/text/FontHostFT.o gccDebug_GLES2/source/text/Paragraph.o gccDebug_GLES2/source/text/ParagraphGroup.o gccDebug_GLES2/source/text/TextLine.o gccDebug_GLES2/source/text/ParagraphGroupImpl.o gccDebug_GLES2/source/text/ParagraphImpl.o gccDebug_GLES2/source/text/TextLineImpl.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/text/FontGlyphCache.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/FontGlyphCache.d
gccDebug_GLES2/source/text/FontGlyphCache.o: ../../source/text/FontGlyphCache.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/FontGlyphCache.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/FontGlyphCache.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/FontGlyphCache.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/FontGlyphCache.d

# Compiles file ../../source/text/FontGlyphCacheImpl.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/FontGlyphCacheImpl.d
gccDebug_GLES2/source/text/FontGlyphCacheImpl.o: ../../source/text/FontGlyphCacheImpl.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/FontGlyphCacheImpl.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/FontGlyphCacheImpl.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/FontGlyphCacheImpl.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/FontGlyphCacheImpl.d

# Compiles file ../../source/text/FontMap.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/FontMap.d
gccDebug_GLES2/source/text/FontMap.o: ../../source/text/FontMap.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/FontMap.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/FontMap.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/FontMap.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/FontMap.d

# Compiles file ../../source/text/FontRenderer.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/FontRenderer.d
gccDebug_GLES2/source/text/FontRenderer.o: ../../source/text/FontRenderer.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/FontRenderer.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/FontRenderer.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/FontRenderer.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/FontRenderer.d

# Compiles file ../../source/text/UString.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/UString.d
gccDebug_GLES2/source/text/UString.o: ../../source/text/UString.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/UString.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/UString.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/UString.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/UString.d

# Compiles file ../../source/text/FontHostFNT.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/FontHostFNT.d
gccDebug_GLES2/source/text/FontHostFNT.o: ../../source/text/FontHostFNT.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/FontHostFNT.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/FontHostFNT.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/FontHostFNT.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/FontHostFNT.d

# Compiles file ../../source/text/FontHostFT.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/FontHostFT.d
gccDebug_GLES2/source/text/FontHostFT.o: ../../source/text/FontHostFT.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/FontHostFT.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/FontHostFT.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/FontHostFT.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/FontHostFT.d

# Compiles file ../../source/text/Paragraph.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/Paragraph.d
gccDebug_GLES2/source/text/Paragraph.o: ../../source/text/Paragraph.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/Paragraph.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/Paragraph.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/Paragraph.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/Paragraph.d

# Compiles file ../../source/text/ParagraphGroup.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/ParagraphGroup.d
gccDebug_GLES2/source/text/ParagraphGroup.o: ../../source/text/ParagraphGroup.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/ParagraphGroup.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/ParagraphGroup.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/ParagraphGroup.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/ParagraphGroup.d

# Compiles file ../../source/text/TextLine.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/TextLine.d
gccDebug_GLES2/source/text/TextLine.o: ../../source/text/TextLine.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/TextLine.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/TextLine.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/TextLine.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/TextLine.d

# Compiles file ../../source/text/ParagraphGroupImpl.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/ParagraphGroupImpl.d
gccDebug_GLES2/source/text/ParagraphGroupImpl.o: ../../source/text/ParagraphGroupImpl.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/ParagraphGroupImpl.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/ParagraphGroupImpl.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/ParagraphGroupImpl.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/ParagraphGroupImpl.d

# Compiles file ../../source/text/ParagraphImpl.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/ParagraphImpl.d
gccDebug_GLES2/source/text/ParagraphImpl.o: ../../source/text/ParagraphImpl.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/ParagraphImpl.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/ParagraphImpl.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/ParagraphImpl.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/ParagraphImpl.d

# Compiles file ../../source/text/TextLineImpl.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/text/TextLineImpl.d
gccDebug_GLES2/source/text/TextLineImpl.o: ../../source/text/TextLineImpl.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/text/TextLineImpl.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/text/TextLineImpl.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/text/TextLineImpl.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/text/TextLineImpl.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/source/text/FontGlyphCache.o gccRelease_GLES2/source/text/FontGlyphCacheImpl.o gccRelease_GLES2/source/text/FontMap.o gccRelease_GLES2/source/text/FontRenderer.o gccRelease_GLES2/source/text/UString.o gccRelease_GLES2/source/text/FontHostFNT.o gccRelease_GLES2/source/text/FontHostFT.o gccRelease_GLES2/source/text/Paragraph.o gccRelease_GLES2/source/text/ParagraphGroup.o gccRelease_GLES2/source/text/TextLine.o gccRelease_GLES2/source/text/ParagraphGroupImpl.o gccRelease_GLES2/source/text/ParagraphImpl.o gccRelease_GLES2/source/text/TextLineImpl.o 
	ar rcs ../lib/gccRelease_GLES2/libText.a gccRelease_GLES2/source/text/FontGlyphCache.o gccRelease_GLES2/source/text/FontGlyphCacheImpl.o gccRelease_GLES2/source/text/FontMap.o gccRelease_GLES2/source/text/FontRenderer.o gccRelease_GLES2/source/text/UString.o gccRelease_GLES2/source/text/FontHostFNT.o gccRelease_GLES2/source/text/FontHostFT.o gccRelease_GLES2/source/text/Paragraph.o gccRelease_GLES2/source/text/ParagraphGroup.o gccRelease_GLES2/source/text/TextLine.o gccRelease_GLES2/source/text/ParagraphGroupImpl.o gccRelease_GLES2/source/text/ParagraphImpl.o gccRelease_GLES2/source/text/TextLineImpl.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/text/FontGlyphCache.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/FontGlyphCache.d
gccRelease_GLES2/source/text/FontGlyphCache.o: ../../source/text/FontGlyphCache.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/FontGlyphCache.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/FontGlyphCache.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/FontGlyphCache.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/FontGlyphCache.d

# Compiles file ../../source/text/FontGlyphCacheImpl.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/FontGlyphCacheImpl.d
gccRelease_GLES2/source/text/FontGlyphCacheImpl.o: ../../source/text/FontGlyphCacheImpl.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/FontGlyphCacheImpl.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/FontGlyphCacheImpl.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/FontGlyphCacheImpl.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/FontGlyphCacheImpl.d

# Compiles file ../../source/text/FontMap.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/FontMap.d
gccRelease_GLES2/source/text/FontMap.o: ../../source/text/FontMap.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/FontMap.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/FontMap.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/FontMap.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/FontMap.d

# Compiles file ../../source/text/FontRenderer.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/FontRenderer.d
gccRelease_GLES2/source/text/FontRenderer.o: ../../source/text/FontRenderer.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/FontRenderer.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/FontRenderer.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/FontRenderer.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/FontRenderer.d

# Compiles file ../../source/text/UString.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/UString.d
gccRelease_GLES2/source/text/UString.o: ../../source/text/UString.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/UString.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/UString.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/UString.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/UString.d

# Compiles file ../../source/text/FontHostFNT.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/FontHostFNT.d
gccRelease_GLES2/source/text/FontHostFNT.o: ../../source/text/FontHostFNT.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/FontHostFNT.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/FontHostFNT.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/FontHostFNT.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/FontHostFNT.d

# Compiles file ../../source/text/FontHostFT.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/FontHostFT.d
gccRelease_GLES2/source/text/FontHostFT.o: ../../source/text/FontHostFT.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/FontHostFT.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/FontHostFT.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/FontHostFT.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/FontHostFT.d

# Compiles file ../../source/text/Paragraph.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/Paragraph.d
gccRelease_GLES2/source/text/Paragraph.o: ../../source/text/Paragraph.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/Paragraph.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/Paragraph.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/Paragraph.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/Paragraph.d

# Compiles file ../../source/text/ParagraphGroup.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/ParagraphGroup.d
gccRelease_GLES2/source/text/ParagraphGroup.o: ../../source/text/ParagraphGroup.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/ParagraphGroup.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/ParagraphGroup.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/ParagraphGroup.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/ParagraphGroup.d

# Compiles file ../../source/text/TextLine.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/TextLine.d
gccRelease_GLES2/source/text/TextLine.o: ../../source/text/TextLine.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/TextLine.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/TextLine.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/TextLine.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/TextLine.d

# Compiles file ../../source/text/ParagraphGroupImpl.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/ParagraphGroupImpl.d
gccRelease_GLES2/source/text/ParagraphGroupImpl.o: ../../source/text/ParagraphGroupImpl.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/ParagraphGroupImpl.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/ParagraphGroupImpl.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/ParagraphGroupImpl.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/ParagraphGroupImpl.d

# Compiles file ../../source/text/ParagraphImpl.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/ParagraphImpl.d
gccRelease_GLES2/source/text/ParagraphImpl.o: ../../source/text/ParagraphImpl.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/ParagraphImpl.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/ParagraphImpl.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/ParagraphImpl.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/ParagraphImpl.d

# Compiles file ../../source/text/TextLineImpl.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/text/TextLineImpl.d
gccRelease_GLES2/source/text/TextLineImpl.o: ../../source/text/TextLineImpl.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/text/TextLineImpl.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/text/TextLineImpl.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/text/TextLineImpl.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/text/TextLineImpl.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLES2/source/text
	mkdir -p ../lib/gccDebug_GLES2
	mkdir -p gccRelease_GLES2/source/text
	mkdir -p ../lib/gccRelease_GLES2

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLES2/source/text/*.o
	rm -f gccDebug_GLES2/source/text/*.d
	rm -f ../lib/gccDebug_GLES2/libText.a
	rm -f gccRelease_GLES2/source/text/*.o
	rm -f gccRelease_GLES2/source/text/*.d
	rm -f ../lib/gccRelease_GLES2/libText.a

