#include "system/SysMutex.h"
#include "system/SysThread.h"

#ifdef WIN32

using namespace izanagi;

CMutex::CMutex()
{
#ifndef __IZ_MUTEX_IMPL_CRITICAL_SECTION__
	m_Handle = IZ_NULL;
#endif
}


CMutex::~CMutex()
{
	Close();
}

/**
*/
IZ_BOOL CMutex::Open()
{
#ifdef __IZ_MUTEX_IMPL_CRITICAL_SECTION__
	::InitializeCriticalSection(&m_Handle);
	return IZ_TRUE;
#else
	m_Handle = ::CreateMutex(
				IZ_NULL,
				IZ_FALSE,
				IZ_NULL);
	IZ_ASSERT(m_Handle != IZ_NULL);

	return (m_Handle != IZ_NULL);
#endif
}

/**
*/
void CMutex::Close()
{
#ifdef __IZ_MUTEX_IMPL_CRITICAL_SECTION__
	::DeleteCriticalSection(&m_Handle);
#else
	if (m_Handle != IZ_NULL) {
		::WaitForSingleObject(m_Handle, INFINITE);
		::CloseHandle(m_Handle);
		m_Handle = IZ_NULL;
	}
#endif
}

/**
*/
void CMutex::Lock()
{
	ThreadId id = CThread::GetCurrentThreadId();
	if (CThread::IsEqualThreadId(m_OwnerThreadId, id)) {
		// This mutex is already locked by this thread.
		return;
	}

#ifdef __IZ_MUTEX_IMPL_CRITICAL_SECTION__
	::EnterCriticalSection(&m_Handle);
	m_OwnerThreadId = CThread::GetCurrentThreadId();
#else
	IZ_ASSERT(m_Handle != IZ_NULL);

	if (m_Handle) {
		::WaitForSingleObject(m_Handle, INFINITE);

		m_OwnerThreadId = CThread::GetCurrentThreadId();
	}
#endif
}

void CMutex::Unlock()
{
	ThreadId id = CThread::GetCurrentThreadId();
	if (!CThread::IsEqualThreadId(m_OwnerThreadId, id)) {
		// This thread already released this mutex.
		return;
	}

	m_OwnerThreadId = 0;

#ifdef __IZ_MUTEX_IMPL_CRITICAL_SECTION__
	::LeaveCriticalSection(&m_Handle);
#else
	IZ_ASSERT(m_Handle != IZ_NULL);

	if (m_Handle) {
		::ReleaseMutex(m_Handle);
	}
#endif
}

#endif
