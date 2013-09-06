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
        template <typename _T>
        static CMessage* CreateMessage(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T();
            {
                ret->m_Allocator = allocator;
            }

            return ret;
        }

        static void DeleteMessage(CMessage*& msg)
        {
            delete msg;
            FREE(msg->m_Allocator, msg);
        }

    protected:
        CMessage()
        {
            m_Item.Init(this);
        }
        virtual ~CMessage() {}

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        virtual void* GetValue() { return IZ_NULL; }
        virtual IZ_BOOL GetValue(CValue& ret) { return IZ_FALSE; }

    private:
        CStdQueue<CMessage>::Item* GetItem() { return &m_Item; }

    private:
        IMemoryAllocator* m_Allocator;

        CStdQueue<CMessage>::Item m_Item;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_MESSAGE_H__)
