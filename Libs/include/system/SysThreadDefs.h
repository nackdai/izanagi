#if !defined(__IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__)
#define __IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__

#if defined(WIN32) || defined(WIN64)
	#include <windows.h>

	typedef UINT	ThreadId;

	#define ThreadHandle	HANDLE
	#define MutexHandle		HANDLE
	#define EventHandle		HANDLE
	#define SemaHandle		HANDLE
#else
	#include <pthread.h>

	typedef pthread_t	ThreadId;

	#define MutexHandle	pthread_mutex_t
#endif

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__)
