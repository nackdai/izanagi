#if !defined(__URANUS_DEBUG_UTIL_MESH_SPHERE_H__)
#define __URANUS_DEBUG_UTIL_MESH_BOX_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	* ��
	*/
	class CDebugMeshSphere : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// �C���X�^���X�쐬
		static CDebugMeshSphere* CreateDebugMeshSphere(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_UINT nSlices,	// �o�x�̕�����
			UN_UINT nStacks);	// �ܓx�̕�����

	protected:
		CDebugMeshSphere() {}
		~CDebugMeshSphere() {}

		NO_COPIABLE(CDebugMeshSphere);

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
			UN_UINT nSlices,
			UN_UINT nStacks);

		// ���_�f�[�^�v�Z
		void ComputeVtx(
			SMeshVtx* pVtx,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fRadius,
			UN_FLOAT fLongitude,	// �o�x
			UN_FLOAT fLatitude);	// �ܓx

		// �ʂɒ��_�C���f�b�N�X�f�[�^�Z�b�g
		UN_BOOL SetIdx(
			UN_UINT nSlices,
			UN_UINT nStacks);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_BOX_H__)
