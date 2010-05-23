#if !defined(__URANUS_DEBUG_UTIL_MESH_CYLINDER_H__)
#define __URANUS_DEBUG_UTIL_MESH_CYLINDER_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	* �~��
	*/
	class CDebugMeshCylinder : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// �C���X�^���X�쐬
		static CDebugMeshCylinder* CreateDebugMeshCylinder(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_FLOAT fHeight,
			UN_UINT nSlices,	// �o�x�̕�����
			UN_UINT nStacks);

	protected:
		CDebugMeshCylinder() {}
		~CDebugMeshCylinder() {}

		NO_COPIABLE(CDebugMeshCylinder);

	protected:
		// ������
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_UINT nSlices,
			UN_UINT nStacks);

		// ���_�f�[�^�Z�b�g
		UN_BOOL SetVtx(
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_FLOAT fHeight,
			UN_UINT nSlices,
			UN_UINT nStacks);

		// ���_�f�[�^�v�Z
		void ComputeVtx(
			SMeshVtx* pVtx,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_FLOAT fHeight,
			UN_FLOAT fY,
			UN_FLOAT fLongitude);

		// �C���f�b�N�X�f�[�^�Z�b�g
		UN_BOOL SetIdx(
			UN_UINT nSlices,
			UN_UINT nStacks);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_CYLINDER_H__)
