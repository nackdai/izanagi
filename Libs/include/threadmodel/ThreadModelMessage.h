#if !defined(__IZANAGI_THREAD_MODEL_MESSAGE_H__)
#define __IZANAGI_THREAD_MODEL_MESSAGE_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class CMessage : public CPlacementNew
    {
        friend class CMessageQueue;

    public:
		/** メッセージを生成.
		 */
        template <typename _T>
        static _T* CreateMessage(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T();
            {
                ret->m_Allocator = allocator;
            }

            return ret;
        }

		/** メッセージを破棄.
		 */
        static void DeleteMessage(CMessage*& msg)
        {
            delete msg;
            FREE(msg->m_Allocator, msg);
        }

    protected:
        CMessage()
        {
            m_Item.Init(this);
			m_isRegistered = IZ_FALSE;
        }
        virtual ~CMessage() {}

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
		/** メッセージ値をセット.
		 */
        virtual void* GetValue() { return IZ_NULL; }

		/** メッセージ値を取得
		 */
        virtual IZ_BOOL GetValue(CValue& ret) { return IZ_FALSE; }

    private:
        CStdQueue<CMessage>::Item* GetItem()
		{
			return &m_Item;
		}

		IZ_BOOL isRegistered()
		{
			return m_isRegistered;
		}

		void reset()
		{
			m_isRegistered = IZ_FALSE;
		}

    private:
        IMemoryAllocator* m_Allocator;

        CStdQueue<CMessage>::Item m_Item;
		IZ_BOOL m_isRegistered;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_MESSAGE_H__)
