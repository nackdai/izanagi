#if defined(WIN32) || defined(WIN64)

#include "system/SysMutex.h"
#include "system/SysThread.h"

using namespace izanagi;

CMutex::CMutex()
{
	m_Handle = IZ_NULL;
}


CMutex::~CMutex()
{
	Close();
}

/**
*/
IZ_BOOL CMutex::Open()
{
	m_Handle = ::CreateMutex(
				NULL,
				FALSE,
				NULL);
	IZ_ASSERT(m_Handle != IZ_NULL);

	return (m_Handle != IZ_NULL);
}

/**
*/
void CMutex::Close()
{
	if (m_Handle) {
		::CloseHandle(m_Handle);
		m_Handle = IZ_NULL;
	}
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

	::WaitForSingleObject(m_Handle, INFINITE);

	m_OwnerThreadId = CThread::GetCurrentThreadId();
}

void CMutex::Unlock()
{
	ThreadId id = CThread::GetCurrentThreadId();
	if (!CThread::IsEqualThreadId(m_OwnerThreadId, id)) {
		// This thread already released this mutex.
		return;
	}

	m_OwnerThreadId = 0;

	::ReleaseMutex(m_Handle);
}

#endif	// #if defined(WIN32) || defined(WIN64)
