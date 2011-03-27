#if !defined(__IZANAGI_SCENEGRAPH_MDL_MESH_H__)
#define __IZANAGI_SCENEGRAPH_MDL_MESH_H__

#include "MDLFormat.h"
#include "scenegraph/mdl/MdlRenderHandler.h"

namespace izanagi {
	class CGraphicsDevice;
	class CVertexDeclaration;
	class IInputStream;
	class CMdlMeshSubset;
	class CMdlSkeleton;

	class CMdlMesh : public CPlacementNew {
		friend class CMdlGeometry;

	private:
		CMdlMesh();
		~CMdlMesh();

		NO_COPIABLE(CMdlMesh);

	private:
		IZ_UINT8* Read(
			IZ_UINT8* pBuf,
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

		CMdlMeshSubset* GetSubset(IZ_UINT idx);

		inline IZ_BOOL RenderTranslucentMesh(
			CGraphicsDevice* pDevice,
			const CMdlSkeleton* pSkeleton,
			IMdlRenderHandler* pRenderHandler);
		inline IZ_BOOL RenderOpacityMesh(
			CGraphicsDevice* pDevice,
			const CMdlSkeleton* pSkeleton,
			IMdlRenderHandler* pRenderHandler);

		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			CStdList<CMdlMeshSubset>& listMeshSubset,
			const CMdlSkeleton* pSkeleton,
			IMdlRenderHandler* pRenderHandler);

	private:
		IZ_UINT GetMeshSubsetNum() const { return m_Info.numSubset; }

	private:
		S_MDL_MESH m_Info;

		CVertexDeclaration* m_VD;
		
		CMdlMeshSubset** m_pSubset;

		// For rendering.
		CStdList<CMdlMeshSubset> m_TranslucentList;
		CStdList<CMdlMeshSubset> m_OpacityList;
	};

	// inline *********************************

	IZ_BOOL CMdlMesh::RenderTranslucentMesh(
		CGraphicsDevice* pDevice,
		const CMdlSkeleton* pSkeleton,
		IMdlRenderHandler* pRenderHandler)
	{
		return Render(
				pDevice,
				m_TranslucentList,
				pSkeleton,
				pRenderHandler);
	}

	IZ_BOOL CMdlMesh::RenderOpacityMesh(
		CGraphicsDevice* pDevice,
		const CMdlSkeleton* pSkeleton,
		IMdlRenderHandler* pRenderHandler)
	{
		return Render(
				pDevice, 
				m_OpacityList,
				pSkeleton,
				pRenderHandler);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_MESH_H__)
