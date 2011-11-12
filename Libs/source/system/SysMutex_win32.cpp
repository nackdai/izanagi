#include "system/SysMutex.h"
#include "system/SysThread.h"

#ifdef WIN32

using namespace izanagi;

CMutex::CMutex()
{
	m_Flags.isInitialized = IZ_FALSE;
	m_Flags.isLocked = IZ_FALSE;
}


CMutex::~CMutex()
{
	Close();
}

/**
*/
IZ_BOOL CMutex::Open()
{
	::InitializeCriticalSection(&m_Handle);
	m_Flags.isInitialized = IZ_TRUE;
	return IZ_TRUE;
}

/**
*/
void CMutex::Close()
{
	IZ_ASSERT(!m_Flags.isLocked);

	::DeleteCriticalSection(&m_Handle);
	m_Flags.isInitialized = IZ_FALSE;
}

/**
*/
void CMutex::Lock()
{
	IZ_ASSERT(m_Flags.isInitialized);
	IZ_ASSERT(!m_Flags.isLocked);

	ThreadId id = CThread::GetCurrentThreadId();
	if (CThread::IsEqualThreadId(m_OwnerThreadId, id)) {
		// This mutex is already locked by this thread.
		return;
	}

	::EnterCriticalSection(&m_Handle);

	m_Flags.isLocked = IZ_TRUE;
	m_OwnerThreadId = CThread::GetCurrentThreadId();
}

void CMutex::Unlock()
{
	IZ_ASSERT(m_Flags.isInitialized);
	IZ_ASSERT(m_Flags.isLocked);

	ThreadId id = CThread::GetCurrentThreadId();
	if (!CThread::IsEqualThreadId(m_OwnerThreadId, id)) {
		// This thread already released this mutex.
		return;
	}

	m_Flags.isLocked = IZ_FALSE;
	m_OwnerThreadId = 0;

	::LeaveCriticalSection(&m_Handle);
}

#endif
