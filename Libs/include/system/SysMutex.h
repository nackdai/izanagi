#if !defined(__URANUS_SYSTEM_SYS_MUTEX_H__)
#define __URANUS_SYSTEM_SYS_MUTEX_H__

#include "SysThread.h"

namespace uranus {
	/**
	*/
	class CMutex {
	public:
		CMutex();
		~CMutex();

		NO_COPIABLE(CMutex);

	public:
		UN_BOOL Init();
		void Release();

		void Lock();
		void Unlock();

	protected:
#ifdef WINDOWS
		HANDLE m_Mutex;
#else
		pthread_mutex_t m_mutex;
#endif

		CThread::ThreadId m_ThreadId;
	};

	////////////////////////////////////////////////////

	/**
	*/
	class CAutoMutex {
	public:
		CAutoMutex(CMutex* mutex)
		{
			m_Mutex = mutex;
			UN_ASSERT(m_Mutex != UN_NULL);
			m_Mutex->Lock();
		}

		~CAutoMutex()
		{
			UN_ASSERT(m_Mutex != UN_NULL);
			m_Mutex->Unlock();
		}

		NO_COPIABLE(CAutoMutex);

	protected:
		CAutoMutex() {}

	protected:
		CMutex* m_Mutex;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_SYSTEM_SYS_MUTEX_H__)
