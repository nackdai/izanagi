#if !defined(__URANUS_DEBUG_UTIL_MESH_FRUSTUM_H__)
#define __URANUS_DEBUG_UTIL_MESH_FRUSTUM_H__

#include "unDefs.h"
#include "DebugMesh.h"

namespace uranus {
	/**
	*/
	class CDebugMeshFrustum : public CDebugMesh {
		friend class CDebugMesh;

	public:
		static CDebugMeshFrustum* CreateDebugMeshFrustum(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_COLOR nColor,
			UN_FLOAT fAspect,
			UN_FLOAT fFov,
			UN_FLOAT fNear, UN_FLOAT fFar);

	protected:
		CDebugMeshFrustum() {}
		~CDebugMeshFrustum() {}

		NO_COPIABLE(CDebugMeshFrustum);

	protected:
		enum {
			VTX_NUM = 3 * 4 + 3 * 2,

			VTX_FORM_FLAG = E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR,
		};

	protected:
		UN_BOOL Init();

		UN_BOOL SetVtx(
			UN_COLOR nColor,
			UN_FLOAT fAspect,
			UN_FLOAT fFov,
			UN_FLOAT fNear, UN_FLOAT fFar);

	public:
		// •`‰æ
		//virtual UN_BOOL Draw(UN_BOOL bEnableDrawDebugAxis = UN_FALSE);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_FRUSTUM_H__)
