#if !defined(__URANUS_DEBUG_UTIL_MESH_SPHERE_H__)
#define __URANUS_DEBUG_UTIL_MESH_BOX_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	* 球
	*/
	class CDebugMeshSphere : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshSphere* CreateDebugMeshSphere(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_UINT nSlices,	// 経度の分割数
			UN_UINT nStacks);	// 緯度の分割数

	protected:
		CDebugMeshSphere() {}
		~CDebugMeshSphere() {}

		NO_COPIABLE(CDebugMeshSphere);

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
			UN_UINT nSlices,
			UN_UINT nStacks);

		// 頂点データ計算
		void ComputeVtx(
			SMeshVtx* pVtx,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_FLOAT fLongitude,	// 経度
			UN_FLOAT fLatitude);	// 緯度

		// 面に頂点インデックスデータセット
		UN_BOOL SetIdx(
			UN_UINT nSlices,
			UN_UINT nStacks);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_BOX_H__)
