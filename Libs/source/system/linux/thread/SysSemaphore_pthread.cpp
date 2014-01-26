#include "system/SysSemaphore.h"

namespace izanagi
{
namespace sys
{
    CSemaphore::CSemaphore()
    {
    }

    CSemaphore::~CSemaphore()
    {
        Close();
    }

    // 初期化.
    IZ_BOOL CSemaphore::Init(IZ_UINT16 initialCount)
    {
        int result = ::sem_init(
            &m_Handle,
            0,
            initialCount);

        IZ_ASSERT(result >= 0);

        return (result >= 0);
    }

    // 終了.
    void CSemaphore::Close()
    {
        ::sem_destroy(&m_Handle);
    }

    // 待機.
    IZ_BOOL CSemaphore::Wait(IZ_INT timeout/*= -1*/)
    {
        // セマフォがシグナル状態になるのを待つ。
        // 待つときにカウントを減らす。
        // カウントが０になったときに非シグナル状態になる。
        // つまり、カウントが０より大きければ待たない。

        ::sem_wait(&m_Handle);

        return IZ_TRUE;
    }

#if 0
    // セマフォカウントを指定された数だけ増やす.
    void CSemaphore::Release(IZ_UINT count/*= 1*/)
    {
        // もし、セマフォが非シグナル状態ならセマフォをシグナル状態にする

        IZ_ASSERT(m_Handle != IZ_NULL);

        // ０なんてある？
        count = (count == 0 ? 1 : count);

        ::sem_post_multiple(&m_Handle, count);
    }
#else
    // セマフォカウントを解放.
    void CSemaphore::Release()
    {
        // もし、セマフォが非シグナル状態ならセマフォをシグナル状態にする

        ::sem_post(&m_Handle);
    }
#endif
}   // namespace sys
}   // namespace izanagi
