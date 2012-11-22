#if !defined(__IZANAGI_DEBUG_UTIL_MESH_SPHERE_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_BOX_H__

#include "DebugMesh.h"

namespace izanagi {
	/**
	* 球
	*/
	class CDebugMeshSphere : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshSphere* CreateDebugMeshSphere(
			IMemoryAllocator* pAllocator,
			graph::CGraphicsDevice* pDevice,
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_FLOAT fRadius,
			IZ_UINT nSlices,	// 経度の分割数
			IZ_UINT nStacks);	// 緯度の分割数

	protected:
		CDebugMeshSphere() {}
		~CDebugMeshSphere() {}

		NO_COPIABLE(CDebugMeshSphere);

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
			IZ_UINT nSlices,
			IZ_UINT nStacks);

		// 頂点データ計算
		void ComputeVtx(
			SMeshVtx* pVtx,
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_FLOAT fRadius,
			IZ_FLOAT fLongitude,	// 経度
			IZ_FLOAT fLatitude);	// 緯度

		// 面に頂点インデックスデータセット
		IZ_BOOL SetIdx(
			IZ_UINT nSlices,
			IZ_UINT nStacks);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUG_UTIL_MESH_BOX_H__)
