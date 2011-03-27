#if !defined(__IZANAGI_SCENEGRAPH_MDL_MESH_SUBSET_H__)
#define __IZANAGI_SCENEGRAPH_MDL_MESH_SUBSET_H__

#include "MDLFormat.h"

namespace izanagi {
	class CGraphicsDevice;
	class CVertexBuffer;
	class CIndexBuffer;
	class IInputStream;
	class IMdlRenderHandler;
	class CMdlSkeleton;

	class CMdlMeshSubset : public CPlacementNew {
		friend class CMdlMesh;
		friend class CMdlGeometry;

	private:
		CMdlMeshSubset();
		~CMdlMeshSubset();

		NO_COPIABLE(CMdlMeshSubset);

	private:
		IZ_BOOL Read(
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			const CMdlSkeleton* pSkeleton,
			IMdlRenderHandler* pRenderHandler);

#if 1
		IZ_BOOL DebugRender(
			CGraphicsDevice* pDevice,
			const CMdlSkeleton* pSkeleton,
			IMdlRenderHandler* pRenderHandler);
#endif

	private:
		const S_MDL_MESH_SUBSET& GetInfo() const { return m_Info; }

		void SetVB(CVertexBuffer* pVB) { SAFE_REPLACE(m_VB, pVB); }

		CStdList<CMdlMeshSubset>::Item* GetListItem() { return &m_ListItem; }
		CStdList<CMdlMeshSubset>::Item* GetRenderListItem() { return &m_RenderListItem; }

	private:
		S_MDL_MESH_SUBSET m_Info;

		IZ_UINT m_PrimNum;

		CVertexBuffer* m_VB;
		CIndexBuffer* m_IB;

		CStdList<CMdlMeshSubset>::Item m_ListItem;
		CStdList<CMdlMeshSubset>::Item m_RenderListItem;

		struct {
			IZ_UINT m_IsVisisbile	: 1;
		};

		IZ_BOOL m_IsTransform;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_MESH_SUBSET_H__)
