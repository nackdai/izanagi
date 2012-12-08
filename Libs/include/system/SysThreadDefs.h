#if !defined(__IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__)
#define __IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__

#if defined(WIN32) || defined(WIN64)
	#include <windows.h>

namespace izanagi
{
namespace sys
{
	typedef UINT	ThreadId;

	#define ThreadHandle	HANDLE
	#define MutexHandle		HANDLE
	#define EventHandle		HANDLE
	#define SemaHandle		HANDLE
#else
	#include <pthread.h>

namespace izanagi
{
namespace sys
{
	typedef pthread_t	ThreadId;

	#define MutexHandle	pthread_mutex_t
#endif
}   // namespace sys
}   // namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_THREAD_DEFS_H__)
