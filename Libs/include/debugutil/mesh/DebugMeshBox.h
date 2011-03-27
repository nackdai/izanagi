#if !defined(__IZANAGI_DEBUG_UTIL_MESH_BOX_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_BOX_H__

#include "DebugMesh.h"

namespace izanagi {
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
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_FLOAT fWidth,
			IZ_FLOAT fHeight,
			IZ_FLOAT fDepth);

	protected:
		CDebugMeshBox() {}
		~CDebugMeshBox() {}

		NO_COPIABLE(CDebugMeshBox);

	protected:
		// 初期化
		IZ_BOOL Init(
			CGraphicsDevice* pDevice,
			IZ_UINT flag);

		// データセット
		IZ_BOOL SetData(
			CGraphicsDevice* pDevice,
			IZ_UINT flag,
			IZ_COLOR nColor,
			IZ_FLOAT fWidth,
			IZ_FLOAT fHeight,
			IZ_FLOAT fDepth);

		// インデックスデータセット
		IZ_BOOL SetIdx();
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUG_UTIL_MESH_BOX_H__)
