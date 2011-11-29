#define _WIN32_WINNT 0x0500
#include "system/SysThreadDefs.h"

#include "system/SysMonitor.h"
#include "system/SysThread.h"

#include "izStd.h"

using namespace izanagi;

// NOTE
// m_WaitCondのSignal、Broadcastの呼び出しの安全性や複数回呼び出しについては
// CCondVarの方で保障しているので、ここでは考慮しない

CMonitor::CMonitor()
{
}

CMonitor::~CMonitor()
{
	Destroy();
}

// 初期化.
IZ_BOOL CMonitor::Init()
{
	IZ_BOOL ret = IZ_FALSE;

	ret = m_LockMutex.Open();
	VGOTO(ret, __EXIT__);

	ret = m_WaitCond.Init();
	VGOTO(ret, __EXIT__);

__EXIT__:
	if (!ret) {
		Destroy();
	}
	return ret;
}

// 終了.
void CMonitor::Destroy()
{
	m_LockMutex.Close();
	m_WaitCond.Destroy();
}

// syncronized開始.
void CMonitor::Lock()
{
	m_LockMutex.Lock();
}

// syncronized終了.
void CMonitor::Unlock()
{
	m_LockMutex.Unlock();

	// 念のため
	NotifyAll();
}

namespace {
	// スレッドIDチェック
	inline IZ_BOOL CheckOwnerThreadId(const ThreadId& threadId)
	{
		ThreadId id = CThread::GetCurrentThreadId();
		IZ_BOOL ret = CThread::IsEqualThreadId(threadId, id);
		return ret;
	}
}

// 待機.
void CMonitor::Wait()
{
	// スレッドIDチェック
	IZ_ASSERT(CheckOwnerThreadId(m_LockMutex.GetOwnerThreadId()));

	m_WaitCond.Wait(m_LockMutex);
}

// 待機中のスレッドを１つ起こす.
void CMonitor::Notify()
{
	// スレッドIDチェック
	IZ_ASSERT(CheckOwnerThreadId(m_LockMutex.GetOwnerThreadId()));

	m_WaitCond.Signal();
}

// 待機中のスレッドを全て起こす.
void CMonitor::NotifyAll()
{
	// スレッドIDチェック
	IZ_ASSERT(CheckOwnerThreadId(m_LockMutex.GetOwnerThreadId()));

	m_WaitCond.Broadcast();
}
