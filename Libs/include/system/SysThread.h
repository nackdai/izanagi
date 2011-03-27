#if !defined(__IZANAGI_SYSTEM_SYS_THREAD_H__)
#define __IZANAGI_SYSTEM_SYS_THREAD_H__

#include "izDefs.h"

namespace izanagi {
	/**
	*/
	class CThread {
		friend class CMutex;

	private:
#ifdef WINDOWS
		typedef IZ_DWORD ThreadId;
#else
		typedef pthread_t ThreadId;
#endif

	public:
		/**
		* Get current thread id.
		*/
		static ThreadId GetCurrentThreadId();

		/**
		* Return whethrer two threadid are equal.
		*/
		static IZ_BOOL IsEqualThreadId(
			const ThreadId& t1,
			const ThreadId& t2);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_THREAD_H__)
