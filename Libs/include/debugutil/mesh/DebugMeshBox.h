#if !defined(__URANUS_DEBUG_UTIL_MESH_BOX_H__)
#define __URANUS_DEBUG_UTIL_MESH_BOX_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	* ボックス
	*/
	class CDebugMeshBox : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshBox* CreateDebugMeshBox(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fWidth,
			UN_FLOAT fHeight,
			UN_FLOAT fDepth);

	protected:
		CDebugMeshBox() {}
		~CDebugMeshBox() {}

		NO_COPIABLE(CDebugMeshBox);

	protected:
		// 初期化
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag);

		// データセット
		UN_BOOL SetData(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fWidth,
			UN_FLOAT fHeight,
			UN_FLOAT fDepth);

		// インデックスデータセット
		UN_BOOL SetIdx();
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_BOX_H__)
