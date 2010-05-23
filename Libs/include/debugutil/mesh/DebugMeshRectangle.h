#if !defined(__URANUS_DEBUG_UTIL_MESH_RECTANGLE_H__)
#define __URANUS_DEBUG_UTIL_MESH_RECTANGLE_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	*/
	class CDebugMeshRectangle : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshRectangle* CreateDebugMeshRectangle(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_UINT nDivideX,
			UN_UINT nDivideY,
			UN_FLOAT fWidth,
			UN_FLOAT fHeight);

	protected:
		CDebugMeshRectangle() {}
		~CDebugMeshRectangle() {}

		NO_COPIABLE(CDebugMeshRectangle);

	protected:
		// 初期化
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_UINT nDivideX,
			UN_UINT nDivideY,
			UN_FLOAT fWidth,
			UN_FLOAT fHeight);

		// 頂点データセット
		UN_BOOL SetVtx(
			UN_UINT flag,
			UN_COLOR nColor,
			UN_UINT nDivideX,
			UN_UINT nDivideY,
			UN_FLOAT fWidth,
			UN_FLOAT fHeight);

		// 面に頂点インデックスデータセット
		UN_BOOL SetIdx(
			UN_UINT nDivideX,
			UN_UINT nDivideY);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_RECTANGLE_H__)
