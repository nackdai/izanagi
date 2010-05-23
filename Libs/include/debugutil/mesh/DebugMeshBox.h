#if !defined(__URANUS_DEBUG_UTIL_MESH_BOX_H__)
#define __URANUS_DEBUG_UTIL_MESH_BOX_H__

#include "DebugMesh.h"

namespace uranus {
	/**
	* �{�b�N�X
	*/
	class CDebugMeshBox : public CDebugMesh {
		friend class CDebugMesh;

	public:
		// �C���X�^���X�쐬
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
		// ������
		UN_BOOL Init(
			CGraphicsDevice* pDevice,
			UN_UINT flag);

		// �f�[�^�Z�b�g
		UN_BOOL SetData(
			CGraphicsDevice* pDevice,
			UN_UINT flag,
			UN_COLOR nColor,
			UN_FLOAT fWidth,
			UN_FLOAT fHeight,
			UN_FLOAT fDepth);

		// �C���f�b�N�X�f�[�^�Z�b�g
		UN_BOOL SetIdx();
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_DEBUG_UTIL_MESH_BOX_H__)
