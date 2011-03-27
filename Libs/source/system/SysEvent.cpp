#include "system/SysEvent.h"

using namespace izanagi;

CEvent::CEvent()
{
}

CEvent::~CEvent()
{
}

IZ_BOOL CEvent::Init()
{
#ifdef WINDOWS
	m_Event = ::CreateEvent(
				IZ_NULL,
				IZ_FALSE,
				IZ_FALSE,
				IZ_NULL);
	IZ_ASSERT(m_Event != IZ_NULL);

	return (m_Event != IZ_NULL);
#else
	pthread_mutexattr_t mattr;
	
	pthread_mutexattr_init(&mattr);
	pthread_mutex_init(&m_Lock, &mattr);
	pthread_cond_init(&m_Cond, IZ_NULL);

	// TODO
	return IZ_TRUE;
#endif	
}

void CEvent::Release()
{
#ifdef WINDOWS
	if (m_Event != IZ_NULL) {
		::CloseHandle(m_Event);
	}
#else
	pthread_cond_destroy(&m_Cond);
	pthread_mutex_destroy(&m_Lock);
#endif

}

void CEvent::Set()
{
#ifdef WINDOWS
	IZ_ASSERT(m_Event != IZ_NULL);
	::SetEvent(m_Event);
#else
	pthread_mutex_lock(&m_Lock);
	pthread_mutex_unlock(&m_Lock);
	pthread_cond_signal(&m_Cond);
#endif
}

IZ_BOOL CEvent::Wait()
{
#ifdef WINDOWS
	IZ_DWORD result = ::WaitForSingleObject(m_Event,INFINITE);
	IZ_ASSERT(result == WAIT_OBJECT_0);

	return (result == WAIT_OBJECT_0);
#else
	pthread_mutex_lock(&m_Lock);
	pthread_cond_wait(&m_Cond, &m_Lock);
	return IZ_TRUE;
#endif
}

void CEvent::Reset()
{
#ifndef WINDOWS
	pthread_mutex_unlock(&m_Lock);
#endif
}
