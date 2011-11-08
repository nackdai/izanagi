#if !defined(__IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__)
#define __IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__

#if defined(WIN32) || defined(WIN64)
	#include <windows.h>

	typedef UINT	ThreadId;

	#define __IZ_MUTEX_IMPL_CRITICAL_SECTION__

	#define ThreadHandle	HANDLE
	#ifdef __IZ_MUTEX_IMPL_CRITICAL_SECTION__
		#define MutexHandle		CRITICAL_SECTION
	#else
		#define MutexHandle		HANDLE
	#endif
	#define EventHandle		HANDLE
	#define CondHandle		HANDLE
#else
	#include <pthread.h>

	typedef pthread_t	ThreadId;

	#define MutexHandle	pthread_mutex_t
#endif

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__)
