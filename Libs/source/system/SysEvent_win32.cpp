#if defined(WIN32) || defined(WIN64)

#include "system/SysEvent.h"
#include "system/SysThread.h"

using namespace izanagi;

CEvent::CEvent()
{
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
				IZ_FALSE,	// InitialState -> TRUEだとシグナル状態、FALSEだと非シグナル状態
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
	IZ_ASSERT(m_Handle != IZ_NULL);
	::SetEvent(m_Handle);
}

// シグナル状態になるのを待つ.
IZ_BOOL CEvent::Wait()
{
	IZ_ASSERT(m_Handle != IZ_NULL);

	IZ_DWORD result = ::WaitForSingleObject(m_Handle, INFINITE);
	IZ_ASSERT(result == WAIT_OBJECT_0);

	return (result == WAIT_OBJECT_0);
}

// NOTE
// 自動リセットでWait終了時に自動で非シグナル状態になるので
// 明示的に非シグナル状態にする必要がない
#if 0
// 非シグナル状態にする.
void CEvent::Reset()
{
	IZ_ASSERT(m_Handle != IZ_NULL);
	::ResetEvent(m_Handle);
}
#endif

#endif	// #if defined(WIN32) || defined(WIN64)
