#if !defined(__URANUS_DEBUG_UTIL_MESH_CYLINDER_H__)
#define __URANUS_DEBUG_UTIL_MESH_CYLINDER_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	* 円柱
	*/
	class CDebugMeshCylinder : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshCylinder* CreateDebugMeshCylinder(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_FLOAT fHeight,
			UN_UINT nSlices,	// 経度の分割数
			UN_UINT nStacks);

	protected:
		CDebugMeshCylinder() {}
		~CDebugMeshCylinder() {}

		NO_COPIABLE(CDebugMeshCylinder);

	protected:
		// 初期化
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_UINT nSlices,
			UN_UINT nStacks);

		// 頂点データセット
		UN_BOOL SetVtx(
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_FLOAT fHeight,
			UN_UINT nSlices,
			UN_UINT nStacks);

		// 頂点データ計算
		void ComputeVtx(
			SMeshVtx* pVtx,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_FLOAT fHeight,
			UN_FLOAT fY,
			UN_FLOAT fLongitude);

		// インデックスデータセット
		UN_BOOL SetIdx(
			UN_UINT nSlices,
			UN_UINT nStacks);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_CYLINDER_H__)
