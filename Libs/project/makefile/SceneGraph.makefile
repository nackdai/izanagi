# Compiler flags...
CPP_COMPILER = g++
C_COMPILER = gcc

# Include paths...
Debug_GLUT_Include_Path=-I"../../include" -I"../../../External/glew/include" -I"../../../External/freeglut/include" 

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
Debug_GLUT: create_folders gccDebug_GLUT/source/scenegraph/Camera.o gccDebug_GLUT/source/scenegraph/Decal.o gccDebug_GLUT/source/scenegraph/SceneGraphUtil.o gccDebug_GLUT/source/scenegraph/SceneParam.o gccDebug_GLUT/source/scenegraph/SceneState.o gccDebug_GLUT/source/scenegraph/VectorCamera.o gccDebug_GLUT/source/scenegraph/img/izImage.o gccDebug_GLUT/source/scenegraph/mdl/izModel.o gccDebug_GLUT/source/scenegraph/anm/AnimationBinder.o gccDebug_GLUT/source/scenegraph/anm/AnimationInterface.o gccDebug_GLUT/source/scenegraph/anm/AnimationInterp.o gccDebug_GLUT/source/scenegraph/anm/AnimationUtil.o gccDebug_GLUT/source/scenegraph/anm/izAnimation.o gccDebug_GLUT/source/scenegraph/anm/PoseUpdater.o gccDebug_GLUT/source/scenegraph/anm/AnimationLinearBlender.o gccDebug_GLUT/source/scenegraph/mtrl/izMaterial.o gccDebug_GLUT/source/scenegraph/msh/izMesh.o gccDebug_GLUT/source/scenegraph/msh/MeshInstance.o gccDebug_GLUT/source/scenegraph/msh/MeshSetInstance.o gccDebug_GLUT/source/scenegraph/msh/MshMeshGroup.o gccDebug_GLUT/source/scenegraph/msh/MshMeshSet.o gccDebug_GLUT/source/scenegraph/msh/MshPrimitiveSet.o gccDebug_GLUT/source/scenegraph/skl/izSkeleton.o gccDebug_GLUT/source/scenegraph/skl/SkeletonInstance.o gccDebug_GLUT/source/scenegraph/skl/SkeletonUtil.o gccDebug_GLUT/source/scenegraph/rendergraph/SceneRenderer.o gccDebug_GLUT/source/scenegraph/rendergraph/RenderGraph.o 
	ar rcs ../lib/gccDebug_GLUT/libSceneGraph.a gccDebug_GLUT/source/scenegraph/Camera.o gccDebug_GLUT/source/scenegraph/Decal.o gccDebug_GLUT/source/scenegraph/SceneGraphUtil.o gccDebug_GLUT/source/scenegraph/SceneParam.o gccDebug_GLUT/source/scenegraph/SceneState.o gccDebug_GLUT/source/scenegraph/VectorCamera.o gccDebug_GLUT/source/scenegraph/img/izImage.o gccDebug_GLUT/source/scenegraph/mdl/izModel.o gccDebug_GLUT/source/scenegraph/anm/AnimationBinder.o gccDebug_GLUT/source/scenegraph/anm/AnimationInterface.o gccDebug_GLUT/source/scenegraph/anm/AnimationInterp.o gccDebug_GLUT/source/scenegraph/anm/AnimationUtil.o gccDebug_GLUT/source/scenegraph/anm/izAnimation.o gccDebug_GLUT/source/scenegraph/anm/PoseUpdater.o gccDebug_GLUT/source/scenegraph/anm/AnimationLinearBlender.o gccDebug_GLUT/source/scenegraph/mtrl/izMaterial.o gccDebug_GLUT/source/scenegraph/msh/izMesh.o gccDebug_GLUT/source/scenegraph/msh/MeshInstance.o gccDebug_GLUT/source/scenegraph/msh/MeshSetInstance.o gccDebug_GLUT/source/scenegraph/msh/MshMeshGroup.o gccDebug_GLUT/source/scenegraph/msh/MshMeshSet.o gccDebug_GLUT/source/scenegraph/msh/MshPrimitiveSet.o gccDebug_GLUT/source/scenegraph/skl/izSkeleton.o gccDebug_GLUT/source/scenegraph/skl/SkeletonInstance.o gccDebug_GLUT/source/scenegraph/skl/SkeletonUtil.o gccDebug_GLUT/source/scenegraph/rendergraph/SceneRenderer.o gccDebug_GLUT/source/scenegraph/rendergraph/RenderGraph.o  $(Debug_GLUT_Implicitly_Linked_Objects)

# Compiles file ../../source/scenegraph/Camera.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/Camera.d
gccDebug_GLUT/source/scenegraph/Camera.o: ../../source/scenegraph/Camera.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/Camera.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/Camera.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/Camera.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/Camera.d

# Compiles file ../../source/scenegraph/Decal.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/Decal.d
gccDebug_GLUT/source/scenegraph/Decal.o: ../../source/scenegraph/Decal.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/Decal.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/Decal.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/Decal.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/Decal.d

# Compiles file ../../source/scenegraph/SceneGraphUtil.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/SceneGraphUtil.d
gccDebug_GLUT/source/scenegraph/SceneGraphUtil.o: ../../source/scenegraph/SceneGraphUtil.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/SceneGraphUtil.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/SceneGraphUtil.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/SceneGraphUtil.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/SceneGraphUtil.d

# Compiles file ../../source/scenegraph/SceneParam.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/SceneParam.d
gccDebug_GLUT/source/scenegraph/SceneParam.o: ../../source/scenegraph/SceneParam.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/SceneParam.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/SceneParam.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/SceneParam.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/SceneParam.d

# Compiles file ../../source/scenegraph/SceneState.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/SceneState.d
gccDebug_GLUT/source/scenegraph/SceneState.o: ../../source/scenegraph/SceneState.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/SceneState.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/SceneState.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/SceneState.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/SceneState.d

# Compiles file ../../source/scenegraph/VectorCamera.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/VectorCamera.d
gccDebug_GLUT/source/scenegraph/VectorCamera.o: ../../source/scenegraph/VectorCamera.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/VectorCamera.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/VectorCamera.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/VectorCamera.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/VectorCamera.d

# Compiles file ../../source/scenegraph/img/izImage.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/img/izImage.d
gccDebug_GLUT/source/scenegraph/img/izImage.o: ../../source/scenegraph/img/izImage.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/img/izImage.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/img/izImage.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/img/izImage.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/img/izImage.d

# Compiles file ../../source/scenegraph/mdl/izModel.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/mdl/izModel.d
gccDebug_GLUT/source/scenegraph/mdl/izModel.o: ../../source/scenegraph/mdl/izModel.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/mdl/izModel.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/mdl/izModel.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/mdl/izModel.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/mdl/izModel.d

# Compiles file ../../source/scenegraph/anm/AnimationBinder.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/anm/AnimationBinder.d
gccDebug_GLUT/source/scenegraph/anm/AnimationBinder.o: ../../source/scenegraph/anm/AnimationBinder.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationBinder.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/anm/AnimationBinder.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationBinder.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/anm/AnimationBinder.d

# Compiles file ../../source/scenegraph/anm/AnimationInterface.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/anm/AnimationInterface.d
gccDebug_GLUT/source/scenegraph/anm/AnimationInterface.o: ../../source/scenegraph/anm/AnimationInterface.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationInterface.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/anm/AnimationInterface.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationInterface.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/anm/AnimationInterface.d

# Compiles file ../../source/scenegraph/anm/AnimationInterp.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/anm/AnimationInterp.d
gccDebug_GLUT/source/scenegraph/anm/AnimationInterp.o: ../../source/scenegraph/anm/AnimationInterp.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationInterp.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/anm/AnimationInterp.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationInterp.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/anm/AnimationInterp.d

# Compiles file ../../source/scenegraph/anm/AnimationUtil.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/anm/AnimationUtil.d
gccDebug_GLUT/source/scenegraph/anm/AnimationUtil.o: ../../source/scenegraph/anm/AnimationUtil.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationUtil.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/anm/AnimationUtil.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationUtil.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/anm/AnimationUtil.d

# Compiles file ../../source/scenegraph/anm/izAnimation.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/anm/izAnimation.d
gccDebug_GLUT/source/scenegraph/anm/izAnimation.o: ../../source/scenegraph/anm/izAnimation.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/anm/izAnimation.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/anm/izAnimation.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/anm/izAnimation.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/anm/izAnimation.d

# Compiles file ../../source/scenegraph/anm/PoseUpdater.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/anm/PoseUpdater.d
gccDebug_GLUT/source/scenegraph/anm/PoseUpdater.o: ../../source/scenegraph/anm/PoseUpdater.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/anm/PoseUpdater.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/anm/PoseUpdater.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/anm/PoseUpdater.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/anm/PoseUpdater.d

# Compiles file ../../source/scenegraph/anm/AnimationLinearBlender.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/anm/AnimationLinearBlender.d
gccDebug_GLUT/source/scenegraph/anm/AnimationLinearBlender.o: ../../source/scenegraph/anm/AnimationLinearBlender.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/anm/AnimationLinearBlender.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/anm/AnimationLinearBlender.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/anm/AnimationLinearBlender.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/anm/AnimationLinearBlender.d

# Compiles file ../../source/scenegraph/mtrl/izMaterial.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/mtrl/izMaterial.d
gccDebug_GLUT/source/scenegraph/mtrl/izMaterial.o: ../../source/scenegraph/mtrl/izMaterial.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/mtrl/izMaterial.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/mtrl/izMaterial.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/mtrl/izMaterial.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/mtrl/izMaterial.d

# Compiles file ../../source/scenegraph/msh/izMesh.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/msh/izMesh.d
gccDebug_GLUT/source/scenegraph/msh/izMesh.o: ../../source/scenegraph/msh/izMesh.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/msh/izMesh.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/msh/izMesh.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/msh/izMesh.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/msh/izMesh.d

# Compiles file ../../source/scenegraph/msh/MeshInstance.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/msh/MeshInstance.d
gccDebug_GLUT/source/scenegraph/msh/MeshInstance.o: ../../source/scenegraph/msh/MeshInstance.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/msh/MeshInstance.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/msh/MeshInstance.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/msh/MeshInstance.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/msh/MeshInstance.d

# Compiles file ../../source/scenegraph/msh/MeshSetInstance.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/msh/MeshSetInstance.d
gccDebug_GLUT/source/scenegraph/msh/MeshSetInstance.o: ../../source/scenegraph/msh/MeshSetInstance.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/msh/MeshSetInstance.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/msh/MeshSetInstance.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/msh/MeshSetInstance.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/msh/MeshSetInstance.d

# Compiles file ../../source/scenegraph/msh/MshMeshGroup.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/msh/MshMeshGroup.d
gccDebug_GLUT/source/scenegraph/msh/MshMeshGroup.o: ../../source/scenegraph/msh/MshMeshGroup.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/msh/MshMeshGroup.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/msh/MshMeshGroup.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/msh/MshMeshGroup.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/msh/MshMeshGroup.d

# Compiles file ../../source/scenegraph/msh/MshMeshSet.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/msh/MshMeshSet.d
gccDebug_GLUT/source/scenegraph/msh/MshMeshSet.o: ../../source/scenegraph/msh/MshMeshSet.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/msh/MshMeshSet.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/msh/MshMeshSet.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/msh/MshMeshSet.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/msh/MshMeshSet.d

# Compiles file ../../source/scenegraph/msh/MshPrimitiveSet.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/msh/MshPrimitiveSet.d
gccDebug_GLUT/source/scenegraph/msh/MshPrimitiveSet.o: ../../source/scenegraph/msh/MshPrimitiveSet.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/msh/MshPrimitiveSet.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/msh/MshPrimitiveSet.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/msh/MshPrimitiveSet.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/msh/MshPrimitiveSet.d

# Compiles file ../../source/scenegraph/skl/izSkeleton.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/skl/izSkeleton.d
gccDebug_GLUT/source/scenegraph/skl/izSkeleton.o: ../../source/scenegraph/skl/izSkeleton.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/skl/izSkeleton.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/skl/izSkeleton.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/skl/izSkeleton.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/skl/izSkeleton.d

# Compiles file ../../source/scenegraph/skl/SkeletonInstance.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/skl/SkeletonInstance.d
gccDebug_GLUT/source/scenegraph/skl/SkeletonInstance.o: ../../source/scenegraph/skl/SkeletonInstance.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/skl/SkeletonInstance.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/skl/SkeletonInstance.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/skl/SkeletonInstance.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/skl/SkeletonInstance.d

# Compiles file ../../source/scenegraph/skl/SkeletonUtil.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/skl/SkeletonUtil.d
gccDebug_GLUT/source/scenegraph/skl/SkeletonUtil.o: ../../source/scenegraph/skl/SkeletonUtil.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/skl/SkeletonUtil.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/skl/SkeletonUtil.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/skl/SkeletonUtil.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/skl/SkeletonUtil.d

# Compiles file ../../source/scenegraph/rendergraph/SceneRenderer.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/rendergraph/SceneRenderer.d
gccDebug_GLUT/source/scenegraph/rendergraph/SceneRenderer.o: ../../source/scenegraph/rendergraph/SceneRenderer.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/rendergraph/SceneRenderer.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/rendergraph/SceneRenderer.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/rendergraph/SceneRenderer.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/rendergraph/SceneRenderer.d

# Compiles file ../../source/scenegraph/rendergraph/RenderGraph.cpp for the Debug_GLUT configuration...
-include gccDebug_GLUT/source/scenegraph/rendergraph/RenderGraph.d
gccDebug_GLUT/source/scenegraph/rendergraph/RenderGraph.o: ../../source/scenegraph/rendergraph/RenderGraph.cpp
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -c ../../source/scenegraph/rendergraph/RenderGraph.cpp $(Debug_GLUT_Include_Path) -o gccDebug_GLUT/source/scenegraph/rendergraph/RenderGraph.o
	$(CPP_COMPILER) $(Debug_GLUT_Preprocessor_Definitions) $(Debug_GLUT_Compiler_Flags) -MM ../../source/scenegraph/rendergraph/RenderGraph.cpp $(Debug_GLUT_Include_Path) > gccDebug_GLUT/source/scenegraph/rendergraph/RenderGraph.d

# Creates the intermediate and output folders for each configuration...
.PHONY: create_folders
create_folders:
	mkdir -p gccDebug_GLUT/source/scenegraph
	mkdir -p gccDebug_GLUT/source/scenegraph/img
	mkdir -p gccDebug_GLUT/source/scenegraph/mdl
	mkdir -p gccDebug_GLUT/source/scenegraph/anm
	mkdir -p gccDebug_GLUT/source/scenegraph/mtrl
	mkdir -p gccDebug_GLUT/source/scenegraph/msh
	mkdir -p gccDebug_GLUT/source/scenegraph/skl
	mkdir -p gccDebug_GLUT/source/scenegraph/rendergraph
	mkdir -p ../lib/gccDebug_GLUT

# Cleans intermediate and output files (objects, libraries, executables)...
.PHONY: clean
clean:
	rm -f gccDebug_GLUT/source/scenegraph/*.o
	rm -f gccDebug_GLUT/source/scenegraph/*.d
	rm -f gccDebug_GLUT/source/scenegraph/img/*.o
	rm -f gccDebug_GLUT/source/scenegraph/img/*.d
	rm -f gccDebug_GLUT/source/scenegraph/mdl/*.o
	rm -f gccDebug_GLUT/source/scenegraph/mdl/*.d
	rm -f gccDebug_GLUT/source/scenegraph/anm/*.o
	rm -f gccDebug_GLUT/source/scenegraph/anm/*.d
	rm -f gccDebug_GLUT/source/scenegraph/mtrl/*.o
	rm -f gccDebug_GLUT/source/scenegraph/mtrl/*.d
	rm -f gccDebug_GLUT/source/scenegraph/msh/*.o
	rm -f gccDebug_GLUT/source/scenegraph/msh/*.d
	rm -f gccDebug_GLUT/source/scenegraph/skl/*.o
	rm -f gccDebug_GLUT/source/scenegraph/skl/*.d
	rm -f gccDebug_GLUT/source/scenegraph/rendergraph/*.o
	rm -f gccDebug_GLUT/source/scenegraph/rendergraph/*.d
	rm -f ../lib/gccDebug_GLUT/libSceneGraph.a

