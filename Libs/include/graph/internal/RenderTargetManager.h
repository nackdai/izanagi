#if !defined(__URANUS_GRAPH_INTERNAL_RENDER_TARGET_MANAGER_H__)
#define __URANUS_GRAPH_INTERNAL_RENDER_TARGET_MANAGER_H__

#if 0
#include "unDefs.h"
#include "unD3DDefs.h"
#include "std/StdQueue.h"
#include "../dx9/Surface.h"

namespace uranus {
	// �����_�[�^�[�Q�b�g�Ǘ�
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
		// �v�b�V��
		UN_BOOL Push(CSurface* pSurf)
		{
			UN_BOOL ret = UN_FALSE;

			if (!m_Queue.IsQueueFull()) {
				// ��t�łȂ�
				SItem& sItem = m_Queue.GetEndFromQueue();
				SAFE_REPLACE(sItem.rt, pSurf);

				ret = m_Queue.PushQueue();
				if (!ret) {
					// ���s�E�E�E
					SAFE_RELEASE(sItem.rt);
				}
			}

			UN_ASSERT(ret);

			return ret;
		}

		// �|�b�v
		CSurface* Pop()
		{
			CSurface* ret = UN_NULL;

			if (!m_Queue.IsQueueEmpty()) {
				// ��łȂ�
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


		// �󂩂ǂ���
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
		// �L���[
		CQueue<SItem, _QUEUE_NUM> m_Queue;
	};
}	// namespace uranus
#else
#include "unDefs.h"
#include "unD3DDefs.h"
#include "std/StdQueue.h"

namespace uranus {
	template <typename _T, UN_UINT _QUEUE_NUM>
	class CObjectManager {
		friend class CGraphicsDevice;

	private:
		CObjectManager()
		{
			m_Queue.Init();
		}

		~CObjectManager() {}

	private:
		// �v�b�V��
		UN_BOOL Push(_T* p)
		{
			UN_BOOL ret = UN_FALSE;

			if (!m_Queue.IsQueueFull()) {
				// ��t�łȂ�
				SItem& sItem = m_Queue.GetEndFromQueue();
				SAFE_REPLACE(sItem.obj, p);

				ret = m_Queue.PushQueue();
				if (!ret) {
					// ���s�E�E�E
					SAFE_RELEASE(sItem.obj);
				}
			}

			UN_ASSERT(ret);

			return ret;
		}

		// �|�b�v
		_T* Pop()
		{
			_T* ret = UN_NULL;

			if (!m_Queue.IsQueueEmpty()) {
				// ��łȂ�
				SItem& sItem = m_Queue.GetFrontFromQueue();
				ret = sItem.obj;

				UN_BOOL result = m_Queue.PopQueue();
				UN_ASSERT(result);

				if (result) {
					SAFE_RELEASE(sItem.obj);
				}
			}

			//UN_ASSERT(ret != UN_NULL);

			return ret;
		}


		// �󂩂ǂ���
		UN_BOOL IsEmpty() const
		{
			return m_Queue.IsQueueEmpty();
		}

	private:
		struct SItem {
			_T* obj;

			SItem()
			{
				obj = UN_NULL;
			}
		};

	private:
		// �L���[
		CQueue<SItem, _QUEUE_NUM> m_Queue;
	};
}	// namespace uranus
#endif

#endif	// #if !defined(__URANUS_GRAPH_INTERNAL_RENDER_TARGET_MANAGER_H__)
