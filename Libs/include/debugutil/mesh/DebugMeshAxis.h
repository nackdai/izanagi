#if !defined(__IZANAGI_DEBUG_UTIL_MESH_AXIS_H__)
#define __IZANAGI_DEBUG_UTIL_MESH_AXIS_H__

#include "DebugMesh.h"

namespace izanagi {
	struct SVector;

	/**
	* 軸
	*/
	class CDebugMeshAxis : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// インスタンス作成
		static CDebugMeshAxis* CreateDebugMeshAxis(
			IMemoryAllocator* pAllocator,
			graph::CGraphicsDevice* pDevice,
			IZ_UINT flag,
			IZ_UINT nPointNum = 0);

		static CDebugMeshAxis* CreateDebugMeshAxisDefault(
			IMemoryAllocator* pAllocator,
			graph::CGraphicsDevice* pDevice);

	protected:
		CDebugMeshAxis() : m_pVtxDst(IZ_NULL), m_nAxisFlag(0), m_nSetPrimCnt(0) {}
		~CDebugMeshAxis();

		NO_COPIABLE(CDebugMeshAxis);

	protected:
		static const IZ_UINT AXIS_VTX_FLAG = E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR;

		// 初期化
		IZ_BOOL Init(
			graph::CGraphicsDevice* pDevice,
			IZ_UINT flag);

	public:
		// 描画
		IZ_BOOL Draw();

		IZ_BOOL BeginRegister();
		IZ_BOOL EndRegister();

		IZ_BOOL SetVtx(
			IZ_UINT type,
			const SVector& start,
			const SVector& end);

	private:
		IZ_UINT8* m_pVtxDst;
		IZ_UINT m_nAxisFlag;
		IZ_UINT m_nSetPrimCnt;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_DEBUG_UTIL_MESH_AXIS_H__)
