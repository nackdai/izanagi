#include "animation/TweenerUtility.h"
#include "animation/Timeline.h"
#include "animation/Tweener.h"

namespace izanagi {
namespace animation {

    static animation::CTweenerBack       s_TweenerBack;
    static animation::CTweenerBounce     s_TweenerBounce;
    static animation::CTweenerCirc       s_TweenerCirc;
    static animation::CTweenerCubic      s_TweenerCubic;
    static animation::CTweenerElastic    s_TweenerElastic;
    static animation::CTweenerExpo       s_TweenerExpo;
    static animation::CTweenerQuad       s_TweenerQuad;
    static animation::CTweenerQuart      s_TweenerQuart;
    static animation::CTweenerQuint      s_TweenerQuint;
    static animation::CTweenerSine       s_TweenerSine;
    static animation::CTweenerLinear     s_TweenerLinear;

    static animation::ITweenerEasing* TweenEasingTbl[] = {
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
    IZ_FLOAT CTweener::GetValue(const CTimeline& timeline)
    {
        IZ_FLOAT time = timeline.GetTime();
        IZ_FLOAT duration = timeline.GetDuration();

        return GetValue(time, duration);
    }

    IZ_FLOAT CTweener::GetValue(IZ_FLOAT time, IZ_FLOAT duration)
    {
        animation::ITweenerEasing* easing = TweenEasingTbl[m_Transition];

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
    void CTweener::SetMode(E_ANM_TWEENER_MODE mode)
    {
        m_Transition = static_cast<E_ANM_TWEENER_TRANSITION>(mode & E_ANM_TWEENER_TRANSITION_MASK);
        m_Equation =  static_cast<E_ANM_TWEENER_EQUATION>(mode & E_ANM_TWEENER_EQUATION_MASK);

        IZ_ASSERT(IS_IN_BOUND(m_Transition, E_ANM_TWEENER_TRANSITION_BEGIN, E_ANM_TWEENER_TRANSITION_END));
        IZ_ASSERT(IS_IN_BOUND(m_Equation, E_ANM_TWEENER_EQUATION_EASE_IN, E_ANM_TWEENER_EQUATION_EASE_IN_OUT));
    }

    /**
    * Return animation mode.
    */
    E_ANM_TWEENER_MODE CTweener::GetMode() const
    {
        IZ_UINT ret = ((m_Transition << E_ANM_TWEENER_TRANSITION_SHIFT) & E_ANM_TWEENER_TRANSITION_MASK)
                        | ((m_Equation << E_ANM_TWEENER_EQUATION_SHIFT) & E_ANM_TWEENER_EQUATION_MASK);
        return static_cast<E_ANM_TWEENER_MODE>(ret);
    }

}   // namespace animtion 
}   // namespace izanagi
