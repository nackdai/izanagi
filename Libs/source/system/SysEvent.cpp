#include "system/SysEvent.h"

using namespace uranus;

CEvent::CEvent()
{
}

CEvent::~CEvent()
{
}

UN_BOOL CEvent::Init()
{
#ifdef WINDOWS
	m_Event = ::CreateEvent(
				UN_NULL,
				UN_FALSE,
				UN_FALSE,
				UN_NULL);
	UN_ASSERT(m_Event != UN_NULL);

	return (m_Event != UN_NULL);
#else
	pthread_mutexattr_t mattr;
	
	pthread_mutexattr_init(&mattr);
	pthread_mutex_init(&m_Lock, &mattr);
	pthread_cond_init(&m_Cond, UN_NULL);

	// TODO
	return UN_TRUE;
#endif	
}

void CEvent::Release()
{
#ifdef WINDOWS
	if (m_Event != UN_NULL) {
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
	UN_ASSERT(m_Event != UN_NULL);
	::SetEvent(m_Event);
#else
	pthread_mutex_lock(&m_Lock);
	pthread_mutex_unlock(&m_Lock);
	pthread_cond_signal(&m_Cond);
#endif
}

UN_BOOL CEvent::Wait()
{
#ifdef WINDOWS
	UN_DWORD result = ::WaitForSingleObject(m_Event,INFINITE);
	UN_ASSERT(result == WAIT_OBJECT_0);

	return (result == WAIT_OBJECT_0);
#else
	pthread_mutex_lock(&m_Lock);
	pthread_cond_wait(&m_Cond, &m_Lock);
	return UN_TRUE;
#endif
}

void CEvent::Reset()
{
#ifndef WINDOWS
	pthread_mutex_unlock(&m_Lock);
#endif
}
