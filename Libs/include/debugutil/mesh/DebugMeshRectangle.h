#if !defined(__IZANAGI_DEBUG_UTIL_MESH_RECTANGLE_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_RECTANGLE_H__

#include "DebugMesh.h"

namespace izanagi {
	/**
	*/
	class CDebugMeshRectangle : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshRectangle* CreateDebugMeshRectangle(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_UINT nDivideX,
			IZ_UINT nDivideY,
			IZ_FLOAT fWidth,
			IZ_FLOAT fHeight);

	protected:
		CDebugMeshRectangle() {}
		~CDebugMeshRectangle() {}

		NO_COPIABLE(CDebugMeshRectangle);

	protected:
		// 初期化
		IZ_BOOL Init(
			CGraphicsDevice* pDevice,
			IZ_UINT flag,
			IZ_UINT nDivideX,
			IZ_UINT nDivideY,
			IZ_FLOAT fWidth,
			IZ_FLOAT fHeight);

		// 頂点データセット
		IZ_BOOL SetVtx(
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_UINT nDivideX,
			IZ_UINT nDivideY,
			IZ_FLOAT fWidth,
			IZ_FLOAT fHeight);

		// 面に頂点インデックスデータセット
		IZ_BOOL SetIdx(
			IZ_UINT nDivideX,
			IZ_UINT nDivideY);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUG_UTIL_MESH_RECTANGLE_H__)
