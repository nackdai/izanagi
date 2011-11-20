#if defined(WIN32) || defined(WIN64)

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT	0x0400
#endif

#include "system/SysCond.h"
#include "system/SysSemaphore.h"
#include "system/SysMutex.h"
#include "system/SysEvent.h"
#include "izStd.h"

using namespace izanagi;

///////////////////////////////////////////
class CCondVar::CCondImpl {
public:
	CCondImpl() {}
	~CCondImpl() {}

	IZ_DECL_PLACEMENT_NEW();

public:
	// 初期化
	IZ_BOOL Init();

	// 終了
	void Destroy();

	// 待機
	void Wait(CMutex& externalMiutex);

	// スレッドを起こす
	void Signal();

	// 全スレッドを起こす
	void Broadcast();

private:
	CSemaphore m_WaitSema;
	CRITICAL_SECTION m_WaitersCountLock;
	EventHandle m_WaitersReleaseDoneEvent;

	IZ_UINT m_Waiters;
	IZ_BOOL m_DoBroadcast;
};

// 初期化
IZ_BOOL CCondVar::CCondImpl::Init()
{
	m_Waiters = 0;
	m_DoBroadcast = IZ_FALSE;

	VRETURN(m_WaitSema.Init(0));
	
	::InitializeCriticalSection(&m_WaitersCountLock);

	// 自動リセットで初期化
	m_WaitersReleaseDoneEvent = ::CreateEvent(
									IZ_NULL,	// Security
									IZ_FALSE,	// ManualReset
									IZ_FALSE,	// InitialState
									IZ_NULL);	// EventName
	VRETURN(m_WaitersReleaseDoneEvent != IZ_NULL);

	return IZ_TRUE;
}

// 終了
void CCondVar::CCondImpl::Destroy()
{
	// 何もしていないこと
	IZ_ASSERT(m_Waiters == 0);
	IZ_ASSERT(!m_DoBroadcast);

	::DeleteCriticalSection(&m_WaitersCountLock);

	if (m_WaitersReleaseDoneEvent) {
		::CloseHandle(m_WaitersReleaseDoneEvent);
		m_WaitersReleaseDoneEvent = IZ_NULL;
	}
}

// 待機
void CCondVar::CCondImpl::Wait(CMutex& externalMiutex)
{
	::EnterCriticalSection(&m_WaitersCountLock);
	{
		m_Waiters++;
	}
	::LeaveCriticalSection(&m_WaitersCountLock);

	// TODO
	// externalMiutexが同じスレッドからの呼び出しかチェック

	// externalMiutexをシグナル状態にして、m_WaitSemaがシグナル状態になるのを待つ
	// m_WaitSemaは初期カウントが０なので、通常はここで必ず待つ
	::SignalObjectAndWait(
		externalMiutex.m_Handle,	// シグナル状態にするオブジェクトを示すハンドル
		m_WaitSema.m_Handle,		// シグナル状態になるまで待機するオブジェクトを示すハンドル
		INFINITE,
		FALSE);

	// 全スレッドがおこされたかどうか
	IZ_BOOL doneBroadcast = IZ_FALSE;

	::EnterCriticalSection(&m_WaitersCountLock);
	{
		m_Waiters--;
		doneBroadcast = m_DoBroadcast && (m_Waiters == 0);
	}
	::LeaveCriticalSection(&m_WaitersCountLock);

	if (doneBroadcast) {
		// 全スレッドを起こした

		// Broadcastで止まっているm_WaitersReleaseDoneEventをシグナル状態にする
		// -> 全てのスレッドが起きるのを待っているm_WaitersReleaseDoneEventをシグナル状態にする
		::SignalObjectAndWait(
			m_WaitersReleaseDoneEvent,
			externalMiutex.m_Handle,
			INFINITE,
			FALSE);
	}
	else {
		externalMiutex.Lock();
	}
}

// スレッドを起こす
void CCondVar::CCondImpl::Signal()
{
	::EnterCriticalSection(&m_WaitersCountLock);

	IZ_BOOL hasWaiters = (m_Waiters > 0);

	::LeaveCriticalSection(&m_WaitersCountLock);

	if (hasWaiters) {
		// Waitで止まっているスレッドを起こす
		m_WaitSema.Release();
	}
}

// 全スレッドを起こす
void CCondVar::CCondImpl::Broadcast()
{
	::EnterCriticalSection(&m_WaitersCountLock);

	IZ_BOOL hasWaiters = IZ_FALSE;

	if (m_Waiters > 0) {
		// 待機スレッドが存在するので、全て起こすようにする
		m_DoBroadcast = IZ_TRUE;
		hasWaiters = IZ_TRUE;
	}

	if (hasWaiters) {
		// 待機スレッドを全て起こす
		m_WaitSema.Release(m_Waiters);

		::LeaveCriticalSection(&m_WaitersCountLock);

		// 全てのスレッドが起こされるのをここで待つ
		::WaitForSingleObject (m_WaitersReleaseDoneEvent, INFINITE);

		// Broadcast終了
		m_DoBroadcast = IZ_FALSE;
	}
	else {
		// 待機スレッドが無いので何もしない
		::LeaveCriticalSection(&m_WaitersCountLock);
	}
}
///////////////////////////////////////////

CCondVar::CCondVar()
{
	m_Impl = IZ_NULL;
	C_ASSERT(sizeof(m_Buf) == sizeof(CCondImpl));
}

CCondVar::~CCondVar()
{
	Destroy();
}

// 初期化.
IZ_BOOL CCondVar::Init()
{
	m_Impl = new(m_Buf) CCondImpl;
	IZ_BOOL ret = m_Impl->Init();
	return ret;
}

// 終了.
void CCondVar::Destroy()
{
	if (m_Impl) {
		m_Impl->Destroy();
		delete m_Impl;
		m_Impl = IZ_NULL;
	}
}

// 待機.
void CCondVar::Wait(CMutex& externalMiutex)
{
	IZ_ASSERT(m_Impl);
	m_Impl->Wait(externalMiutex);
}

// 待機中スレッドをひとつ起動.
void CCondVar::Signal()
{
	IZ_ASSERT(m_Impl);
	m_Impl->Signal();
}

// 待機中スレッドを全て起動
void CCondVar::Broadcast()
{
	IZ_ASSERT(m_Impl);
	m_Impl->Broadcast();
}

#endif	// #if defined(WIN32) || defined(WIN64)
