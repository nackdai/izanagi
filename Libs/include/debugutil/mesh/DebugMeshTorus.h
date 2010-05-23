#if !defined(__URANUS_DEBUG_UTIL_MESH_TORUS_H__)
#define __URANUS_DEBUG_UTIL_MESH_TORUS_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	* Torus
	*/
	class CDebugMeshTorus : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshTorus* CreateDebugMeshTorus(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fInnerRadius,	// 内径
			UN_FLOAT fOuterRadius,	// 外径
			UN_UINT nSides,			// 横断面の辺の数
			UN_UINT nRings);		// トーラスに含まれるリングの数

	protected:
		CDebugMeshTorus() {}
		~CDebugMeshTorus() {}

		NO_COPIABLE(CDebugMeshTorus);

	protected:
		// 初期化
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_UINT nSides, 
			UN_UINT nRings);

		// 頂点データセット
		UN_BOOL SetVtx(
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fInnerRadius,	// 内径
			UN_FLOAT fOuterRadius,	// 外径
			UN_UINT nSides,			// 横断面の辺の数
			UN_UINT nRings);		// トーラスに含まれるリングの数

		void ComputeVtx(
			SMeshVtx* pVtx,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fCenterRadius,	// リング中心までの径
			UN_FLOAT fRingRadius,	// リングの半径
			UN_FLOAT fLongitude,	// 経度
			UN_FLOAT fLatitude);	// 緯度

		// インデックスデータセット
		UN_BOOL SetIdx(
			UN_UINT nSides,			// 横断面の辺の数
			UN_UINT nRings);		// トーラスに含まれるリングの数
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_TORUS_H__)
