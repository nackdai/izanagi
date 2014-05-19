#include "animation/SplineInterpolator.h"
#include "izMath.h"

namespace izanagi
{
namespace animation
{
    CSplineInterpolator* CSplineInterpolator::Create(
        IMemoryAllocator* allocator,
        IZ_FLOAT from, IZ_FLOAT to,
        IZ_FLOAT duration,
        IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
        IZ_FLOAT cp2X, IZ_FLOAT cp2Y)
    {
        void* buf = ALLOC_ZERO(allocator, sizeof(CSplineInterpolator));
        VRETURN_NULL(buf != IZ_NULL);

        CSplineInterpolator* ret = new(buf) CSplineInterpolator;
        ret->AddRef();
        ret->m_Allocator = allocator;
        ret->Init(
            from, to,
            duration,
            cp1X, cp1Y,
            cp2X, cp2Y);

        return ret;
    }

    CSplineInterpolator::CSplineInterpolator()
    {
    }

    CSplineInterpolator::~CSplineInterpolator()
    {
    }

    void CSplineInterpolator::Init(
        IZ_FLOAT from, IZ_FLOAT to,
        IZ_FLOAT duration,
        IZ_FLOAT cp1X, IZ_FLOAT cp1Y,
        IZ_FLOAT cp2X, IZ_FLOAT cp2Y)
    {
        m_From = from;
        m_To = to;

        m_Cp1.x = 0.0f;
        m_Cp1.y = 0.0f;
        
        m_Cp2.x = math::CMath::Clamp(cp1X, 0.0f, 1.0f);
        m_Cp2.y = math::CMath::Clamp(cp1Y, 0.0f, 1.0f);

        m_Cp3.x = math::CMath::Clamp(cp2X, 0.0f, 1.0f);
        m_Cp3.y = math::CMath::Clamp(cp2Y, 0.0f, 1.0f);

        m_Cp4.x = 1.0f;
        m_Cp4.y = 1.0f;

        m_Timeline.Init(duration, 0.0f);
    }

    CSplineInterpolator::Func::Func(CSplineInterpolator* interp, IZ_BOOL isDerivation)
    {
        interpolator = interp;
        this->isDerivation = isDerivation;
    }

    IZ_FLOAT CSplineInterpolator::Func::operator()(IZ_FLOAT t)
    {
        if (isDerivation) {
            return interpolator->BezierD(t);
        }
        else {
            return interpolator->BezierX(t);
        }
    }

    IZ_FLOAT CSplineInterpolator::BezierX(IZ_FLOAT t)
    {
        IZ_FLOAT x1 = m_Cp2.x;
        IZ_FLOAT x2 = m_Cp3.x;

        IZ_FLOAT ret = 3 * (1 - t) * (1 - t) * t * x1 + 3 * (1 - t) * t * t * x2 + t * t * t;
        return ret;
    }

    IZ_FLOAT CSplineInterpolator::BezierY(IZ_FLOAT t)
    {
        IZ_FLOAT y1 = m_Cp2.y;
        IZ_FLOAT y2 = m_Cp3.y;

        IZ_FLOAT ret = 3 * (1 - t) * (1 - t) * t * y1 + 3 * (1 - t) * t * t * y2 + t * t * t;
        return ret;
    }

    IZ_FLOAT CSplineInterpolator::BezierD(IZ_FLOAT t)
    {
        IZ_FLOAT x1 = m_Cp2.x;
        IZ_FLOAT x2 = m_Cp3.x;

        IZ_FLOAT ret = -6 * (1 - t) * t * x1 + 3 * (1 - t) * (1 - t) * x1
            - 3 * t * t * x2 + 6 * (1 - t) * t * x2 + 3 * t * t;

        return ret;
    }

    IZ_FLOAT CSplineInterpolator::ComputeX(
        Func& func,
        Func& funcd,
        IZ_FLOAT time,
        IZ_FLOAT threshold,
        IZ_UINT loopCnt)
    {
        IZ_FLOAT x = time;
        IZ_FLOAT newX = 0.0f;

        for (IZ_UINT i = 0; i < loopCnt; i++) {
            IZ_FLOAT v = func(x) - time;

            if (izanagi::math::CMath::Absf(v) < threshold) {
                return x;
            }

            IZ_FLOAT dx = funcd(x);

            if (izanagi::math::CMath::IsNearyEqualZero(dx)) {
                return x;
            }

            newX = x - v /dx;
            x = newX;

            if (izanagi::math::CMath::Absf(x) < threshold) {
                return x;
            }
        }

        return x;
    }

    IZ_FLOAT CSplineInterpolator::GetValue(IZ_FLOAT time, IZ_FLOAT duration)
    {
        IZ_FLOAT normTime = (duration != 0.0f ? time / duration : 0.0f);
        IZ_FLOAT ret = GetValue(normTime);
        return ret;
    }

    IZ_FLOAT CSplineInterpolator::GetValue(IZ_FLOAT normTime)
    {
        IZ_FLOAT t = ComputeX(
            Func(this, IZ_FALSE),
            Func(this, IZ_TRUE),
            normTime,
            0.00001f,
            10);

        IZ_FLOAT ret = BezierY(t);
        
        ret = m_From + (m_To - m_From) * ret;

        return ret;
    }

    IZ_FLOAT CSplineInterpolator::GetValue()
    {
        IZ_FLOAT time = m_Timeline.GetNormalized();
        IZ_FLOAT ret = GetValue(time);
        return ret;
    }
}   // namespace izanagi
}   // namespace animation
