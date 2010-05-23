#if !defined(__URANUS_DEBUG_UTIL_MESH_GRID_H__)
#define __URANUS_DEBUG_UTIL_MESH_GRID_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	* グリッド
	*/
	class CDebugMeshGrid : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshGrid* CreateDebugMeshGrid(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_COLOR nColor,
			UN_UINT nGridNumX,
			UN_UINT nGridNumY,
			UN_FLOAT fGridSize);

	protected:
		CDebugMeshGrid() {}
		~CDebugMeshGrid() {}

		NO_COPIABLE(CDebugMeshGrid);

	protected:
		// 初期化
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_UINT nGridNumX,
			UN_UINT nGridNumY);

		// 頂点データセット
		UN_BOOL SetVtx(
			UN_UINT flag,
			UN_COLOR nColor,
			UN_UINT nGridNumX,
			UN_UINT nGridNumY,
			UN_FLOAT fGridSize);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_GRID_H__)
