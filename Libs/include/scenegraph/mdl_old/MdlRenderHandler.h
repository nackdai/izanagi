#if !defined(__IZANAGI_SCENEGRAPH_MDL_RENDER_HANDLER_H__)
#define __IZANAGI_SCENEGRAPH_MDL_RENDER_HANDLER_H__

#include "izStd.h"

namespace izanagi {
	/**
	*/
	class IMdlRenderHandler : public CObject {
	public:
		template <typename _T>
		static _T* CreateMdlRenderHandler(IMemoryAllocator* pAllocator)
		{
			void* pBuf = ALLOC_ZERO(pAllocator, sizeof(_T));
			VRETURN_NULL(pBuf != IZ_NULL);

			_T* ret = new(pBuf) _T;
			{
				ret->AddRef();
				ret->m_pAllocator = pAllocator;
			}

			return ret;
		}

	protected:
		IMdlRenderHandler() { m_pAllocator = IZ_NULL; }
		virtual ~IMdlRenderHandler() {}

		NO_COPIABLE(IMdlRenderHandler);

		IZ_DEFINE_INTERNAL_RELEASE();

	public:
		PURE_VIRTUAL(void BeginRenderMesh());
		PURE_VIRTUAL(void EndRenderMesh());

		PURE_VIRTUAL(
			void SetJointMatrix(
				IZ_UINT nIdx,
				const SMatrix& mtx));

		PURE_VIRTUAL(void CommitChanges());

	protected:
		IMemoryAllocator* m_pAllocator;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MDL_RENDER_HANDLER_H__)
