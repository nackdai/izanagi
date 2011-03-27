#if !defined(__IZANAGI_SYSTEM_SYS_MUTEX_H__)
#define __IZANAGI_SYSTEM_SYS_MUTEX_H__

#include "SysThread.h"

namespace izanagi {
	/**
	*/
	class CMutex {
	public:
		CMutex();
		~CMutex();

		NO_COPIABLE(CMutex);

	public:
		IZ_BOOL Init();
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
			IZ_ASSERT(m_Mutex != IZ_NULL);
			m_Mutex->Lock();
		}

		~CAutoMutex()
		{
			IZ_ASSERT(m_Mutex != IZ_NULL);
			m_Mutex->Unlock();
		}

		NO_COPIABLE(CAutoMutex);

	protected:
		CAutoMutex() {}

	protected:
		CMutex* m_Mutex;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_MUTEX_H__)
