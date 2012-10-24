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
            m_Key = 0;
            m_Item.Init(this);
        }
        virtual ~CMessage() {}

    public:
        void SetKey(IZ_UINT key) { m_Key = key; }
        IZ_UINT GetKey() const { return m_Key; }

        virtual IZ_BOOL GetValueProxy(CValue& ret) { return IZ_FALSE; }

    private:
        CStdQueue<CMessage>::Item* GetItem() { return &m_Item; }

    private:
        IZ_UINT m_Key;
        CStdQueue<CMessage>::Item m_Item;
    };

    /**
     */
    template <typename T>
    class CMessageImpl : public CMessage
    {
    public:
        CMessageImpl()
        {
            m_Value = (T)0;
        }
        CMessageImpl(const T& rhs)
        {
            m_Value = rhs;
        }
        virtual ~CMessageImpl() {}

    public:
        void SetValue(const typename T& value)
        {
            m_Value = value;
        }

        const T& GetValue() const
        {
            return m_Value;
        }

    protected:
        T m_Value;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_MESSAGE_H__)
