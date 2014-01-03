#include "system/SysEvent.h"
#include "system/SysThread.h"

namespace izanagi
{
namespace sys
{
    CEvent::CEvent()
    {
        m_Handle.mutex = IZ_NULL;
        m_Handle.cond = IZ_NULL;
    }

    CEvent::~CEvent()
    {
        Close();
    }

    // 初期化.
    IZ_BOOL CEvent::Open()
    {
        ::pthread_cond_init(&m_Handle.cond, NULL);
        VRETURN(m_Handle.cond != IZ_NULL);

        ::pthread_mutex_init(&m_Handle.mutex, NULL);
        VRETURN(m_Handle.mutex != IZ_NULL);

        return IZ_TRUE;
    }

    // 終了.
    void CEvent::Close()
    {
        if (m_Handle.cond != IZ_NULL
            && m_Handle.mutex != IZ_NULL)
        {
            // 念のため
            Set();

            ::pthread_mutex_destroy(&m_Handle.mutex);
            ::pthread_cond_destroy(&m_Handle.cond);

            m_Handle.mutex = IZ_NULL;
            m_Handle.cond = IZ_NULL;
        }
    }

    // シグナル状態にする.
    void CEvent::Set()
    {
        IZ_ASSERT(m_Handle.cond != IZ_NULL);
        IZ_ASSERT(m_Handle.mutex != IZ_NULL);

        ::pthread_mutex_lock(&m_Handle.mutex);
        ::pthread_cond_signal(&m_Handle.cond);
        ::pthread_mutex_unlock(&m_Handle.mutex);
    }

    // シグナル状態になるのを待つ.
    IZ_BOOL CEvent::Wait()
    {
        IZ_ASSERT(m_Handle.cond != IZ_NULL);
        IZ_ASSERT(m_Handle.mutex != IZ_NULL);

        ::pthread_mutex_lock(&m_Handle.mutex);
        int result = ::pthread_cond_wait(&m_Handle.cond, &m_Handle.mutex);
        ::pthread_mutex_unlock(&m_Handle.mutex);

        return (result >= 0);
    }

    // 非シグナル状態にする.
    void CEvent::Reset()
    {
        // Nothing is done...
    }
}   // namespace sys
}   // namespace iznaagi
