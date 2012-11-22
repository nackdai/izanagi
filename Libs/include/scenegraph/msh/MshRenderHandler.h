#if !defined(__IZANAGI_SCENEGRAPH_MSH_RENDER_HANDLER_H__)
#define __IZANAGI_SCENEGRAPH_MSH_RENDER_HANDLER_H__

#include "izStd.h"

namespace izanagi {
	/**
	*/
	class IMshRenderHandler : public CObject {
	public:
		template <typename _T>
		static _T* CreateMshRenderHandler(IMemoryAllocator* pAllocator)
		{
			void* pBuf = ALLOC_ZERO(pAllocator, sizeof(_T));
			VRETURN_NULL(pBuf != IZ_NULL);

			_T* ret = new(pBuf) _T;
			{
				ret->AddRef();
				ret->m_Allocator = pAllocator;
			}

			return ret;
		}

	protected:
		IMshRenderHandler() { m_Allocator = IZ_NULL; }
		virtual ~IMshRenderHandler() {}

		NO_COPIABLE(IMshRenderHandler);

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
		IMemoryAllocator* m_Allocator;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SCENEGRAPH_MSH_RENDER_HANDLER_H__)
