#if !defined(__IZANAGI_SCENEGRAPH_MSH_MESH_SET_H__)
#define __IZANAGI_SCENEGRAPH_MSH_MESH_SET_H__

#include "MSHFormat.h"

namespace izanagi {
	class CGraphicsDevice;
	class CVertexDeclaration;
	class IInputStream;
	class CPrimitiveSet;
	class CSkeletonInstance;
	class IMshRenderHandler;

	class CMeshSet : public CObject {
		friend class CMesh;
		friend class CMeshGroup;

	private:
		CMeshSet();
		~CMeshSet();

		NO_COPIABLE(CMeshSet);

		void InternalRelease() { delete this; }

	public:
		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			CSkeletonInstance* pSkeleton,
			IMshRenderHandler* pRenderHandler);

	private:
		IZ_UINT8* Read(
			IZ_UINT8* pBuf,
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

	public:
		CPrimitiveSet* GetPrimSet(IZ_UINT idx);

	public:
		inline void GetCenter(SVector& vPos);

		const S_MSH_MTRL& GetMtrlInfo() const { return m_Info.mtrl; }

		IZ_UINT GetPrimSetNum() const { return m_Info.numSubset; }

		CVertexDeclaration* GetVD() { return m_VD; }

	private:
		S_MSH_MESH_SET m_Info;

		CVertexDeclaration* m_VD;
		
		CPrimitiveSet** m_pPrimSet;
	};

	// inline *************************************

	void CMeshSet::GetCenter(SVector& vPos)
	{
		vPos.x = m_Info.center[0];
		vPos.y = m_Info.center[1];
		vPos.z = m_Info.center[2];
		vPos.w = 1.0f;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_MESH_H__)
