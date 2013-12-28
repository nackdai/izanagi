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
Debug_GLES2: create_folders gccDebug_GLES2/../../../source/debugutil/DebugFont.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMesh.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshBox.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.o 
	ar rcs ../lib/gccDebug_GLES2/libDebugUtil.a gccDebug_GLES2/../../../source/debugutil/DebugFont.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMesh.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshBox.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/debugutil/DebugFont.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/DebugFont.d
gccDebug_GLES2/../../../source/debugutil/DebugFont.o: ../../source/debugutil/DebugFont.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/DebugFont.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/DebugFont.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/DebugFont.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/DebugFont.d

# Compiles file ../../source/debugutil/mesh/DebugMesh.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/mesh/DebugMesh.d
gccDebug_GLES2/../../../source/debugutil/mesh/DebugMesh.o: ../../source/debugutil/mesh/DebugMesh.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMesh.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMesh.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMesh.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/mesh/DebugMesh.d

# Compiles file ../../source/debugutil/mesh/DebugMeshAxis.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.d
gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.o: ../../source/debugutil/mesh/DebugMeshAxis.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshAxis.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshAxis.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.d

# Compiles file ../../source/debugutil/mesh/DebugMeshBox.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshBox.d
gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshBox.o: ../../source/debugutil/mesh/DebugMeshBox.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshBox.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshBox.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshBox.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshBox.d

# Compiles file ../../source/debugutil/mesh/DebugMeshCylinder.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.d
gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.o: ../../source/debugutil/mesh/DebugMeshCylinder.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshCylinder.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshCylinder.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.d

# Compiles file ../../source/debugutil/mesh/DebugMeshFrustum.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.d
gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.o: ../../source/debugutil/mesh/DebugMeshFrustum.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshFrustum.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshFrustum.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.d

# Compiles file ../../source/debugutil/mesh/DebugMeshGrid.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.d
gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.o: ../../source/debugutil/mesh/DebugMeshGrid.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshGrid.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshGrid.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.d

# Compiles file ../../source/debugutil/mesh/DebugMeshRectangle.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.d
gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.o: ../../source/debugutil/mesh/DebugMeshRectangle.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshRectangle.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshRectangle.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.d

# Compiles file ../../source/debugutil/mesh/DebugMeshSphere.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.d
gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.o: ../../source/debugutil/mesh/DebugMeshSphere.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshSphere.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshSphere.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.d

# Compiles file ../../source/debugutil/mesh/DebugMeshTorus.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.d
gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.o: ../../source/debugutil/mesh/DebugMeshTorus.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshTorus.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshTorus.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/../../../source/debugutil/DebugFont.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMesh.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshBox.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.o 
	ar rcs ../lib/gccRelease_GLES2/libDebugUtil.a gccRelease_GLES2/../../../source/debugutil/DebugFont.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMesh.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshBox.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/debugutil/DebugFont.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/DebugFont.d
gccRelease_GLES2/../../../source/debugutil/DebugFont.o: ../../source/debugutil/DebugFont.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/DebugFont.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/DebugFont.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/DebugFont.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/DebugFont.d

# Compiles file ../../source/debugutil/mesh/DebugMesh.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/mesh/DebugMesh.d
gccRelease_GLES2/../../../source/debugutil/mesh/DebugMesh.o: ../../source/debugutil/mesh/DebugMesh.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMesh.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMesh.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMesh.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/mesh/DebugMesh.d

# Compiles file ../../source/debugutil/mesh/DebugMeshAxis.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.d
gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.o: ../../source/debugutil/mesh/DebugMeshAxis.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshAxis.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshAxis.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshAxis.d

# Compiles file ../../source/debugutil/mesh/DebugMeshBox.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshBox.d
gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshBox.o: ../../source/debugutil/mesh/DebugMeshBox.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshBox.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshBox.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshBox.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshBox.d

# Compiles file ../../source/debugutil/mesh/DebugMeshCylinder.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.d
gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.o: ../../source/debugutil/mesh/DebugMeshCylinder.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshCylinder.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshCylinder.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshCylinder.d

# Compiles file ../../source/debugutil/mesh/DebugMeshFrustum.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.d
gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.o: ../../source/debugutil/mesh/DebugMeshFrustum.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshFrustum.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshFrustum.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshFrustum.d

# Compiles file ../../source/debugutil/mesh/DebugMeshGrid.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.d
gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.o: ../../source/debugutil/mesh/DebugMeshGrid.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshGrid.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshGrid.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshGrid.d

# Compiles file ../../source/debugutil/mesh/DebugMeshRectangle.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.d
gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.o: ../../source/debugutil/mesh/DebugMeshRectangle.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshRectangle.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshRectangle.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshRectangle.d

# Compiles file ../../source/debugutil/mesh/DebugMeshSphere.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.d
gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.o: ../../source/debugutil/mesh/DebugMeshSphere.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshSphere.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshSphere.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshSphere.d

# Compiles file ../../source/debugutil/mesh/DebugMeshTorus.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.d
gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.o: ../../source/debugutil/mesh/DebugMeshTorus.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/debugutil/mesh/DebugMeshTorus.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/debugutil/mesh/DebugMeshTorus.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/../../../source/debugutil/mesh/DebugMeshTorus.d

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

