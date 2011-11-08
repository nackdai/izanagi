#include "system/SysEvent.h"
#include "system/SysThread.h"

using namespace izanagi;

CEvent::CEvent()
{
	m_OwnerThreadId = 0;
	m_Handle = IZ_NULL;
}

CEvent::~CEvent()
{
	Close();
}

// 初期化.
IZ_BOOL CEvent::Open()
{
	// NOTE
	// ・自動リセット
	// 　-> 待機関数が制御を返す前に自動的に非シグナル状態になるようにする
	// 　-> Wait終了時に自動で非シグナル状態になる
	// ・手動リセット
	//   -> ResetEvent() 関数を使って明示的に非シグナル状態に再設定しなければならない

	// 手動リセット
	// 非シグナル状態
	m_Handle = ::CreateEvent(
				IZ_NULL,	// Security
				IZ_TRUE,	// ManualReset
				IZ_FALSE,	// InitialState
				IZ_NULL);	// EventName

	IZ_ASSERT(m_Handle != IZ_NULL);

	return (m_Handle != IZ_NULL);
}

// 終了.
void CEvent::Close()
{
	if (m_Handle != IZ_NULL) {
		// 念のため
		Set();

		::CloseHandle(m_Handle);
		m_Handle = IZ_NULL;
	}
}

// シグナル状態にする.
void CEvent::Set()
{
	ThreadId id = CThread::GetCurrentThreadId();
	if (CThread::IsEqualThreadId(m_OwnerThreadId, id)) {
		// This mutex is already locked by this thread.
		return;
	}

	IZ_ASSERT(m_Handle != IZ_NULL);

	if (m_Handle) {
		::SetEvent(m_Handle);

		m_OwnerThreadId = CThread::GetCurrentThreadId();
	}
}

// シグナル状態になるのを待つ.
IZ_BOOL CEvent::Wait()
{
	IZ_ASSERT(m_Handle != IZ_NULL);

	IZ_DWORD result = ::WaitForSingleObject(m_Handle,INFINITE);
	IZ_ASSERT(result == WAIT_OBJECT_0);

	return (result == WAIT_OBJECT_0);
}

// 非シグナル状態にする.
void CEvent::Reset()
{
	ThreadId id = CThread::GetCurrentThreadId();
	if (!CThread::IsEqualThreadId(m_OwnerThreadId, id)) {
		// This thread already released this mutex.
		return;
	}

	m_OwnerThreadId = 0;

	IZ_ASSERT(m_Handle != IZ_NULL);

	if (m_Handle) {
		::ResetEvent(m_Handle);
	}
}
