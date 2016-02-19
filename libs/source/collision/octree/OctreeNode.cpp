#include "collision/octree/OctreeNode.h"
#include "collision/octree/OctreeElement.h"

namespace izanagi
{
namespace col
{
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

    IZ_UINT OctreeNode::getMortonNumber() const
    {
        return m_mortonNumber;
    }
}   // namespace math
}   // namespace izanagi
