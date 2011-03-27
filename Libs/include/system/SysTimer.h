#if !defined(__IZANAGI_SYSTEM_SYS_TIMER_H__)
#define __IZANAGI_SYSTEM_SYS_TIMER_H__

#include "izDefs.h"

namespace izanagi {
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
		IZ_FLOAT End();

		// 計測時間取得
		IZ_FLOAT GetTime() { return m_fTime; }

	protected:
		IZ_INT64 m_Frequency;
		IZ_INT64 m_Begin;

		IZ_FLOAT m_fTime;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_TIMER_H__)
