#if !defined(__URANUS_SYSTEM_SYS_EVENT_H__)
#define __URANUS_SYSTEM_SYS_EVENT_H__

#include "unDefs.h"

namespace uranus {
	/**
	*/
	class CEvent {
	public:
		CEvent();
		~CEvent();

		NO_COPIABLE(CEvent);

	public:
		UN_BOOL Init();
		void Release();

		void Set();
		UN_BOOL Wait();
		void Reset();

	protected:
#ifdef WINDOWS
		HANDLE m_Event;
#else
		pthread_cond_t m_Cond;
		pthread_mutex_t m_Lock;
#endif
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_SYSTEM_SYS_EVENT_H__)
