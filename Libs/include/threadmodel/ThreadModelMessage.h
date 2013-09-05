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
    class CMessage
    {
        friend class CMessageQueue;

    protected:
        CMessage()
        {
            m_Item.Init(this);
        }
        virtual ~CMessage() {}

    public:
        virtual void* GetValue() { return IZ_NULL; }
        virtual IZ_BOOL GetValue(CValue& ret) { return IZ_FALSE; }

    private:
        CStdQueue<CMessage>::Item* GetItem() { return &m_Item; }

    private:
        CStdQueue<CMessage>::Item m_Item;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_MESSAGE_H__)
