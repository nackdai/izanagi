# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLES2_Include_Path=-I"../include" -I"../../include" 
Release_GLES2_Include_Path=-I"../include" -I"../../include" 

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
Debug_GLES2: create_folders gccDebug_GLES2/../../../source/shader/ShaderManager.o gccDebug_GLES2/../../../source/shader/ShaderUtil.o gccDebug_GLES2/../../../source/shader/base/ShaderPass.o gccDebug_GLES2/../../../source/shader/base/ShaderBasic.o gccDebug_GLES2/../../../source/shader/base/table/ShaderAttrTable.o gccDebug_GLES2/../../../source/shader/base/table/ShaderParameterTable.o gccDebug_GLES2/../../../source/shader/base/table/ShaderPassTable.o gccDebug_GLES2/../../../source/shader/base/table/ShaderSamplerTable.o 
	ar rcs ../lib/gccDebug_GLES2/libShader.a gccDebug_GLES2/../../../source/shader/ShaderManager.o gccDebug_GLES2/../../../source/shader/ShaderUtil.o gccDebug_GLES2/../../../source/shader/base/ShaderPass.o gccDebug_GLES2/../../../source/shader/base/ShaderBasic.o gccDebug_GLES2/../../../source/shader/base/table/ShaderAttrTable.o gccDebug_GLES2/../../../source/shader/base/table/ShaderParameterTable.o gccDebug_GLES2/../../../source/shader/base/table/ShaderPassTable.o gccDebug_GLES2/../../../source/shader/base/table/ShaderSamplerTable.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/shader/ShaderManager.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/shader/ShaderManager.d
gccDebug_GLES2/../../../source/shader/ShaderManager.o: ../../source/shader/ShaderManager.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/shader/ShaderManager.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/shader/ShaderManager.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/shader/ShaderManager.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/shader/ShaderManager.d

# Compiles file ../../source/shader/ShaderUtil.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/shader/ShaderUtil.d
gccDebug_GLES2/../../../source/shader/ShaderUtil.o: ../../source/shader/ShaderUtil.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/shader/ShaderUtil.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/shader/ShaderUtil.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/shader/ShaderUtil.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/shader/ShaderUtil.d

# Compiles file ../../source/shader/base/ShaderPass.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/shader/base/ShaderPass.d
gccDebug_GLES2/../../../source/shader/base/ShaderPass.o: ../../source/shader/base/ShaderPass.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/shader/base/ShaderPass.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/shader/base/ShaderPass.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/shader/base/ShaderPass.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/shader/base/ShaderPass.d

# Compiles file ../../source/shader/base/ShaderBasic.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/shader/base/ShaderBasic.d
gccDebug_GLES2/../../../source/shader/base/ShaderBasic.o: ../../source/shader/base/ShaderBasic.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/shader/base/ShaderBasic.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/shader/base/ShaderBasic.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/shader/base/ShaderBasic.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/shader/base/ShaderBasic.d

# Compiles file ../../source/shader/base/table/ShaderAttrTable.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/shader/base/table/ShaderAttrTable.d
gccDebug_GLES2/../../../source/shader/base/table/ShaderAttrTable.o: ../../source/shader/base/table/ShaderAttrTable.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/shader/base/table/ShaderAttrTable.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/shader/base/table/ShaderAttrTable.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/shader/base/table/ShaderAttrTable.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/shader/base/table/ShaderAttrTable.d

# Compiles file ../../source/shader/base/table/ShaderParameterTable.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/shader/base/table/ShaderParameterTable.d
gccDebug_GLES2/../../../source/shader/base/table/ShaderParameterTable.o: ../../source/shader/base/table/ShaderParameterTable.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/shader/base/table/ShaderParameterTable.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/shader/base/table/ShaderParameterTable.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/shader/base/table/ShaderParameterTable.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/shader/base/table/ShaderParameterTable.d

# Compiles file ../../source/shader/base/table/ShaderPassTable.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/shader/base/table/ShaderPassTable.d
gccDebug_GLES2/../../../source/shader/base/table/ShaderPassTable.o: ../../source/shader/base/table/ShaderPassTable.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/shader/base/table/ShaderPassTable.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/shader/base/table/ShaderPassTable.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/shader/base/table/ShaderPassTable.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/shader/base/table/ShaderPassTable.d

# Compiles file ../../source/shader/base/table/ShaderSamplerTable.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/shader/base/table/ShaderSamplerTable.d
gccDebug_GLES2/../../../source/shader/base/table/ShaderSamplerTable.o: ../../source/shader/base/table/ShaderSamplerTable.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/shader/base/table/ShaderSamplerTable.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/shader/base/table/ShaderSamplerTable.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/shader/base/table/ShaderSamplerTable.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/shader/base/table/ShaderSamplerTable.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/../../../source/shader/ShaderManager.o gccRelease_GLES2/../../../source/shader/ShaderUtil.o gccRelease_GLES2/../../../source/shader/base/ShaderPass.o gccRelease_GLES2/../../../source/shader/base/ShaderBasic.o gccRelease_GLES2/../../../source/shader/base/table/ShaderAttrTable.o gccRelease_GLES2/../../../source/shader/base/table/ShaderParameterTable.o gccRelease_GLES2/../../../source/shader/base/table/ShaderPassTable.o gccRelease_GLES2/../../../source/shader/base/table/ShaderSamplerTable.o 
	ar rcs ../lib/gccRelease_GLES2/libShader.a gccRelease_GLES2/../../../source/shader/ShaderManager.o gccRelease_GLES2/../../../source/shader/ShaderUtil.o gccRelease_GLES2/../../../source/shader/base/ShaderPass.o gccRelease_GLES2/../../../source/shader/base/ShaderBasic.o gccRelease_GLES2/../../../source/shader/base/table/ShaderAttrTable.o gccRelease_GLES2/../../../source/shader/base/table/ShaderParameterTable.o gccRelease_GLES2/../../../source/shader/base/table/ShaderPassTable.o gccRelease_GLES2/../../../source/shader/base/table/ShaderSamplerTable.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/shader/ShaderManager.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/shader/ShaderManager.d
gccRelease_GLES2/../../../source/shader/ShaderManager.o: ../../source/shader/ShaderManager.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/shader/ShaderManager.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/shader/ShaderManager.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/shader/ShaderManager.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/shader/ShaderManager.d

# Compiles file ../../source/shader/ShaderUtil.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/shader/ShaderUtil.d
gccRelease_GLES2/../../../source/shader/ShaderUtil.o: ../../source/shader/ShaderUtil.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/shader/ShaderUtil.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/shader/ShaderUtil.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/shader/ShaderUtil.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/shader/ShaderUtil.d

# Compiles file ../../source/shader/base/ShaderPass.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/shader/base/ShaderPass.d
gccRelease_GLES2/../../../source/shader/base/ShaderPass.o: ../../source/shader/base/ShaderPass.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/shader/base/ShaderPass.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/shader/base/ShaderPass.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/shader/base/ShaderPass.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/shader/base/ShaderPass.d

# Compiles file ../../source/shader/base/ShaderBasic.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/shader/base/ShaderBasic.d
gccRelease_GLES2/../../../source/shader/base/ShaderBasic.o: ../../source/shader/base/ShaderBasic.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/shader/base/ShaderBasic.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/shader/base/ShaderBasic.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/shader/base/ShaderBasic.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/shader/base/ShaderBasic.d

# Compiles file ../../source/shader/base/table/ShaderAttrTable.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/shader/base/table/ShaderAttrTable.d
gccRelease_GLES2/../../../source/shader/base/table/ShaderAttrTable.o: ../../source/shader/base/table/ShaderAttrTable.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/shader/base/table/ShaderAttrTable.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/shader/base/table/ShaderAttrTable.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/shader/base/table/ShaderAttrTable.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/shader/base/table/ShaderAttrTable.d

# Compiles file ../../source/shader/base/table/ShaderParameterTable.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/shader/base/table/ShaderParameterTable.d
gccRelease_GLES2/../../../source/shader/base/table/ShaderParameterTable.o: ../../source/shader/base/table/ShaderParameterTable.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/shader/base/table/ShaderParameterTable.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/shader/base/table/ShaderParameterTable.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/shader/base/table/ShaderParameterTable.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/shader/base/table/ShaderParameterTable.d

# Compiles file ../../source/shader/base/table/ShaderPassTable.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/shader/base/table/ShaderPassTable.d
gccRelease_GLES2/../../../source/shader/base/table/ShaderPassTable.o: ../../source/shader/base/table/ShaderPassTable.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/shader/base/table/ShaderPassTable.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/shader/base/table/ShaderPassTable.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/shader/base/table/ShaderPassTable.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/shader/base/table/ShaderPassTable.d

# Compiles file ../../source/shader/base/table/ShaderSamplerTable.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/shader/base/table/ShaderSamplerTable.d
gccRelease_GLES2/../../../source/shader/base/table/ShaderSamplerTable.o: ../../source/shader/base/table/ShaderSamplerTable.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/shader/base/table/ShaderSamplerTable.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/shader/base/table/ShaderSamplerTable.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/shader/base/table/ShaderSamplerTable.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/shader/base/table/ShaderSamplerTable.d

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

