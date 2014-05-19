#include "animation/Interpolator.h"

namespace izanagi
{
namespace animation
{
    CInterpolator::CInterpolator()
    {
    }

    void CInterpolator::Advance(IZ_FLOAT delta)
    {
        m_Timeline.Advance(delta);
    }

    void CInterpolator::EnableLoop(IZ_BOOL enable)
    {
        m_Timeline.EnableLoop(enable);
    }

    void CInterpolator::AutoReverse(IZ_BOOL enable)
    {
        m_Timeline.AutoReverse(enable);
    }

    void CInterpolator::Start()
    {
        m_Timeline.Start();
    }

    void CInterpolator::Pause()
    {
        m_Timeline.Pause();
    }

    void CInterpolator::Stop()
    {
        m_Timeline.Stop();
    }
}   // namespace izanagi
}   // namespace animation
