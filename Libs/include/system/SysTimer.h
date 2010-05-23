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
		// �v���J�n
		void Begin();

		// �v���I��
		UN_FLOAT End();

		// �v�����Ԏ擾
		UN_FLOAT GetTime() { return m_fTime; }

	protected:
		UN_INT64 m_Frequency;
		UN_INT64 m_Begin;

		UN_FLOAT m_fTime;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_SYSTEM_SYS_TIMER_H__)
