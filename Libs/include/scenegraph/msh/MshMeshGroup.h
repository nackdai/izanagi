#if !defined(__IZANAGI_SCENEGRAPH_MESH_GROUP_H__)
#define __IZANAGI_SCENEGRAPH_MESH_GROUP_H__

#include "MSHFormat.h"

namespace izanagi {
	class CGraphicsDevice;
	class CVertexBuffer;
	class IInputStream;
	class CMeshSet;
	class IMshRenderHandler;
	class CSkeletonInstance;

	class CMeshGroup : public CObject {
		friend class CMesh;

	private:
		CMeshGroup();
		~CMeshGroup();

		NO_COPIABLE(CMeshGroup);

		void InternalRelease() { delete this; }

	public:
		IZ_UINT GetMeshSetNum() { return m_Info.numMeshSet; }

		CMeshSet* GetMeshSet(IZ_UINT idx);

		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			CSkeletonInstance* pSkl,
			IMshRenderHandler* pRenderHandler);

	private:
		IZ_UINT8* Read(
			IZ_UINT8* pBuf,
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

		IZ_BOOL ReadVertices(
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

	private:
		S_MSH_MESH_GROUP m_Info;

		CVertexBuffer** m_pVB;
		CMeshSet** m_pMeshSet;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MESH_GROUP_H__)
