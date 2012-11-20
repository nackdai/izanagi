#if !defined(__IZANAGI_DEBUG_UTIL_MESH_GRID_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_GRID_H__

#include "DebugMesh.h"

namespace izanagi {
	/**
	* グリッド
	*/
	class CDebugMeshGrid : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshGrid* CreateDebugMeshGrid(
			IMemoryAllocator* pAllocator,
			graph::CGraphicsDevice* pDevice,
			IZ_COLOR nColor,
			IZ_UINT nGridNumX,
			IZ_UINT nGridNumY,
			IZ_FLOAT fGridSize);

	protected:
		CDebugMeshGrid() {}
		~CDebugMeshGrid() {}

		NO_COPIABLE(CDebugMeshGrid);

	protected:
		// 初期化
		IZ_BOOL Init(
			graph::CGraphicsDevice* pDevice,
			IZ_UINT flag,
			IZ_UINT nGridNumX,
			IZ_UINT nGridNumY);

		// 頂点データセット
		IZ_BOOL SetVtx(
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_UINT nGridNumX,
			IZ_UINT nGridNumY,
			IZ_FLOAT fGridSize);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUG_UTIL_MESH_GRID_H__)
