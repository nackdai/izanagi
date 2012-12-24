#include "animation/TweenerInterpolater.h"
#include "animation/Timeline.h"
#include "math/Tweener.h"

namespace izanagi {
namespace animation {

    static math::CTweenerBack       s_TweenerBack;
    static math::CTweenerBounce     s_TweenerBounce;
    static math::CTweenerCirc       s_TweenerCirc;
    static math::CTweenerCubic      s_TweenerCubic;
    static math::CTweenerElastic    s_TweenerElastic;
    static math::CTweenerExpo       s_TweenerExpo;
    static math::CTweenerQuad       s_TweenerQuad;
    static math::CTweenerQuart      s_TweenerQuart;
    static math::CTweenerQuint      s_TweenerQuint;
    static math::CTweenerSine       s_TweenerSine;
    static math::CTweenerLinear     s_TweenerLinear;

    static math::ITweenerEasing* TweenEasingTbl[] = {
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

    IZ_C_ASSERT(COUNTOF(TweenEasingTbl) == E_ANM_TWEENER_TRANSITION_NUM);

    /**
    * Return animation value by timeline.
    */
    IZ_FLOAT CTweenerInterpolater::GetValue(const CTimeline& timeline)
    {
        math::ITweenerEasing* easing = TweenEasingTbl[m_Transition];

        IZ_FLOAT time = timeline.GetTime();
        IZ_FLOAT duration = timeline.GetDuration();

        IZ_FLOAT ret = 0.0f;

        switch (m_Equation) {
        case E_ANM_TWEENER_EQUATION_EASE_IN:
            ret = easing->EaseIn(time, duration);
            break;
        case E_ANM_TWEENER_EQUATION_EASE_OUT:
            ret = easing->EaseOut(time, duration);
            break;
        case E_ANM_TWEENER_EQUATION_EASE_IN_OUT:
            ret = easing->EaseInOut(time, duration);
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        IZ_FLOAT range = m_End - m_Start;

        ret = range * ret + m_Start;

        return ret;
    }

    /**
    * Set animation mode.
    */
    void CTweenerInterpolater::SetMode(E_ANM_TWEENER_MODE mode)
    {
        m_Transition = static_cast<E_ANM_TWEENER_TRANSITION>(mode & E_ANM_TWEENER_TRANSITION_MASK);
        m_Equation =  static_cast<E_ANM_TWEENER_EQUATION>(mode & E_ANM_TWEENER_EQUATION_MASK);

        IZ_ASSERT(IS_IN_BOUND(m_Transition, E_ANM_TWEENER_TRANSITION_BEGIN, E_ANM_TWEENER_TRANSITION_END));
        IZ_ASSERT(IS_IN_BOUND(m_Equation, E_ANM_TWEENER_EQUATION_EASE_IN, E_ANM_TWEENER_EQUATION_EASE_IN_OUT));
    }

    /**
    * Return animation mode.
    */
    E_ANM_TWEENER_MODE CTweenerInterpolater::GetMode() const
    {
        IZ_UINT ret = ((m_Transition << E_ANM_TWEENER_TRANSITION_SHIFT) & E_ANM_TWEENER_TRANSITION_MASK)
                        | ((m_Equation << E_ANM_TWEENER_EQUATION_SHIFT) & E_ANM_TWEENER_EQUATION_MASK);
        return static_cast<E_ANM_TWEENER_MODE>(ret);
    }

}   // namespace animtion 
}   // namespace izanagi
