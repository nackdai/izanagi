#if !defined(__IZANAGI_SCENE_GRAPH_H__)
#define __IZANAGI_SCENE_GRAPH_H__

#include "scenegraph/SceneGraphUtil.h"
#include "scenegraph/SceneParam.h"

// camera
#include "scenegraph/Camera.h"
#include "scenegraph/VectorCamera.h"
#include "scenegraph/StereoCamera.h"

// state
#include "scenegraph/SceneState.h"
#include "scenegraph/FixedSceneStateManager.h"

// image
#include "scenegraph/img/IMGFormat.h"
#include "scenegraph/img/izImage.h"

// mesh
#include "scenegraph/msh/MSHFormat.h"
#include "scenegraph/msh/izMesh.h"
#include "scenegraph/msh/MeshInstance.h"

// skeleton
#include "scenegraph/skl/SKLFormat.h"
#include "scenegraph/skl/izSkeleton.h"
#include "scenegraph/skl/SkeletonInstance.h"

// model
#include "scenegraph/mdl/izModel.h"
#include "scenegraph/mdl/MDLFormat.h"

// rendergraph
#include "scenegraph/rendergraph/RenderGraph.h"
#include "scenegraph/rendergraph/SceneRenderer.h"

// animation
#include "scenegraph/anm/ANMFormat.h"
#include "scenegraph/anm/AnimationInterface.h"
#include "scenegraph/anm/izAnimation.h"
#include "scenegraph/anm/AnimationUtil.h"
#include "scenegraph/anm/AnimationInterp.h"
#include "scenegraph/anm/AnimationBinder.h"
#include "scenegraph/anm/AnimationBlender.h"
#include "scenegraph/anm/AnimationLinearBlender.h"

// material
#include "scenegraph/mtrl/MTRLFormat.h"
#include "scenegraph/mtrl/izMaterial.h"

// decal
#include "scenegraph/Decal.h"

#endif  // #if !defined(__IZANAGI_SCENE_GRAPH_H__)
