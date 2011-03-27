#if !defined(__IZANAGI_SCENEGRAPH_MSH_MESH_SUBSET_H__)
#define __IZANAGI_SCENEGRAPH_MSH_MESH_SUBSET_H__

#include "MSHFormat.h"

namespace izanagi {
	class CGraphicsDevice;
	class CVertexBuffer;
	class CIndexBuffer;
	class IInputStream;
	class CSkeletonInstance;
	class IMshRenderHandler;

	class CPrimitiveSet : public CObject {
		friend class CMesh;
		friend class CMeshGroup;
		friend class CMeshSet;

	private:
		CPrimitiveSet();
		~CPrimitiveSet();

		NO_COPIABLE(CPrimitiveSet);

		void InternalRelease() { delete this; }

	public:
		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			CSkeletonInstance* pSkeleton,
			IMshRenderHandler* pRenderHandler);

	private:
		IZ_BOOL Read(
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

		IZ_BOOL DebugRender(
			CGraphicsDevice* pDevice,
			const CSkeletonInstance* pSkeleton,
			IMshRenderHandler* pRenderHandler);

	private:
		const S_MSH_PRIM_SET& GetInfo() const { return m_Info; }

		void SetVB(CVertexBuffer* pVB) { SAFE_REPLACE(m_VB, pVB); }

	private:
		S_MSH_PRIM_SET m_Info;

		IZ_UINT m_PrimNum;

		CVertexBuffer* m_VB;
		CIndexBuffer* m_IB;

		struct {
			IZ_UINT m_IsTransform	: 1;
		};
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MSH_MESH_SUBSET_H__)
