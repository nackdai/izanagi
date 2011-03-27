#if !defined(__IZANAGI_SCENEGRAPH_MDL_GEOMETRY_H__)
#define __IZANAGI_SCENEGRAPH_MDL_GEOMETRY_H__

#include "MDLFormat.h"

namespace izanagi {
	class CGraphicsDevice;
	class CVertexBuffer;
	class IInputStream;
	class CMdlMesh;
	class CMdlSkeleton;
	class IMdlRenderHandler;

	class CMdlGeometry : public CObject {
		friend class CModel;
		friend class CModelInstance;

	private:
		static CMdlGeometry* CreateMdlGeometry(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

	private:
		CMdlGeometry();
		~CMdlGeometry();

		NO_COPIABLE(CMdlGeometry);

		IZ_DEFINE_INTERNAL_RELEASE();

	private:
		IZ_BOOL ReadVertices(
			const S_MDL_GEOMETRY_CHUNK_HEADER& sHeader,
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

	public:
		IZ_BOOL Render(
			CGraphicsDevice* pDevice,
			const CMdlSkeleton* pSkeleton,
			IMdlRenderHandler* pRenderHandler);

	private:
		IMemoryAllocator* m_pAllocator;

		S_MDL_GEOMETRY_CHUNK_HEADER m_Header;

		CVertexBuffer** m_pVB;
		CMdlMesh** m_pMesh;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_GEOMETRY_H__)
