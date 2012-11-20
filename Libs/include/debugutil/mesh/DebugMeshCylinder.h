#if !defined(__IZANAGI_DEBUG_UTIL_MESH_CYLINDER_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_CYLINDER_H__

#include "DebugMesh.h"

namespace izanagi {
	/**
	* 円柱
	*/
	class CDebugMeshCylinder : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshCylinder* CreateDebugMeshCylinder(
			IMemoryAllocator* pAllocator,
			graph::CGraphicsDevice* pDevice,
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_FLOAT fRadius,
			IZ_FLOAT fHeight,
			IZ_UINT nSlices,	// 経度の分割数
			IZ_UINT nStacks);

	protected:
		CDebugMeshCylinder() {}
		~CDebugMeshCylinder() {}

		NO_COPIABLE(CDebugMeshCylinder);

	protected:
		// 初期化
		IZ_BOOL Init(
			graph::CGraphicsDevice* pDevice,
			IZ_UINT flag,
			IZ_UINT nSlices,
			IZ_UINT nStacks);

		// 頂点データセット
		IZ_BOOL SetVtx(
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_FLOAT fRadius,
			IZ_FLOAT fHeight,
			IZ_UINT nSlices,
			IZ_UINT nStacks);

		// 頂点データ計算
		void ComputeVtx(
			SMeshVtx* pVtx,
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_FLOAT fRadius,
			IZ_FLOAT fHeight,
			IZ_FLOAT fY,
			IZ_FLOAT fLongitude);

		// インデックスデータセット
		IZ_BOOL SetIdx(
			IZ_UINT nSlices,
			IZ_UINT nStacks);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUG_UTIL_MESH_CYLINDER_H__)
