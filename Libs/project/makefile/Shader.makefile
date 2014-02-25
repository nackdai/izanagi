# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../include" -I"../../include" -I"../../../External/glew/include" -I"../../../External/freeglut/include" 

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
Debug_GLUT: create_folders gccDebug_GLUT/source/shader/ShaderManager.o gccDebug_GLUT/source/shader/ShaderUtil.o gccDebug_GLUT/source/shader/base/ShaderPass.o gccDebug_GLUT/source/shader/base/ShaderBasic.o gccDebug_GLUT/source/shader/base/table/ShaderAttrTable.o gccDebug_GLUT/source/shader/base/table/ShaderParameterTable.o gccDebug_GLUT/source/shader/base/table/ShaderPassTable.o gccDebug_GLUT/source/shader/base/table/ShaderSamplerTable.o 
	ar rcs ../lib/gccDebug_GLUT/libShader.a gccDebug_GLUT/source/shader/ShaderManager.o gccDebug_GLUT/source/shader/ShaderUtil.o gccDebug_GLUT/source/shader/base/ShaderPass.o gccDebug_GLUT/source/shader/base/ShaderBasic.o gccDebug_GLUT/source/shader/base/table/ShaderAttrTable.o gccDebug_GLUT/source/shader/base/table/ShaderParameterTable.o gccDebug_GLUT/source/shader/base/table/ShaderPassTable.o gccDebug_GLUT/source/shader/base/table/ShaderSamplerTable.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/shader/ShaderManager.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/shader/ShaderManager.d
gccDebug_GLUT/source/shader/ShaderManager.o: ../../source/shader/ShaderManager.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/shader/ShaderManager.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/shader/ShaderManager.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/shader/ShaderManager.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/shader/ShaderManager.d

# Compiles file ../../source/shader/ShaderUtil.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/shader/ShaderUtil.d
gccDebug_GLUT/source/shader/ShaderUtil.o: ../../source/shader/ShaderUtil.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/shader/ShaderUtil.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/shader/ShaderUtil.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/shader/ShaderUtil.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/shader/ShaderUtil.d

# Compiles file ../../source/shader/base/ShaderPass.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/shader/base/ShaderPass.d
gccDebug_GLUT/source/shader/base/ShaderPass.o: ../../source/shader/base/ShaderPass.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/shader/base/ShaderPass.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/shader/base/ShaderPass.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/shader/base/ShaderPass.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/shader/base/ShaderPass.d

# Compiles file ../../source/shader/base/ShaderBasic.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/shader/base/ShaderBasic.d
gccDebug_GLUT/source/shader/base/ShaderBasic.o: ../../source/shader/base/ShaderBasic.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/shader/base/ShaderBasic.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/shader/base/ShaderBasic.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/shader/base/ShaderBasic.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/shader/base/ShaderBasic.d

# Compiles file ../../source/shader/base/table/ShaderAttrTable.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/shader/base/table/ShaderAttrTable.d
gccDebug_GLUT/source/shader/base/table/ShaderAttrTable.o: ../../source/shader/base/table/ShaderAttrTable.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/shader/base/table/ShaderAttrTable.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/shader/base/table/ShaderAttrTable.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/shader/base/table/ShaderAttrTable.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/shader/base/table/ShaderAttrTable.d

# Compiles file ../../source/shader/base/table/ShaderParameterTable.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/shader/base/table/ShaderParameterTable.d
gccDebug_GLUT/source/shader/base/table/ShaderParameterTable.o: ../../source/shader/base/table/ShaderParameterTable.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/shader/base/table/ShaderParameterTable.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/shader/base/table/ShaderParameterTable.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/shader/base/table/ShaderParameterTable.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/shader/base/table/ShaderParameterTable.d

# Compiles file ../../source/shader/base/table/ShaderPassTable.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/shader/base/table/ShaderPassTable.d
gccDebug_GLUT/source/shader/base/table/ShaderPassTable.o: ../../source/shader/base/table/ShaderPassTable.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/shader/base/table/ShaderPassTable.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/shader/base/table/ShaderPassTable.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/shader/base/table/ShaderPassTable.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/shader/base/table/ShaderPassTable.d

# Compiles file ../../source/shader/base/table/ShaderSamplerTable.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/shader/base/table/ShaderSamplerTable.d
gccDebug_GLUT/source/shader/base/table/ShaderSamplerTable.o: ../../source/shader/base/table/ShaderSamplerTable.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/shader/base/table/ShaderSamplerTable.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/shader/base/table/ShaderSamplerTable.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/shader/base/table/ShaderSamplerTable.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/shader/base/table/ShaderSamplerTable.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/shader
	mkdir -p gccDebug_GLUT/source/shader/base
	mkdir -p gccDebug_GLUT/source/shader/base/table
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/shader/*.o
	rm -f gccDebug_GLUT/source/shader/*.d
	rm -f gccDebug_GLUT/source/shader/base/*.o
	rm -f gccDebug_GLUT/source/shader/base/*.d
	rm -f gccDebug_GLUT/source/shader/base/table/*.o
	rm -f gccDebug_GLUT/source/shader/base/table/*.d
	rm -f ../lib/gccDebug_GLUT/libShader.a

