#include "animation/Interpolator.h"

namespace izanagi
{
namespace animation
{
    CInterpolator::CInterpolator()
    {
        m_ListItem.Init(this);
    }

    void CInterpolator::Advance(IZ_FLOAT delta)
    {
        m_Timeline.Start();
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
}   // namespace izanagi
}   // namespace animation
