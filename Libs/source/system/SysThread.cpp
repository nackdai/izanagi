#include "system/SysThread.h"

using namespace izanagi;

/**
* Get current thread id.
*/
CThread::ThreadId CThread::GetCurrentThreadId()
{
		ThreadId ret;
#ifdef WINDOWS
		ret = static_cast<ThreadId>(::GetCurrentThreadId());
#else
		// TODO
#endif
		return ret;
}

/**
* Return whethrer two threadid are equal.
*/
IZ_BOOL CThread::IsEqualThreadId(
	 const ThreadId& t1,
	 const ThreadId& t2)
{
#ifdef WINDOWS
	return (t1 == t2);
#else
	// TODO
#endif
}
