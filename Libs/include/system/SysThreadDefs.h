#if !defined(__IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__)
#define __IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__

#undef WINDOWS

#if defined(WINDOWS)
    #include <windows.h>

namespace izanagi
{
namespace sys
{
    typedef UINT    ThreadId;

    typedef HANDLE  ThreadHandle;
    typedef HANDLE  MutexHandle;
    typedef HANDLE  EventHandle;
    typedef HANDLE  SemaHandle;
#else
    #include <pthread.h>

namespace izanagi
{
namespace sys
{
    typedef pthread_t   ThreadId;

    typedef pthread_t       ThreadHandle;
    typedef pthread_mutex_t MutexHandle;
#endif
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__)
