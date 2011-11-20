#define _WIN32_WINNT 0x0500  
#include "system/SysThreadDefs.h"

#include "system/SysMonitor.h"

using namespace izanagi;

///////////////////////////////////////////////////

class CMonitor::CMonitorImpl {
public:
	CMonitorImpl();
	~CMonitorImpl();

public:
	// 初期化
	void Init();

	// 終了
	void Destroy();

	// 同じスレッド内での呼び出しかどうか確認.
	IZ_BOOL CheckLockByCallingThread();

	// syncronized開始.
	void BeginSyncronized();

	// syncronized終了.
	IZ_BOOL EndSyncronized();

	// 待機.
	IZ_BOOL Wait();

	// 待機中のスレッドを１つ起こす.
	void Notify();

	// 待機中のスレッドを全て起こす.
	void NotifyAll();

private:
	IZ_UINT m_LockCount;

	CRITICAL_SECTION m_Syncronized;
	CRITICAL_SECTION m_WaitQueue;
};

CMonitor::CMonitorImpl::CMonitorImpl()
{
	m_LockCount = 0;
}

CMonitor::CMonitorImpl::~CMonitorImpl()
{
}

// 初期化
void CMonitor::CMonitorImpl::Init()
{
	::InitializeCriticalSection(&m_Syncronized);
	::InitializeCriticalSection(&m_WaitQueue);
}

// 終了
void CMonitor::CMonitorImpl::Destroy()
{
	::DeleteCriticalSection(&m_Syncronized);
	::DeleteCriticalSection(&m_WaitQueue);
}

// 同じスレッド内での呼び出しかどうか確認.
IZ_BOOL CMonitor::CMonitorImpl::CheckLockByCallingThread()
{
	// クリティカルセクションへのエントリをブロックせずに試みる

	// NOTE
	// 指定したクリティカルセクションに入ることができた場合
	// または現在のスレッドがクリティカルセクションをすでに所有している場合は
	// 0 以外の値が返る
	IZ_BOOL isScceedTryEnterCS = (::TryEnterCriticalSection(&m_Syncronized) != 0);
	VRETURN(!isScceedTryEnterCS);

	// そもそもクリティカルセクションに入っていないなんてことはないか確認
	IZ_ASSERT(m_LockCount > 0);

	// TryEnterCriticalSectionで入った分を抜ける
	::LeaveCriticalSection(&m_Syncronized);

	return IZ_TRUE;
}

// syncronized開始.
void CMonitor::CMonitorImpl::BeginSyncronized()
{
	::EnterCriticalSection(&m_Syncronized);
	++m_LockCount;
}

// syncronized終了.
IZ_BOOL CMonitor::CMonitorImpl::EndSyncronized()
{
	// TODO
	return IZ_TRUE;
}

// 待機.
IZ_BOOL CMonitor::CMonitorImpl::Wait()
{
	// 同じスレッド内での呼び出しかどうか確認.
	VRETURN(CheckLockByCallingThread());

	// TODO
	return IZ_TRUE;
}

// 待機中のスレッドを１つ起こす.
void CMonitor::CMonitorImpl::Notify()
{
}

// 待機中のスレッドを全て起こす.
void CMonitor::CMonitorImpl::NotifyAll()
{
}

///////////////////////////////////////////////////

CMonitor::CMonitor()
{
}


CMonitor::~CMonitor()
{
}
