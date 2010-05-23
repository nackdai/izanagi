#if !defined(__URANUS_GRAPH_INTERNAL_RENDER_TARGET_MANAGER_H__)
#define __URANUS_GRAPH_INTERNAL_RENDER_TARGET_MANAGER_H__

#include "unDefs.h"
#include "unD3DDefs.h"
#include "std/StdQueue.h"
#include "../dx9/Surface.h"

namespace uranus {
	// レンダーターゲット管理
	template <UN_UINT _QUEUE_NUM>
	class CRenderTargetManager {
		friend class CGraphicsDevice;

	private:
		CRenderTargetManager()
		{
			m_Queue.Init();
		}

		~CRenderTargetManager() {}

	private:
		// プッシュ
		UN_BOOL Push(CSurface* pSurf)
		{
			UN_BOOL ret = UN_FALSE;

			if (!m_Queue.IsQueueFull()) {
				// 一杯でない
				SItem& sItem = m_Queue.GetEndFromQueue();
				SAFE_REPLACE(sItem.rt, pSurf);

				ret = m_Queue.PushQueue();
				if (!ret) {
					// 失敗・・・
					SAFE_RELEASE(sItem.rt);
				}
			}

			UN_ASSERT(ret);

			return ret;
		}

		// ポップ
		CSurface* Pop()
		{
			CSurface* ret = UN_NULL;

			if (!m_Queue.IsQueueEmpty()) {
				// 空でない
				SItem& sItem = m_Queue.GetFrontFromQueue();
				ret = sItem.rt;

				UN_BOOL result = m_Queue.PopQueue();
				UN_ASSERT(result);

				if (result) {
					SAFE_RELEASE(sItem.rt);
				}
			}

			//UN_ASSERT(ret != UN_NULL);

			return ret;
		}


		// 空かどうか
		UN_BOOL IsEmpty() const
		{
			return m_Queue.IsQueueEmpty();
		}

	private:
		struct SItem {
			CSurface* rt;

			SItem()
			{
				rt = UN_NULL;
			}
		};

	private:
		// キュー
		CQueue<SItem, _QUEUE_NUM> m_Queue;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_INTERNAL_RENDER_TARGET_MANAGER_H__)
