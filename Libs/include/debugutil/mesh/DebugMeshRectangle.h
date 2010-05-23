#if !defined(__URANUS_DEBUG_UTIL_MESH_RECTANGLE_H__)
#define __URANUS_DEBUG_UTIL_MESH_RECTANGLE_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	*/
	class CDebugMeshRectangle : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// �C���X�^���X�쐬
		static CDebugMeshRectangle* CreateDebugMeshRectangle(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_UINT nDivideX,
			UN_UINT nDivideY,
			UN_FLOAT fWidth,
			UN_FLOAT fHeight);

	protected:
		CDebugMeshRectangle() {}
		~CDebugMeshRectangle() {}

		NO_COPIABLE(CDebugMeshRectangle);

	protected:
		// ������
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_UINT nDivideX,
			UN_UINT nDivideY,
			UN_FLOAT fWidth,
			UN_FLOAT fHeight);

		// ���_�f�[�^�Z�b�g
		UN_BOOL SetVtx(
			UN_UINT flag,
			UN_COLOR nColor,
			UN_UINT nDivideX,
			UN_UINT nDivideY,
			UN_FLOAT fWidth,
			UN_FLOAT fHeight);

		// �ʂɒ��_�C���f�b�N�X�f�[�^�Z�b�g
		UN_BOOL SetIdx(
			UN_UINT nDivideX,
			UN_UINT nDivideY);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_RECTANGLE_H__)
