#if !defined(__IZANAGI_SYSTEM_SYS_EVENT_H__)
#define __IZANAGI_SYSTEM_SYS_EVENT_H__

#include "izDefs.h"

namespace izanagi {
	/**
	*/
	class CEvent {
	public:
		CEvent();
		~CEvent();

		NO_COPIABLE(CEvent);

	public:
		IZ_BOOL Init();
		void Release();

		void Set();
		IZ_BOOL Wait();
		void Reset();

	protected:
#ifdef WINDOWS
		HANDLE m_Event;
#else
		pthread_cond_t m_Cond;
		pthread_mutex_t m_Lock;
#endif
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_EVENT_H__)
