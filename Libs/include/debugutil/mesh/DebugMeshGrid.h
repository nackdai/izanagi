#if !defined(__URANUS_DEBUG_UTIL_MESH_GRID_H__)
#define __URANUS_DEBUG_UTIL_MESH_GRID_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	* �O���b�h
	*/
	class CDebugMeshGrid : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// �C���X�^���X�쐬
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
		// ������
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_UINT nGridNumX,
			UN_UINT nGridNumY);

		// ���_�f�[�^�Z�b�g
		UN_BOOL SetVtx(
			UN_UINT flag,
			UN_COLOR nColor,
			UN_UINT nGridNumX,
			UN_UINT nGridNumY,
			UN_FLOAT fGridSize);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_GRID_H__)
