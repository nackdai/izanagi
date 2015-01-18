# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../zlib" -I"../../include" -I"../../../External/glew/include" -I"../../../External/freeglut/include" 

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
Debug_GLUT: create_folders gccDebug_GLUT/source/resource/Resource.o gccDebug_GLUT/source/resource/ResourceManager.o gccDebug_GLUT/source/resource/archive/Archive.o gccDebug_GLUT/source/resource/archive/ArchiveTarball.o 
	ar rcs ../lib/gccDebug_GLUT/libResource.a gccDebug_GLUT/source/resource/Resource.o gccDebug_GLUT/source/resource/ResourceManager.o gccDebug_GLUT/source/resource/archive/Archive.o gccDebug_GLUT/source/resource/archive/ArchiveTarball.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/resource/Resource.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/resource/Resource.d
gccDebug_GLUT/source/resource/Resource.o: ../../source/resource/Resource.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/resource/Resource.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/resource/Resource.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/resource/Resource.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/resource/Resource.d

# Compiles file ../../source/resource/ResourceManager.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/resource/ResourceManager.d
gccDebug_GLUT/source/resource/ResourceManager.o: ../../source/resource/ResourceManager.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/resource/ResourceManager.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/resource/ResourceManager.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/resource/ResourceManager.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/resource/ResourceManager.d

# Compiles file ../../source/resource/archive/Archive.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/resource/archive/Archive.d
gccDebug_GLUT/source/resource/archive/Archive.o: ../../source/resource/archive/Archive.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/resource/archive/Archive.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/resource/archive/Archive.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/resource/archive/Archive.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/resource/archive/Archive.d

# Compiles file ../../source/resource/archive/ArchiveTarball.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/resource/archive/ArchiveTarball.d
gccDebug_GLUT/source/resource/archive/ArchiveTarball.o: ../../source/resource/archive/ArchiveTarball.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/resource/archive/ArchiveTarball.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/resource/archive/ArchiveTarball.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/resource/archive/ArchiveTarball.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/resource/archive/ArchiveTarball.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/resource
	mkdir -p gccDebug_GLUT/source/resource/archive
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/resource/*.o
	rm -f gccDebug_GLUT/source/resource/*.d
	rm -f gccDebug_GLUT/source/resource/archive/*.o
	rm -f gccDebug_GLUT/source/resource/archive/*.d
	rm -f ../lib/gccDebug_GLUT/libResource.a

