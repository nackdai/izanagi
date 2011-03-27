#if !defined(__IZANAGI_SCENEGRAPH_IZ_MODEL_H__)
#define __IZANAGI_SCENEGRAPH_IZ_MODEL_H__

#include "izStd.h"
#include "izGraph.h"
#include "MDLFormat.h"

namespace izanagi {
	class IInputStream;
	class CMdlGeometry;
	class CMdlJoint;
	class IMdlRenderHandler;

	/**
	*/
	class CModel : public CObject {
		friend class CModelInstance;

	public:
		// Create an instance.
		static CModel* CreateModel(
			IMemoryAllocator* pAllocator,
			CGraphicsDevice* pDevice,
			IInputStream* pIn);

	private:
		CModel();
		~CModel();

		NO_COPIABLE(CModel);

		IZ_DEFINE_INTERNAL_RELEASE();

	private:
		IZ_BOOL Read(
			IMemoryAllocator* pAllocator,
			IInputStream* pIn);

		CGraphicsDevice* GetGraphicsDevice() { return m_pDevice; }
		CMdlGeometry* GetGeometry() { return m_pGeom; }
		CMdlJoint* GetJoint() { return m_pJoint; }

	private:
		IMemoryAllocator* m_pAllocator;
		CGraphicsDevice* m_pDevice;

		S_MDL_HEADER m_sHeader;

		CMdlGeometry* m_pGeom;
		CMdlJoint* m_pJoint;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_IZ_MODEL_H__)
