#if !defined(__URANUS_SYSTEM_SYS_THREAD_H__)
#define __URANUS_SYSTEM_SYS_THREAD_H__

#include "unDefs.h"

namespace uranus {
	/**
	*/
	class CThread {
		friend class CMutex;

	private:
#ifdef WINDOWS
		typedef UN_DWORD ThreadId;
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
		static UN_BOOL IsEqualThreadId(
			const ThreadId& t1,
			const ThreadId& t2);
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_SYSTEM_SYS_THREAD_H__)
