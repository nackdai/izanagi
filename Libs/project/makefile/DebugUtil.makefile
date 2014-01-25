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
Debug_GLUT: create_folders gccDebug_GLUT/source/debugutil/DebugFont.o gccDebug_GLUT/source/debugutil/mesh/DebugMesh.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshAxis.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshBox.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshCylinder.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshFrustum.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshGrid.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshRectangle.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshSphere.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshTorus.o 
	ar rcs ../lib/gccDebug_GLUT/libDebugUtil.a gccDebug_GLUT/source/debugutil/DebugFont.o gccDebug_GLUT/source/debugutil/mesh/DebugMesh.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshAxis.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshBox.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshCylinder.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshFrustum.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshGrid.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshRectangle.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshSphere.o gccDebug_GLUT/source/debugutil/mesh/DebugMeshTorus.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/debugutil/DebugFont.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/DebugFont.d
gccDebug_GLUT/source/debugutil/DebugFont.o: ../../source/debugutil/DebugFont.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/DebugFont.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/DebugFont.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/DebugFont.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/DebugFont.d

# Compiles file ../../source/debugutil/mesh/DebugMesh.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/mesh/DebugMesh.d
gccDebug_GLUT/source/debugutil/mesh/DebugMesh.o: ../../source/debugutil/mesh/DebugMesh.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMesh.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/mesh/DebugMesh.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMesh.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/mesh/DebugMesh.d

# Compiles file ../../source/debugutil/mesh/DebugMeshAxis.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/mesh/DebugMeshAxis.d
gccDebug_GLUT/source/debugutil/mesh/DebugMeshAxis.o: ../../source/debugutil/mesh/DebugMeshAxis.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshAxis.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/mesh/DebugMeshAxis.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshAxis.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/mesh/DebugMeshAxis.d

# Compiles file ../../source/debugutil/mesh/DebugMeshBox.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/mesh/DebugMeshBox.d
gccDebug_GLUT/source/debugutil/mesh/DebugMeshBox.o: ../../source/debugutil/mesh/DebugMeshBox.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshBox.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/mesh/DebugMeshBox.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshBox.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/mesh/DebugMeshBox.d

# Compiles file ../../source/debugutil/mesh/DebugMeshCylinder.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/mesh/DebugMeshCylinder.d
gccDebug_GLUT/source/debugutil/mesh/DebugMeshCylinder.o: ../../source/debugutil/mesh/DebugMeshCylinder.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshCylinder.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/mesh/DebugMeshCylinder.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshCylinder.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/mesh/DebugMeshCylinder.d

# Compiles file ../../source/debugutil/mesh/DebugMeshFrustum.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/mesh/DebugMeshFrustum.d
gccDebug_GLUT/source/debugutil/mesh/DebugMeshFrustum.o: ../../source/debugutil/mesh/DebugMeshFrustum.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshFrustum.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/mesh/DebugMeshFrustum.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshFrustum.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/mesh/DebugMeshFrustum.d

# Compiles file ../../source/debugutil/mesh/DebugMeshGrid.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/mesh/DebugMeshGrid.d
gccDebug_GLUT/source/debugutil/mesh/DebugMeshGrid.o: ../../source/debugutil/mesh/DebugMeshGrid.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshGrid.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/mesh/DebugMeshGrid.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshGrid.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/mesh/DebugMeshGrid.d

# Compiles file ../../source/debugutil/mesh/DebugMeshRectangle.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/mesh/DebugMeshRectangle.d
gccDebug_GLUT/source/debugutil/mesh/DebugMeshRectangle.o: ../../source/debugutil/mesh/DebugMeshRectangle.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshRectangle.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/mesh/DebugMeshRectangle.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshRectangle.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/mesh/DebugMeshRectangle.d

# Compiles file ../../source/debugutil/mesh/DebugMeshSphere.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/mesh/DebugMeshSphere.d
gccDebug_GLUT/source/debugutil/mesh/DebugMeshSphere.o: ../../source/debugutil/mesh/DebugMeshSphere.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshSphere.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/mesh/DebugMeshSphere.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshSphere.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/mesh/DebugMeshSphere.d

# Compiles file ../../source/debugutil/mesh/DebugMeshTorus.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/debugutil/mesh/DebugMeshTorus.d
gccDebug_GLUT/source/debugutil/mesh/DebugMeshTorus.o: ../../source/debugutil/mesh/DebugMeshTorus.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshTorus.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/debugutil/mesh/DebugMeshTorus.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshTorus.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/debugutil/mesh/DebugMeshTorus.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/debugutil
	mkdir -p gccDebug_GLUT/source/debugutil/mesh
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/debugutil/*.o
	rm -f gccDebug_GLUT/source/debugutil/*.d
	rm -f gccDebug_GLUT/source/debugutil/mesh/*.o
	rm -f gccDebug_GLUT/source/debugutil/mesh/*.d
	rm -f ../lib/gccDebug_GLUT/libDebugUtil.a

