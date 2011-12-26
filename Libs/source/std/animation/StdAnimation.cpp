#include "std/animation/StdAnimation.h"
#include "std/animation/Tweener.h"

using namespace izanagi;

static CTweenerBack		s_TweenerBack;
static CTweenerBounce	s_TweenerBounce;
static CTweenerCirc		s_TweenerCirc;
static CTweenerCubic	s_TweenerCubic;
static CTweenerElastic	s_TweenerElastic;
static CTweenerExpo		s_TweenerExpo;
static CTweenerQuad		s_TweenerQuad;
static CTweenerQuart	s_TweenerQuart;
static CTweenerQuint	s_TweenerQuint;
static CTweenerSine		s_TweenerSine;
static CTweenerLinear	s_TweenerLinear;

static ITweenerEasing* TweenEasingTbl[] = {
	&s_TweenerBack,
	&s_TweenerBounce,
	&s_TweenerCirc,
	&s_TweenerCubic,
	&s_TweenerElastic,
	&s_TweenerExpo,
	&s_TweenerQuad,
	&s_TweenerQuart,
	&s_TweenerQuint,
	&s_TweenerSine,
	&s_TweenerLinear,
};

IZ_C_ASSERT(COUNTOF(TweenEasingTbl) == E_STD_ANM_MODE_TRANSITION_NUM);

/**
* Return animation value by timeline.
*/
IZ_FLOAT CStdAnimation::GetValue(const CStdTimeline& cTimeline)
{
	IZ_ASSERT(IS_IN_BOUND(m_nTransition, E_STD_ANM_MODE_TRANSITION_BEGIN, E_STD_ANM_MODE_TRANSITION_END));
	IZ_ASSERT(IS_IN_BOUND(m_nEquation, E_STD_ANM_MODE_EASE_IN, E_STD_ANM_MODE_EASE_IN_OUT));

	ITweenerEasing* pEasing = TweenEasingTbl[m_nTransition];

	IZ_FLOAT fTime = cTimeline.GetTime();
	IZ_FLOAT fDuration = cTimeline.GetDuration();

	IZ_FLOAT ret = 0.0f;

	switch (m_nEquation) {
	case E_STD_ANM_MODE_EASE_IN:
		ret = pEasing->EaseIn(fTime, fDuration);
		break;
	case E_STD_ANM_MODE_EASE_OUT:
		ret = pEasing->EaseOut(fTime, fDuration);
		break;
	case E_STD_ANM_MODE_EASE_IN_OUT:
		ret = pEasing->EaseInOut(fTime, fDuration);
		break;
	default:
		IZ_ASSERT(IZ_FALSE);
		break;
	}

	IZ_FLOAT fRange = m_fEnd - m_fStart;

	ret = fRange * ret + m_fStart;

	return ret;
}

/**
* Set animation mode.
*/
void CStdAnimation::SetMode(E_STD_ANM_MODE mode)
{
	m_nTransition = static_cast<E_STD_ANM_MODE>(mode & E_STD_ANM_MODE_TRANSITION_MASK);
	m_nEquation =  static_cast<E_STD_ANM_MODE>(mode & E_STD_ANM_MODE_EQUATION_MASK);

	IZ_ASSERT(IS_IN_BOUND(m_nTransition, E_STD_ANM_MODE_TRANSITION_BEGIN, E_STD_ANM_MODE_TRANSITION_END));
	IZ_ASSERT(IS_IN_BOUND(m_nEquation, E_STD_ANM_MODE_EASE_IN, E_STD_ANM_MODE_EASE_IN_OUT));
}

/**
* Return animation mode.
*/
E_STD_ANM_MODE CStdAnimation::GetMode() const
{
	IZ_UINT ret = ((m_nTransition << E_STD_ANM_MODE_TRANSITION_SHIFT) & E_STD_ANM_MODE_TRANSITION_MASK)
					| ((m_nEquation << E_STD_ANM_MODE_EQUATION_SHIFT) & E_STD_ANM_MODE_EQUATION_MASK);
	return static_cast<E_STD_ANM_MODE>(ret);
}
