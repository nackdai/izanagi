#if !defined(__IZANAGI_SCENEGRAPH_SCENE_GRAPH_DEFS_H__)
#define __IZANAGI_SCENEGRAPH_SCENE_GRAPH_DEFS_H__

namespace izanagi {
	/**
	*/
	enum E_SCENEGRAPH_S3D_TARGET_TYPE {
		E_SCENEGRAPH_S3D_TARGET_TYPE_LEFT  = 0,
		E_SCENEGRAPH_S3D_TARGET_TYPE_RIGHT,

		E_SCENEGRAPH_S3D_TARGET_TYPE_NUM,
		E_SCENEGRAPH_S3D_TARGET_TYPE_FORCE_INT32 = 0x7fffffff,
	};

	/**
	*/
	enum E_SCENEGRAPH_S3D_RENDER_TYPE {
		E_SCENEGRAPH_S3D_RENDER_TYPE_NONE = 0,
		E_SCENEGRAPH_S3D_RENDER_TYPE_LEFT,
		E_SCENEGRAPH_S3D_RENDER_TYPE_RIGHT,

		E_SCENEGRAPH_S3D_RENDER_TYPE_NUM,
		E_SCENEGRAPH_S3D_RENDER_TYPE_FORCE_INT32 = 0x7fffffff,
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_SCENE_GRAPH_DEFS_H__)
