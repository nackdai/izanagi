#if !defined(__URANUS_DEBUG_UTIL_MESH_AXIS_H__)
#define __URANUS_DEBUG_UTIL_MESH_AXIS_H__

#include "DebugMesh.h"

namespace uranus {
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
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_UINT nPointNum = 0);

		static CDebugMeshAxis* CreateDebugMeshAxisDefault(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice);

	protected:
		CDebugMeshAxis() : m_pVtxDst(UN_NULL), m_nAxisFlag(0), m_nSetPrimCnt(0) {}
		~CDebugMeshAxis();

		NO_COPIABLE(CDebugMeshAxis);

	protected:
		static const UN_UINT AXIS_VTX_FLAG = E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR;

		// 初期化
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag);

	public:
		// 描画
		UN_BOOL Draw();

		UN_BOOL BeginRegister();
		UN_BOOL EndRegister();

		UN_BOOL SetVtx(
			UN_UINT type,
			const SVector& start,
			const SVector& end);

	private:
		UN_UINT8* m_pVtxDst;
		UN_UINT m_nAxisFlag;
		UN_UINT m_nSetPrimCnt;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_AXIS_H__)
