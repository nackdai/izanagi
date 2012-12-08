#if defined(WIN32) || defined(WIN64)

#include "system/SysSemaphore.h"

namespace izanagi
{
namespace sys
{
    CSemaphore::CSemaphore()
    {
	    m_Handle = IZ_NULL;
    }

    CSemaphore::~CSemaphore()
    {
	    Close();
    }

    // 初期化.
    IZ_BOOL CSemaphore::Init(IZ_UINT16 initialCount)
    {
	    IZ_ASSERT(m_Handle == IZ_NULL);

	    m_Handle = ::CreateSemaphore(
					    IZ_NULL,
					    initialCount,	// 初期カウント
					    IZ_INT16_MAX,	// 最大カウント
					    IZ_NULL);

	    IZ_BOOL ret = (m_Handle != IZ_NULL);
	    IZ_ASSERT(ret);

	    return ret;
    }

    // 終了.
    void CSemaphore::Close()
    {
	    if (m_Handle) {
		    ::CloseHandle(m_Handle);
		    m_Handle = IZ_NULL;
	    }
    }

    // 待機.
    IZ_BOOL CSemaphore::Wait(IZ_INT timeout/*= -1*/)
    {
	    // セマフォがシグナル状態になるのを待つ。
	    // 待つときにカウントを減らす。
	    // カウントが０になったときに非シグナル状態になる。
	    // つまり、カウントが０より大きければ待たない。

	    IZ_ASSERT(m_Handle != IZ_NULL);

	    IZ_DWORD result = ::WaitForSingleObject(
						    m_Handle,
						    (timeout < 0 ? INFINITE : timeout));
	    IZ_ASSERT(result == WAIT_OBJECT_0);

	    return (result == WAIT_OBJECT_0);
    }

    // セマフォカウントを指定された数だけ増やす.
    void CSemaphore::Release(IZ_UINT count/*= 1*/)
    {
	    // もし、セマフォが非シグナル状態ならセマフォをシグナル状態にする

	    IZ_ASSERT(m_Handle != IZ_NULL);

	    // ０なんてある？
	    count = (count == 0 ? 1 : count);

	    IZ_BOOL result = ::ReleaseSemaphore(m_Handle, count, IZ_NULL);
	    IZ_ASSERT(result);
    }
}   // namespace sys
}   // namespace izanagi
#endif	// #if defined(WIN32) || defined(WIN64)
