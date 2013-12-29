# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLES2_Include_Path=-I"../../zlib" -I"../../include" 
Release_GLES2_Include_Path=-I"../../zlib" -I"../../include" 

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
Debug_GLES2: create_folders gccDebug_GLES2/../../../source/resource/Resource.o gccDebug_GLES2/../../../source/resource/ResourceManager.o gccDebug_GLES2/../../../source/resource/archive/Archive.o 
	ar rcs ../lib/gccDebug_GLES2/libResource.a gccDebug_GLES2/../../../source/resource/Resource.o gccDebug_GLES2/../../../source/resource/ResourceManager.o gccDebug_GLES2/../../../source/resource/archive/Archive.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/resource/Resource.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/resource/Resource.d
gccDebug_GLES2/../../../source/resource/Resource.o: ../../source/resource/Resource.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/resource/Resource.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/resource/Resource.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/resource/Resource.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/resource/Resource.d

# Compiles file ../../source/resource/ResourceManager.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/resource/ResourceManager.d
gccDebug_GLES2/../../../source/resource/ResourceManager.o: ../../source/resource/ResourceManager.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/resource/ResourceManager.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/resource/ResourceManager.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/resource/ResourceManager.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/resource/ResourceManager.d

# Compiles file ../../source/resource/archive/Archive.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/resource/archive/Archive.d
gccDebug_GLES2/../../../source/resource/archive/Archive.o: ../../source/resource/archive/Archive.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/resource/archive/Archive.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/resource/archive/Archive.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/resource/archive/Archive.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/resource/archive/Archive.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/../../../source/resource/Resource.o gccRelease_GLES2/../../../source/resource/ResourceManager.o gccRelease_GLES2/../../../source/resource/archive/Archive.o 
	ar rcs ../lib/gccRelease_GLES2/libResource.a gccRelease_GLES2/../../../source/resource/Resource.o gccRelease_GLES2/../../../source/resource/ResourceManager.o gccRelease_GLES2/../../../source/resource/archive/Archive.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/resource/Resource.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/resource/Resource.d
gccRelease_GLES2/../../../source/resource/Resource.o: ../../source/resource/Resource.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/resource/Resource.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/resource/Resource.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/resource/Resource.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/resource/Resource.d

# Compiles file ../../source/resource/ResourceManager.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/resource/ResourceManager.d
gccRelease_GLES2/../../../source/resource/ResourceManager.o: ../../source/resource/ResourceManager.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/resource/ResourceManager.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/resource/ResourceManager.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/resource/ResourceManager.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/resource/ResourceManager.d

# Compiles file ../../source/resource/archive/Archive.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/resource/archive/Archive.d
gccRelease_GLES2/../../../source/resource/archive/Archive.o: ../../source/resource/archive/Archive.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/resource/archive/Archive.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/resource/archive/Archive.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/resource/archive/Archive.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/resource/archive/Archive.d

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

