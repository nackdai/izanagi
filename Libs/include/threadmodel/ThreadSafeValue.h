#if !defined(__IZANAGI_THREAD_SAFE_FLAG_H__)
#define __IZANAGI_THREAD_SAFE_FLAG_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    template <typename _T>
    class CThreadSafeValue
    {
    public:
        CThreadSafeValue(_T initialValue)
        {
            m_Value = initialValue;
            m_Mutex.Open();
        }
        ~CThreadSafeValue()
        {
            m_Mutex.Close();
        }

    protected:
        CThreadSafeValue()
        {
            m_Mutex.Open();
        }

    private:
        CThreadSafeValue(const CThreadSafeValue& rhs) {}

    public:
        void SetValue(_T value)
        {
            m_Mutex.Lock();
            {
                m_Value = value;
            }
            m_Mutex.Unlock();
        }

        _T GetValue()
        {
            _T ret = IZ_FALSE;

            m_Mutex.Lock();
            {
                ret = m_Value;
            }
            m_Mutex.Unlock();

            return ret;
        }

        const CThreadSafeValue& operator=(const CThreadSafeValue& rhs)
        {
            SetValue((const_cast<CThreadSafeValue&>(rhs)).GetValue());
            return *this;
        }

        const CThreadSafeValue& operator=(_T value)
        {
            SetValue(value);
            return *this;
        }

        operator _T()
        {
            return GetValue();
        }

    private:
        sys::CMutex m_Mutex;
        _T m_Value;
    };

    /**
     */
    class CThreadSafeFlag : public CThreadSafeValue<IZ_BOOL>
    {
    public:
        CThreadSafeFlag(IZ_BOOL initialValue = IZ_FALSE) : CThreadSafeValue(initialValue)
        {}

        ~CThreadSafeFlag() {}

    private:
        CThreadSafeFlag(const CThreadSafeFlag& rhs) {}
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_SAFE_FLAG_H__)
