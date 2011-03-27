#include "system/SysMutex.h"

using namespace izanagi;

CMutex::CMutex()
{
	m_Mutex = IZ_NULL;
}


CMutex::~CMutex()
{
	Release();
}

/**
*/
IZ_BOOL CMutex::Init()
{
#ifdef WINDOWS
	m_Mutex = ::CreateMutex(
				IZ_NULL,
				IZ_FALSE,
				IZ_NULL);
	IZ_ASSERT(m_Mutex != IZ_NULL);

	return (m_Mutex != IZ_NULL);
#else
   pthread_mutexattr_t mattr;

   pthread_mutexattr_init(&mattr);
   pthread_mutex_init(&m_Mutex, &mattr);

   return IZ_TRUE;
#endif
}

/**
*/
void CMutex::Release()
{
#ifdef WINDOWS
	if (m_Mutex != IZ_NULL) {
		::WaitForSingleObject(m_Mutex, INFINITE);
		::CloseHandle(m_Mutex);
	}
#else
	pthread_mutex_lock(&m_Mutex);
	pthread_mutex_unlock(&m_Mutex);
	pthread_mutex_destroy(&m_Mutex);
#endif
}

/**
*/
void CMutex::Lock()
{
	CThread::ThreadId id = CThread::GetCurrentThreadId();
	if (CThread::IsEqualThreadId(m_ThreadId, id)) {
		// This mutex is already locked by this thread.
		return;
	}

#ifdef WINDOWS
	IZ_ASSERT(m_Mutex != IZ_NULL);
	::WaitForSingleObject(m_Mutex, INFINITE);
#else
#endif

	m_ThreadId = CThread::GetCurrentThreadId();
}

/**
*/
void CMutex::Unlock()
{
	CThread::ThreadId id = CThread::GetCurrentThreadId();
	if (!CThread::IsEqualThreadId(m_ThreadId, id)) {
		// This thread already released this mutex.
		return;
	}

	FILL_ZERO(&m_ThreadId, sizeof(m_ThreadId));

#ifdef WINDOWS
	IZ_ASSERT(m_Mutex != IZ_NULL);
	ReleaseMutex(m_Mutex);
#else
	// TODO
#endif
}
