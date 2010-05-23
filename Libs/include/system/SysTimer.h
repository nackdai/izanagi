#if !defined(__URANUS_SYSTEM_SYS_TIMER_H__)
#define __URANUS_SYSTEM_SYS_TIMER_H__

#include "unDefs.h"

namespace uranus {
	/**
	* Timer
	*/
	class CTimer {
	public:
		CTimer();
		~CTimer() {}

		NO_COPIABLE(CTimer);

	public:
		// 計測開始
		void Begin();

		// 計測終了
		UN_FLOAT End();

		// 計測時間取得
		UN_FLOAT GetTime() { return m_fTime; }

	protected:
		UN_INT64 m_Frequency;
		UN_INT64 m_Begin;

		UN_FLOAT m_fTime;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_SYSTEM_SYS_TIMER_H__)
