#if !defined(__IZANAGI_SYSTEM_SYS_THREAD_UTIL_H__)
#define __IZANAGI_SYSTEM_SYS_THREAD_UTIL_H__

namespace izanagi
{
namespace sys
{
    /**
     */
    template <class _T>
    class CScopeGuard {
    public:
        CScopeGuard(_T& guardian)
            : m_Body(guardian)
        {
            m_Body.Lock();
        }

        ~CScopeGuard()
        {
            m_Body.Unlock();
        }

    private:
        _T& m_Body;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_THREAD_UTIL_H__)
