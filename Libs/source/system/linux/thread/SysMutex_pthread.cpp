#include "system/SysMutex.h"
#include "system/SysThread.h"

namespace izanagi
{
namespace sys
{
    CMutex::CMutex()
    {
        m_Handle = IZ_NULL;
        //m_OwnerThreadId = 0;
    }

    CMutex::~CMutex()
    {
        Close();
    }

    /**
    */
    IZ_BOOL CMutex::Open()
    {
        ::pthread_mutex_init(&m_Handle, NULL);
        return IZ_TRUE;
    }

    /**
    */
    void CMutex::Close()
    {
        if (m_Handle != IZ_NULL) {
            ::pthread_mutex_destroy(&m_Handle);
        }
    }

    /**
    */
    void CMutex::Lock()
    {
        // TODO
#if 0
        ThreadId id = CThread::GetCurrentThreadId();
        if (CThread::IsEqualThreadId(m_OwnerThreadId, id)) {
            // This mutex is already locked by this thread.
            return;
        }
#endif

        ::pthread_mutex_lock(&m_Handle);

#if 0
        m_OwnerThreadId = CThread::GetCurrentThreadId();
#endif
    }

    void CMutex::Unlock()
    {
#if 0
        ThreadId id = CThread::GetCurrentThreadId();
        if (!CThread::IsEqualThreadId(m_OwnerThreadId, id)) {
            // This thread already released this mutex.
            return;
        }

        m_OwnerThreadId = 0;
#endif

        ::pthread_mutex_unlock(&m_Handle);
    }
}   // namespace sys
}   // namespace iznagi
