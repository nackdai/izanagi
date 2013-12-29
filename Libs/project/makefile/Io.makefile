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
Debug_GLES2: create_folders 
	ar rcs ../lib/gccDebug_GLES2/libIo.a  $(Debug_GLES2_Implicitly_Linked_Objects)

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders 
	ar rcs ../lib/gccRelease_GLES2/libIo.a  $(Release_GLES2_Implicitly_Linked_Objects)

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

