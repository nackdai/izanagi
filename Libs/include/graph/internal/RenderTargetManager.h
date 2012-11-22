#if !defined(__IZANAGI_GRAPH_INTERNAL_RENDER_TARGET_MANAGER_H__)
#define __IZANAGI_GRAPH_INTERNAL_RENDER_TARGET_MANAGER_H__

#include "izDefs.h"
#include "std/StdStack.h"
#include "graph/Surface.h"

namespace izanagi
{
namespace graph
{
	// レンダーターゲット管理
	template <IZ_UINT _STACK_NUM>
	class CRenderTargetManager
    {
		friend class CGraphicsDevice;

	private:
		CRenderTargetManager() {}
		~CRenderTargetManager() {}

	public:
		// プッシュ
		IZ_BOOL Push(CSurface* pSurf)
		{
			IZ_BOOL ret = IZ_FALSE;

			if (!m_Stack.IsFull()) {
				// 一杯でない
				SItem& sItem = m_Stack.Get();

				ret = m_Stack.Push();
				if (ret) {
					SAFE_REPLACE(sItem.rt, pSurf);
				}
			}

			IZ_ASSERT(ret);

			return ret;
		}

		// ポップ
		CSurface* Pop()
		{
			CSurface* ret = IZ_NULL;

			if (!m_Stack.IsEmpty()) {
				// 空でない
				IZ_BOOL result = m_Stack.Pop();
				IZ_ASSERT(result);

				if (result) {
					SItem& sItem = m_Stack.Get();
					ret = sItem.rt;
					SAFE_RELEASE(sItem.rt);
				}
			}

			//IZ_ASSERT(ret != IZ_NULL);

			return ret;
		}


		// 空かどうか
		IZ_BOOL IsEmpty() const
		{
			return m_Stack.IsEmpty();
		}

	private:
		struct SItem {
			CSurface* rt;

			SItem()
			{
				rt = IZ_NULL;
			}
		};

	private:
		// スタック
		CStack<SItem, _STACK_NUM> m_Stack;
	};
}   // namespace graph
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_INTERNAL_RENDER_TARGET_MANAGER_H__)
