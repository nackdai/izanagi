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
		// �C���X�^���X�쐬
		static CDebugMeshTorus* CreateDebugMeshTorus(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fInnerRadius,	// ���a
			UN_FLOAT fOuterRadius,	// �O�a
			UN_UINT nSides,			// ���f�ʂ̕ӂ̐�
			UN_UINT nRings);		// �g�[���X�Ɋ܂܂�郊���O�̐�

	protected:
		CDebugMeshTorus() {}
		~CDebugMeshTorus() {}

		NO_COPIABLE(CDebugMeshTorus);

	protected:
		// ������
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_UINT nSides, 
			UN_UINT nRings);

		// ���_�f�[�^�Z�b�g
		UN_BOOL SetVtx(
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fInnerRadius,	// ���a
			UN_FLOAT fOuterRadius,	// �O�a
			UN_UINT nSides,			// ���f�ʂ̕ӂ̐�
			UN_UINT nRings);		// �g�[���X�Ɋ܂܂�郊���O�̐�

		void ComputeVtx(
			SMeshVtx* pVtx,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fCenterRadius,	// �����O���S�܂ł̌a
			UN_FLOAT fRingRadius,	// �����O�̔��a
			UN_FLOAT fLongitude,	// �o�x
			UN_FLOAT fLatitude);	// �ܓx

		// �C���f�b�N�X�f�[�^�Z�b�g
		UN_BOOL SetIdx(
			UN_UINT nSides,			// ���f�ʂ̕ӂ̐�
			UN_UINT nRings);		// �g�[���X�Ɋ܂܂�郊���O�̐�
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_TORUS_H__)
