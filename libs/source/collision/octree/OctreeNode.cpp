#include "collision/octree/OctreeNode.h"
#include "collision/octree/OctreeElement.h"

namespace izanagi
{
namespace col
{    
    void OctreeNode::getAABB(AABB& aabb)
    {
        aabb = m_aabb;
    }

    void OctreeNode::setAABB(
        const math::SVector4& minPtr,
        const math::SVector4& maxPtr)
    {
        m_aabb.initialize(minPtr, maxPtr);
    }

    IZ_BOOL OctreeNode::add(OctreeElement& element)
    {
        element.remove();

        return m_list.AddTail(element.getListItem());
    }

    IZ_BOOL OctreeNode::remove(OctreeElement& element)
    {
        return m_list.Remove(element.getListItem());
    }

    IZ_UINT OctreeNode::getElementCount() const
    {
        auto ret = m_list.GetItemNum();
        return ret;
    }
}   // namespace math
}   // namespace izanagi
