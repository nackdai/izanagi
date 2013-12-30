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
Debug_GLES2: create_folders gccDebug_GLES2/source/scenegraph/Camera.o gccDebug_GLES2/source/scenegraph/Decal.o gccDebug_GLES2/source/scenegraph/SceneGraphUtil.o gccDebug_GLES2/source/scenegraph/SceneParam.o gccDebug_GLES2/source/scenegraph/SceneState.o gccDebug_GLES2/source/scenegraph/VectorCamera.o gccDebug_GLES2/source/scenegraph/img/izImage.o gccDebug_GLES2/source/scenegraph/mdl/izModel.o gccDebug_GLES2/source/scenegraph/anm/AnimationBinder.o gccDebug_GLES2/source/scenegraph/anm/AnimationInterface.o gccDebug_GLES2/source/scenegraph/anm/AnimationInterp.o gccDebug_GLES2/source/scenegraph/anm/AnimationUtil.o gccDebug_GLES2/source/scenegraph/anm/izAnimation.o gccDebug_GLES2/source/scenegraph/anm/PoseUpdater.o gccDebug_GLES2/source/scenegraph/anm/AnimationLinearBlender.o gccDebug_GLES2/source/scenegraph/mtrl/izMaterial.o gccDebug_GLES2/source/scenegraph/msh/izMesh.o gccDebug_GLES2/source/scenegraph/msh/MeshInstance.o gccDebug_GLES2/source/scenegraph/msh/MeshSetInstance.o gccDebug_GLES2/source/scenegraph/msh/MshMeshGroup.o gccDebug_GLES2/source/scenegraph/msh/MshMeshSet.o gccDebug_GLES2/source/scenegraph/msh/MshPrimitiveSet.o gccDebug_GLES2/source/scenegraph/skl/izSkeleton.o gccDebug_GLES2/source/scenegraph/skl/SkeletonInstance.o gccDebug_GLES2/source/scenegraph/skl/SkeletonUtil.o gccDebug_GLES2/source/scenegraph/rendergraph/SceneRenderer.o gccDebug_GLES2/source/scenegraph/rendergraph/RenderGraph.o 
	ar rcs ../lib/gccDebug_GLES2/libSceneGraph.a gccDebug_GLES2/source/scenegraph/Camera.o gccDebug_GLES2/source/scenegraph/Decal.o gccDebug_GLES2/source/scenegraph/SceneGraphUtil.o gccDebug_GLES2/source/scenegraph/SceneParam.o gccDebug_GLES2/source/scenegraph/SceneState.o gccDebug_GLES2/source/scenegraph/VectorCamera.o gccDebug_GLES2/source/scenegraph/img/izImage.o gccDebug_GLES2/source/scenegraph/mdl/izModel.o gccDebug_GLES2/source/scenegraph/anm/AnimationBinder.o gccDebug_GLES2/source/scenegraph/anm/AnimationInterface.o gccDebug_GLES2/source/scenegraph/anm/AnimationInterp.o gccDebug_GLES2/source/scenegraph/anm/AnimationUtil.o gccDebug_GLES2/source/scenegraph/anm/izAnimation.o gccDebug_GLES2/source/scenegraph/anm/PoseUpdater.o gccDebug_GLES2/source/scenegraph/anm/AnimationLinearBlender.o gccDebug_GLES2/source/scenegraph/mtrl/izMaterial.o gccDebug_GLES2/source/scenegraph/msh/izMesh.o gccDebug_GLES2/source/scenegraph/msh/MeshInstance.o gccDebug_GLES2/source/scenegraph/msh/MeshSetInstance.o gccDebug_GLES2/source/scenegraph/msh/MshMeshGroup.o gccDebug_GLES2/source/scenegraph/msh/MshMeshSet.o gccDebug_GLES2/source/scenegraph/msh/MshPrimitiveSet.o gccDebug_GLES2/source/scenegraph/skl/izSkeleton.o gccDebug_GLES2/source/scenegraph/skl/SkeletonInstance.o gccDebug_GLES2/source/scenegraph/skl/SkeletonUtil.o gccDebug_GLES2/source/scenegraph/rendergraph/SceneRenderer.o gccDebug_GLES2/source/scenegraph/rendergraph/RenderGraph.o  $(Debug_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/scenegraph/Camera.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/Camera.d
gccDebug_GLES2/source/scenegraph/Camera.o: ../../source/scenegraph/Camera.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/Camera.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/Camera.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/Camera.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/Camera.d

# Compiles file ../../source/scenegraph/Decal.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/Decal.d
gccDebug_GLES2/source/scenegraph/Decal.o: ../../source/scenegraph/Decal.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/Decal.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/Decal.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/Decal.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/Decal.d

# Compiles file ../../source/scenegraph/SceneGraphUtil.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/SceneGraphUtil.d
gccDebug_GLES2/source/scenegraph/SceneGraphUtil.o: ../../source/scenegraph/SceneGraphUtil.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/SceneGraphUtil.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/SceneGraphUtil.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/SceneGraphUtil.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/SceneGraphUtil.d

# Compiles file ../../source/scenegraph/SceneParam.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/SceneParam.d
gccDebug_GLES2/source/scenegraph/SceneParam.o: ../../source/scenegraph/SceneParam.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/SceneParam.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/SceneParam.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/SceneParam.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/SceneParam.d

# Compiles file ../../source/scenegraph/SceneState.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/SceneState.d
gccDebug_GLES2/source/scenegraph/SceneState.o: ../../source/scenegraph/SceneState.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/SceneState.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/SceneState.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/SceneState.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/SceneState.d

# Compiles file ../../source/scenegraph/VectorCamera.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/VectorCamera.d
gccDebug_GLES2/source/scenegraph/VectorCamera.o: ../../source/scenegraph/VectorCamera.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/VectorCamera.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/VectorCamera.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/VectorCamera.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/VectorCamera.d

# Compiles file ../../source/scenegraph/img/izImage.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/img/izImage.d
gccDebug_GLES2/source/scenegraph/img/izImage.o: ../../source/scenegraph/img/izImage.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/img/izImage.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/img/izImage.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/img/izImage.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/img/izImage.d

# Compiles file ../../source/scenegraph/mdl/izModel.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/mdl/izModel.d
gccDebug_GLES2/source/scenegraph/mdl/izModel.o: ../../source/scenegraph/mdl/izModel.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/mdl/izModel.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/mdl/izModel.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/mdl/izModel.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/mdl/izModel.d

# Compiles file ../../source/scenegraph/anm/AnimationBinder.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/anm/AnimationBinder.d
gccDebug_GLES2/source/scenegraph/anm/AnimationBinder.o: ../../source/scenegraph/anm/AnimationBinder.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationBinder.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/anm/AnimationBinder.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationBinder.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/anm/AnimationBinder.d

# Compiles file ../../source/scenegraph/anm/AnimationInterface.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/anm/AnimationInterface.d
gccDebug_GLES2/source/scenegraph/anm/AnimationInterface.o: ../../source/scenegraph/anm/AnimationInterface.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationInterface.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/anm/AnimationInterface.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationInterface.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/anm/AnimationInterface.d

# Compiles file ../../source/scenegraph/anm/AnimationInterp.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/anm/AnimationInterp.d
gccDebug_GLES2/source/scenegraph/anm/AnimationInterp.o: ../../source/scenegraph/anm/AnimationInterp.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationInterp.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/anm/AnimationInterp.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationInterp.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/anm/AnimationInterp.d

# Compiles file ../../source/scenegraph/anm/AnimationUtil.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/anm/AnimationUtil.d
gccDebug_GLES2/source/scenegraph/anm/AnimationUtil.o: ../../source/scenegraph/anm/AnimationUtil.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationUtil.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/anm/AnimationUtil.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationUtil.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/anm/AnimationUtil.d

# Compiles file ../../source/scenegraph/anm/izAnimation.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/anm/izAnimation.d
gccDebug_GLES2/source/scenegraph/anm/izAnimation.o: ../../source/scenegraph/anm/izAnimation.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/izAnimation.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/anm/izAnimation.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/izAnimation.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/anm/izAnimation.d

# Compiles file ../../source/scenegraph/anm/PoseUpdater.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/anm/PoseUpdater.d
gccDebug_GLES2/source/scenegraph/anm/PoseUpdater.o: ../../source/scenegraph/anm/PoseUpdater.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/PoseUpdater.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/anm/PoseUpdater.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/PoseUpdater.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/anm/PoseUpdater.d

# Compiles file ../../source/scenegraph/anm/AnimationLinearBlender.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/anm/AnimationLinearBlender.d
gccDebug_GLES2/source/scenegraph/anm/AnimationLinearBlender.o: ../../source/scenegraph/anm/AnimationLinearBlender.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationLinearBlender.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/anm/AnimationLinearBlender.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationLinearBlender.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/anm/AnimationLinearBlender.d

# Compiles file ../../source/scenegraph/mtrl/izMaterial.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/mtrl/izMaterial.d
gccDebug_GLES2/source/scenegraph/mtrl/izMaterial.o: ../../source/scenegraph/mtrl/izMaterial.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/mtrl/izMaterial.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/mtrl/izMaterial.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/mtrl/izMaterial.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/mtrl/izMaterial.d

# Compiles file ../../source/scenegraph/msh/izMesh.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/msh/izMesh.d
gccDebug_GLES2/source/scenegraph/msh/izMesh.o: ../../source/scenegraph/msh/izMesh.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/izMesh.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/msh/izMesh.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/izMesh.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/msh/izMesh.d

# Compiles file ../../source/scenegraph/msh/MeshInstance.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/msh/MeshInstance.d
gccDebug_GLES2/source/scenegraph/msh/MeshInstance.o: ../../source/scenegraph/msh/MeshInstance.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MeshInstance.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/msh/MeshInstance.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MeshInstance.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/msh/MeshInstance.d

# Compiles file ../../source/scenegraph/msh/MeshSetInstance.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/msh/MeshSetInstance.d
gccDebug_GLES2/source/scenegraph/msh/MeshSetInstance.o: ../../source/scenegraph/msh/MeshSetInstance.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MeshSetInstance.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/msh/MeshSetInstance.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MeshSetInstance.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/msh/MeshSetInstance.d

# Compiles file ../../source/scenegraph/msh/MshMeshGroup.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/msh/MshMeshGroup.d
gccDebug_GLES2/source/scenegraph/msh/MshMeshGroup.o: ../../source/scenegraph/msh/MshMeshGroup.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MshMeshGroup.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/msh/MshMeshGroup.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MshMeshGroup.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/msh/MshMeshGroup.d

# Compiles file ../../source/scenegraph/msh/MshMeshSet.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/msh/MshMeshSet.d
gccDebug_GLES2/source/scenegraph/msh/MshMeshSet.o: ../../source/scenegraph/msh/MshMeshSet.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MshMeshSet.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/msh/MshMeshSet.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MshMeshSet.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/msh/MshMeshSet.d

# Compiles file ../../source/scenegraph/msh/MshPrimitiveSet.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/msh/MshPrimitiveSet.d
gccDebug_GLES2/source/scenegraph/msh/MshPrimitiveSet.o: ../../source/scenegraph/msh/MshPrimitiveSet.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MshPrimitiveSet.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/msh/MshPrimitiveSet.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MshPrimitiveSet.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/msh/MshPrimitiveSet.d

# Compiles file ../../source/scenegraph/skl/izSkeleton.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/skl/izSkeleton.d
gccDebug_GLES2/source/scenegraph/skl/izSkeleton.o: ../../source/scenegraph/skl/izSkeleton.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/skl/izSkeleton.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/skl/izSkeleton.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/skl/izSkeleton.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/skl/izSkeleton.d

# Compiles file ../../source/scenegraph/skl/SkeletonInstance.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/skl/SkeletonInstance.d
gccDebug_GLES2/source/scenegraph/skl/SkeletonInstance.o: ../../source/scenegraph/skl/SkeletonInstance.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/skl/SkeletonInstance.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/skl/SkeletonInstance.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/skl/SkeletonInstance.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/skl/SkeletonInstance.d

# Compiles file ../../source/scenegraph/skl/SkeletonUtil.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/skl/SkeletonUtil.d
gccDebug_GLES2/source/scenegraph/skl/SkeletonUtil.o: ../../source/scenegraph/skl/SkeletonUtil.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/skl/SkeletonUtil.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/skl/SkeletonUtil.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/skl/SkeletonUtil.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/skl/SkeletonUtil.d

# Compiles file ../../source/scenegraph/rendergraph/SceneRenderer.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/rendergraph/SceneRenderer.d
gccDebug_GLES2/source/scenegraph/rendergraph/SceneRenderer.o: ../../source/scenegraph/rendergraph/SceneRenderer.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/rendergraph/SceneRenderer.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/rendergraph/SceneRenderer.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/rendergraph/SceneRenderer.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/rendergraph/SceneRenderer.d

# Compiles file ../../source/scenegraph/rendergraph/RenderGraph.cpp for the Debug_GLES2 configuration...
-include gccDebug_GLES2/source/scenegraph/rendergraph/RenderGraph.d
gccDebug_GLES2/source/scenegraph/rendergraph/RenderGraph.o: ../../source/scenegraph/rendergraph/RenderGraph.cpp
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -c ../../source/scenegraph/rendergraph/RenderGraph.cpp $(Debug_GLES2_Include_Path) -o gccDebug_GLES2/source/scenegraph/rendergraph/RenderGraph.o
	$(CPP_COMPILER) $(Debug_GLES2_Preprocessor_Definitions) $(Debug_GLES2_Compiler_Flags) -MM ../../source/scenegraph/rendergraph/RenderGraph.cpp $(Debug_GLES2_Include_Path) > gccDebug_GLES2/source/scenegraph/rendergraph/RenderGraph.d

# Builds the Release_GLES2 configuration...
.PHONY: Release_GLES2
Release_GLES2: create_folders gccRelease_GLES2/source/scenegraph/Camera.o gccRelease_GLES2/source/scenegraph/Decal.o gccRelease_GLES2/source/scenegraph/SceneGraphUtil.o gccRelease_GLES2/source/scenegraph/SceneParam.o gccRelease_GLES2/source/scenegraph/SceneState.o gccRelease_GLES2/source/scenegraph/VectorCamera.o gccRelease_GLES2/source/scenegraph/img/izImage.o gccRelease_GLES2/source/scenegraph/mdl/izModel.o gccRelease_GLES2/source/scenegraph/anm/AnimationBinder.o gccRelease_GLES2/source/scenegraph/anm/AnimationInterface.o gccRelease_GLES2/source/scenegraph/anm/AnimationInterp.o gccRelease_GLES2/source/scenegraph/anm/AnimationUtil.o gccRelease_GLES2/source/scenegraph/anm/izAnimation.o gccRelease_GLES2/source/scenegraph/anm/PoseUpdater.o gccRelease_GLES2/source/scenegraph/anm/AnimationLinearBlender.o gccRelease_GLES2/source/scenegraph/mtrl/izMaterial.o gccRelease_GLES2/source/scenegraph/msh/izMesh.o gccRelease_GLES2/source/scenegraph/msh/MeshInstance.o gccRelease_GLES2/source/scenegraph/msh/MeshSetInstance.o gccRelease_GLES2/source/scenegraph/msh/MshMeshGroup.o gccRelease_GLES2/source/scenegraph/msh/MshMeshSet.o gccRelease_GLES2/source/scenegraph/msh/MshPrimitiveSet.o gccRelease_GLES2/source/scenegraph/skl/izSkeleton.o gccRelease_GLES2/source/scenegraph/skl/SkeletonInstance.o gccRelease_GLES2/source/scenegraph/skl/SkeletonUtil.o gccRelease_GLES2/source/scenegraph/rendergraph/SceneRenderer.o gccRelease_GLES2/source/scenegraph/rendergraph/RenderGraph.o 
	ar rcs ../lib/gccRelease_GLES2/libSceneGraph.a gccRelease_GLES2/source/scenegraph/Camera.o gccRelease_GLES2/source/scenegraph/Decal.o gccRelease_GLES2/source/scenegraph/SceneGraphUtil.o gccRelease_GLES2/source/scenegraph/SceneParam.o gccRelease_GLES2/source/scenegraph/SceneState.o gccRelease_GLES2/source/scenegraph/VectorCamera.o gccRelease_GLES2/source/scenegraph/img/izImage.o gccRelease_GLES2/source/scenegraph/mdl/izModel.o gccRelease_GLES2/source/scenegraph/anm/AnimationBinder.o gccRelease_GLES2/source/scenegraph/anm/AnimationInterface.o gccRelease_GLES2/source/scenegraph/anm/AnimationInterp.o gccRelease_GLES2/source/scenegraph/anm/AnimationUtil.o gccRelease_GLES2/source/scenegraph/anm/izAnimation.o gccRelease_GLES2/source/scenegraph/anm/PoseUpdater.o gccRelease_GLES2/source/scenegraph/anm/AnimationLinearBlender.o gccRelease_GLES2/source/scenegraph/mtrl/izMaterial.o gccRelease_GLES2/source/scenegraph/msh/izMesh.o gccRelease_GLES2/source/scenegraph/msh/MeshInstance.o gccRelease_GLES2/source/scenegraph/msh/MeshSetInstance.o gccRelease_GLES2/source/scenegraph/msh/MshMeshGroup.o gccRelease_GLES2/source/scenegraph/msh/MshMeshSet.o gccRelease_GLES2/source/scenegraph/msh/MshPrimitiveSet.o gccRelease_GLES2/source/scenegraph/skl/izSkeleton.o gccRelease_GLES2/source/scenegraph/skl/SkeletonInstance.o gccRelease_GLES2/source/scenegraph/skl/SkeletonUtil.o gccRelease_GLES2/source/scenegraph/rendergraph/SceneRenderer.o gccRelease_GLES2/source/scenegraph/rendergraph/RenderGraph.o  $(Release_GLES2_Implicitly_Linked_Objects)

# Compiles file ../../source/scenegraph/Camera.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/Camera.d
gccRelease_GLES2/source/scenegraph/Camera.o: ../../source/scenegraph/Camera.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/Camera.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/Camera.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/Camera.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/Camera.d

# Compiles file ../../source/scenegraph/Decal.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/Decal.d
gccRelease_GLES2/source/scenegraph/Decal.o: ../../source/scenegraph/Decal.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/Decal.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/Decal.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/Decal.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/Decal.d

# Compiles file ../../source/scenegraph/SceneGraphUtil.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/SceneGraphUtil.d
gccRelease_GLES2/source/scenegraph/SceneGraphUtil.o: ../../source/scenegraph/SceneGraphUtil.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/SceneGraphUtil.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/SceneGraphUtil.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/SceneGraphUtil.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/SceneGraphUtil.d

# Compiles file ../../source/scenegraph/SceneParam.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/SceneParam.d
gccRelease_GLES2/source/scenegraph/SceneParam.o: ../../source/scenegraph/SceneParam.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/SceneParam.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/SceneParam.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/SceneParam.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/SceneParam.d

# Compiles file ../../source/scenegraph/SceneState.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/SceneState.d
gccRelease_GLES2/source/scenegraph/SceneState.o: ../../source/scenegraph/SceneState.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/SceneState.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/SceneState.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/SceneState.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/SceneState.d

# Compiles file ../../source/scenegraph/VectorCamera.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/VectorCamera.d
gccRelease_GLES2/source/scenegraph/VectorCamera.o: ../../source/scenegraph/VectorCamera.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/VectorCamera.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/VectorCamera.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/VectorCamera.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/VectorCamera.d

# Compiles file ../../source/scenegraph/img/izImage.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/img/izImage.d
gccRelease_GLES2/source/scenegraph/img/izImage.o: ../../source/scenegraph/img/izImage.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/img/izImage.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/img/izImage.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/img/izImage.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/img/izImage.d

# Compiles file ../../source/scenegraph/mdl/izModel.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/mdl/izModel.d
gccRelease_GLES2/source/scenegraph/mdl/izModel.o: ../../source/scenegraph/mdl/izModel.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/mdl/izModel.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/mdl/izModel.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/mdl/izModel.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/mdl/izModel.d

# Compiles file ../../source/scenegraph/anm/AnimationBinder.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/anm/AnimationBinder.d
gccRelease_GLES2/source/scenegraph/anm/AnimationBinder.o: ../../source/scenegraph/anm/AnimationBinder.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationBinder.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/anm/AnimationBinder.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationBinder.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/anm/AnimationBinder.d

# Compiles file ../../source/scenegraph/anm/AnimationInterface.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/anm/AnimationInterface.d
gccRelease_GLES2/source/scenegraph/anm/AnimationInterface.o: ../../source/scenegraph/anm/AnimationInterface.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationInterface.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/anm/AnimationInterface.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationInterface.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/anm/AnimationInterface.d

# Compiles file ../../source/scenegraph/anm/AnimationInterp.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/anm/AnimationInterp.d
gccRelease_GLES2/source/scenegraph/anm/AnimationInterp.o: ../../source/scenegraph/anm/AnimationInterp.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationInterp.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/anm/AnimationInterp.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationInterp.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/anm/AnimationInterp.d

# Compiles file ../../source/scenegraph/anm/AnimationUtil.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/anm/AnimationUtil.d
gccRelease_GLES2/source/scenegraph/anm/AnimationUtil.o: ../../source/scenegraph/anm/AnimationUtil.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationUtil.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/anm/AnimationUtil.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationUtil.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/anm/AnimationUtil.d

# Compiles file ../../source/scenegraph/anm/izAnimation.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/anm/izAnimation.d
gccRelease_GLES2/source/scenegraph/anm/izAnimation.o: ../../source/scenegraph/anm/izAnimation.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/izAnimation.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/anm/izAnimation.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/izAnimation.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/anm/izAnimation.d

# Compiles file ../../source/scenegraph/anm/PoseUpdater.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/anm/PoseUpdater.d
gccRelease_GLES2/source/scenegraph/anm/PoseUpdater.o: ../../source/scenegraph/anm/PoseUpdater.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/PoseUpdater.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/anm/PoseUpdater.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/PoseUpdater.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/anm/PoseUpdater.d

# Compiles file ../../source/scenegraph/anm/AnimationLinearBlender.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/anm/AnimationLinearBlender.d
gccRelease_GLES2/source/scenegraph/anm/AnimationLinearBlender.o: ../../source/scenegraph/anm/AnimationLinearBlender.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationLinearBlender.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/anm/AnimationLinearBlender.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationLinearBlender.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/anm/AnimationLinearBlender.d

# Compiles file ../../source/scenegraph/mtrl/izMaterial.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/mtrl/izMaterial.d
gccRelease_GLES2/source/scenegraph/mtrl/izMaterial.o: ../../source/scenegraph/mtrl/izMaterial.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/mtrl/izMaterial.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/mtrl/izMaterial.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/mtrl/izMaterial.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/mtrl/izMaterial.d

# Compiles file ../../source/scenegraph/msh/izMesh.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/msh/izMesh.d
gccRelease_GLES2/source/scenegraph/msh/izMesh.o: ../../source/scenegraph/msh/izMesh.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/izMesh.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/msh/izMesh.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/izMesh.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/msh/izMesh.d

# Compiles file ../../source/scenegraph/msh/MeshInstance.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/msh/MeshInstance.d
gccRelease_GLES2/source/scenegraph/msh/MeshInstance.o: ../../source/scenegraph/msh/MeshInstance.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MeshInstance.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/msh/MeshInstance.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MeshInstance.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/msh/MeshInstance.d

# Compiles file ../../source/scenegraph/msh/MeshSetInstance.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/msh/MeshSetInstance.d
gccRelease_GLES2/source/scenegraph/msh/MeshSetInstance.o: ../../source/scenegraph/msh/MeshSetInstance.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MeshSetInstance.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/msh/MeshSetInstance.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MeshSetInstance.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/msh/MeshSetInstance.d

# Compiles file ../../source/scenegraph/msh/MshMeshGroup.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/msh/MshMeshGroup.d
gccRelease_GLES2/source/scenegraph/msh/MshMeshGroup.o: ../../source/scenegraph/msh/MshMeshGroup.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MshMeshGroup.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/msh/MshMeshGroup.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MshMeshGroup.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/msh/MshMeshGroup.d

# Compiles file ../../source/scenegraph/msh/MshMeshSet.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/msh/MshMeshSet.d
gccRelease_GLES2/source/scenegraph/msh/MshMeshSet.o: ../../source/scenegraph/msh/MshMeshSet.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MshMeshSet.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/msh/MshMeshSet.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MshMeshSet.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/msh/MshMeshSet.d

# Compiles file ../../source/scenegraph/msh/MshPrimitiveSet.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/msh/MshPrimitiveSet.d
gccRelease_GLES2/source/scenegraph/msh/MshPrimitiveSet.o: ../../source/scenegraph/msh/MshPrimitiveSet.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/msh/MshPrimitiveSet.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/msh/MshPrimitiveSet.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/msh/MshPrimitiveSet.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/msh/MshPrimitiveSet.d

# Compiles file ../../source/scenegraph/skl/izSkeleton.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/skl/izSkeleton.d
gccRelease_GLES2/source/scenegraph/skl/izSkeleton.o: ../../source/scenegraph/skl/izSkeleton.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/skl/izSkeleton.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/skl/izSkeleton.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/skl/izSkeleton.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/skl/izSkeleton.d

# Compiles file ../../source/scenegraph/skl/SkeletonInstance.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/skl/SkeletonInstance.d
gccRelease_GLES2/source/scenegraph/skl/SkeletonInstance.o: ../../source/scenegraph/skl/SkeletonInstance.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/skl/SkeletonInstance.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/skl/SkeletonInstance.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/skl/SkeletonInstance.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/skl/SkeletonInstance.d

# Compiles file ../../source/scenegraph/skl/SkeletonUtil.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/skl/SkeletonUtil.d
gccRelease_GLES2/source/scenegraph/skl/SkeletonUtil.o: ../../source/scenegraph/skl/SkeletonUtil.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/skl/SkeletonUtil.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/skl/SkeletonUtil.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/skl/SkeletonUtil.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/skl/SkeletonUtil.d

# Compiles file ../../source/scenegraph/rendergraph/SceneRenderer.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/rendergraph/SceneRenderer.d
gccRelease_GLES2/source/scenegraph/rendergraph/SceneRenderer.o: ../../source/scenegraph/rendergraph/SceneRenderer.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/rendergraph/SceneRenderer.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/rendergraph/SceneRenderer.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/rendergraph/SceneRenderer.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/rendergraph/SceneRenderer.d

# Compiles file ../../source/scenegraph/rendergraph/RenderGraph.cpp for the Release_GLES2 configuration...
-include gccRelease_GLES2/source/scenegraph/rendergraph/RenderGraph.d
gccRelease_GLES2/source/scenegraph/rendergraph/RenderGraph.o: ../../source/scenegraph/rendergraph/RenderGraph.cpp
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -c ../../source/scenegraph/rendergraph/RenderGraph.cpp $(Release_GLES2_Include_Path) -o gccRelease_GLES2/source/scenegraph/rendergraph/RenderGraph.o
	$(CPP_COMPILER) $(Release_GLES2_Preprocessor_Definitions) $(Release_GLES2_Compiler_Flags) -MM ../../source/scenegraph/rendergraph/RenderGraph.cpp $(Release_GLES2_Include_Path) > gccRelease_GLES2/source/scenegraph/rendergraph/RenderGraph.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLES2/source/scenegraph
	mkdir -p gccDebug_GLES2/source/scenegraph/img
	mkdir -p gccDebug_GLES2/source/scenegraph/mdl
	mkdir -p gccDebug_GLES2/source/scenegraph/anm
	mkdir -p gccDebug_GLES2/source/scenegraph/mtrl
	mkdir -p gccDebug_GLES2/source/scenegraph/msh
	mkdir -p gccDebug_GLES2/source/scenegraph/skl
	mkdir -p gccDebug_GLES2/source/scenegraph/rendergraph
	mkdir -p ../lib/gccDebug_GLES2
	mkdir -p gccRelease_GLES2/source/scenegraph
	mkdir -p gccRelease_GLES2/source/scenegraph/img
	mkdir -p gccRelease_GLES2/source/scenegraph/mdl
	mkdir -p gccRelease_GLES2/source/scenegraph/anm
	mkdir -p gccRelease_GLES2/source/scenegraph/mtrl
	mkdir -p gccRelease_GLES2/source/scenegraph/msh
	mkdir -p gccRelease_GLES2/source/scenegraph/skl
	mkdir -p gccRelease_GLES2/source/scenegraph/rendergraph
	mkdir -p ../lib/gccRelease_GLES2

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLES2/source/scenegraph/*.o
	rm -f gccDebug_GLES2/source/scenegraph/*.d
	rm -f gccDebug_GLES2/source/scenegraph/img/*.o
	rm -f gccDebug_GLES2/source/scenegraph/img/*.d
	rm -f gccDebug_GLES2/source/scenegraph/mdl/*.o
	rm -f gccDebug_GLES2/source/scenegraph/mdl/*.d
	rm -f gccDebug_GLES2/source/scenegraph/anm/*.o
	rm -f gccDebug_GLES2/source/scenegraph/anm/*.d
	rm -f gccDebug_GLES2/source/scenegraph/mtrl/*.o
	rm -f gccDebug_GLES2/source/scenegraph/mtrl/*.d
	rm -f gccDebug_GLES2/source/scenegraph/msh/*.o
	rm -f gccDebug_GLES2/source/scenegraph/msh/*.d
	rm -f gccDebug_GLES2/source/scenegraph/skl/*.o
	rm -f gccDebug_GLES2/source/scenegraph/skl/*.d
	rm -f gccDebug_GLES2/source/scenegraph/rendergraph/*.o
	rm -f gccDebug_GLES2/source/scenegraph/rendergraph/*.d
	rm -f ../lib/gccDebug_GLES2/libSceneGraph.a
	rm -f gccRelease_GLES2/source/scenegraph/*.o
	rm -f gccRelease_GLES2/source/scenegraph/*.d
	rm -f gccRelease_GLES2/source/scenegraph/img/*.o
	rm -f gccRelease_GLES2/source/scenegraph/img/*.d
	rm -f gccRelease_GLES2/source/scenegraph/mdl/*.o
	rm -f gccRelease_GLES2/source/scenegraph/mdl/*.d
	rm -f gccRelease_GLES2/source/scenegraph/anm/*.o
	rm -f gccRelease_GLES2/source/scenegraph/anm/*.d
	rm -f gccRelease_GLES2/source/scenegraph/mtrl/*.o
	rm -f gccRelease_GLES2/source/scenegraph/mtrl/*.d
	rm -f gccRelease_GLES2/source/scenegraph/msh/*.o
	rm -f gccRelease_GLES2/source/scenegraph/msh/*.d
	rm -f gccRelease_GLES2/source/scenegraph/skl/*.o
	rm -f gccRelease_GLES2/source/scenegraph/skl/*.d
	rm -f gccRelease_GLES2/source/scenegraph/rendergraph/*.o
	rm -f gccRelease_GLES2/source/scenegraph/rendergraph/*.d
	rm -f ../lib/gccRelease_GLES2/libSceneGraph.a

