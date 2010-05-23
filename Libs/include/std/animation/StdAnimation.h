#if !defined(__URANUS_STD_STD_ANIMATION_H__)
#define __URANUS_STD_STD_ANIMATION_H__

#include "unDefs.h"
#include "std/StdObject.h"
#include "StdTimeline.h"

namespace uranus {

	/**
	*/
	enum E_STD_ANM_MODE {
		E_STD_ANM_MODE_EQUATION_SHIFT  = 16,
		E_STD_ANM_MODE_EQUATION_MASK   = 0xffff0000,

		E_STD_ANM_MODE_TRANSITION_SHIFT = 0,
		E_STD_ANM_MODE_TRANSITION_MASK  = 0x0000ffff,

		E_STD_ANM_MODE_EASE_IN     = 1 << E_STD_ANM_MODE_EQUATION_SHIFT,
		E_STD_ANM_MODE_EASE_OUT	   = 2 << E_STD_ANM_MODE_EQUATION_SHIFT,
		E_STD_ANM_MODE_EASE_IN_OUT = 3 << E_STD_ANM_MODE_EQUATION_SHIFT,

		E_STD_ANM_MODE_TRANSITION_BEGIN = 0,
			E_STD_ANM_MODE_TRANSITION_BACK = E_STD_ANM_MODE_TRANSITION_BEGIN,
			E_STD_ANM_MODE_TRANSITION_BOUNCE,
			E_STD_ANM_MODE_TRANSITION_CIRC,
			E_STD_ANM_MODE_TRANSITION_CUBIC,
			E_STD_ANM_MODE_TRANSITION_ELASTIC,
			E_STD_ANM_MODE_TRANSITION_EXPO,
			E_STD_ANM_MODE_TRANSITION_QUAD,
			E_STD_ANM_MODE_TRANSITION_QUART,
			E_STD_ANM_MODE_TRANSITION_QUINT,
			E_STD_ANM_MODE_TRANSITION_SINE,
			E_STD_ANM_MODE_TRANSITION_LINEAR,
		E_STD_ANM_MODE_TRANSITION_END = E_STD_ANM_MODE_TRANSITION_LINEAR,
		E_STD_ANM_MODE_TRANSITION_NUM,

		/////////////////////////////////////////////////

		E_STD_ANM_MODE_BACK_EASE_IN     = E_STD_ANM_MODE_TRANSITION_BACK | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_BACK_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_BACK | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_BACK_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_BACK | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_BOUNCE_EASE_IN     = E_STD_ANM_MODE_TRANSITION_BOUNCE | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_BOUNCE_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_BOUNCE | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_BOUNCE_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_BOUNCE | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_CIRC_EASE_IN     = E_STD_ANM_MODE_TRANSITION_CIRC | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_CIRC_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_CIRC | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_CIRC_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_CIRC | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_CUBIC_EASE_IN     = E_STD_ANM_MODE_TRANSITION_CUBIC | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_CUBIC_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_CUBIC | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_CUBIC_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_CUBIC | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_ELASTIC_EASE_IN     = E_STD_ANM_MODE_TRANSITION_ELASTIC | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_ELASTIC_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_ELASTIC | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_ELASTIC_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_ELASTIC | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_EXPO_EASE_IN     = E_STD_ANM_MODE_TRANSITION_EXPO | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_EXPO_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_EXPO | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_EXPO_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_EXPO | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_QUAD_EASE_IN     = E_STD_ANM_MODE_TRANSITION_QUAD | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_QUAD_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_QUAD | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_QUAD_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_QUAD | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_QUART_EASE_IN     = E_STD_ANM_MODE_TRANSITION_QUART | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_QUART_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_QUART | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_QUART_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_QUART | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_QUINT_EASE_IN     = E_STD_ANM_MODE_TRANSITION_QUINT | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_QUINT_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_QUINT | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_QUINT_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_QUINT | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_SINE_EASE_IN     = E_STD_ANM_MODE_TRANSITION_SINE | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_SINE_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_SINE | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_SINE_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_SINE | E_STD_ANM_MODE_EASE_IN_OUT,

		E_STD_ANM_MODE_LINEAR_EASE_IN     = E_STD_ANM_MODE_TRANSITION_LINEAR | E_STD_ANM_MODE_EASE_IN,
		E_STD_ANM_MODE_LINEAR_EASE_OUT    = E_STD_ANM_MODE_TRANSITION_LINEAR | E_STD_ANM_MODE_EASE_OUT,
		E_STD_ANM_MODE_LINEAR_EASE_IN_OUT = E_STD_ANM_MODE_TRANSITION_LINEAR | E_STD_ANM_MODE_EASE_IN_OUT,
	};

	class CStdTimeline;

	/**
	*/
	class CStdAnimation {
	public:
		CStdAnimation()
		{
			SetMode(E_STD_ANM_MODE_LINEAR_EASE_IN);
			m_fMin = 0.0f;
			m_fMax = 0.0f;
		}

		CStdAnimation(E_STD_ANM_MODE mode, UN_FLOAT fMin, UN_FLOAT fMax)
		{
			SetMode(mode);
			m_fMin = fMin;
			m_fMax = fMax;
		}

		~CStdAnimation() {}

		NO_COPIABLE(CStdAnimation);

	public:
		UN_FLOAT GetValue(const CStdTimeline& cTimeline);

		void Init(
			E_STD_ANM_MODE mode,
			UN_FLOAT fMin, UN_FLOAT fMax)
		{
			SetMinMax(fMin, fMax);
			SetMode(mode);
		}

		void SetMinMax(UN_FLOAT fMin, UN_FLOAT fMax)
		{
			m_fMin = MIN(fMin, fMax);
			m_fMax = MAX(fMin, fMax);
		}

		UN_FLOAT GetMin() const { return m_fMin; }
		UN_FLOAT GetMax() const { return m_fMax; }

		void SetMode(E_STD_ANM_MODE mode);
		E_STD_ANM_MODE GetMode() const;

	protected:
		E_STD_ANM_MODE m_nTransition;
		E_STD_ANM_MODE m_nEquation;

		UN_FLOAT m_fMin;
		UN_FLOAT m_fMax;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_ANIMATION_H__)
