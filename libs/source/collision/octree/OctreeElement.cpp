#include "collision/octree/OctreeElement.h"
#include "collision/octree/OctreeNode.h"

namespace izanagi
{
namespace col
{
    OctreeElement::OctreeElement()
    {
        m_listItem.Init(this);
    }

    OctreeElement::~OctreeElement()
    {
        remove();
    }

    void OctreeElement::setData(void* data)
    {
        m_data = data;
    }

    void* OctreeElement::getData()
    {
        return m_data;
    }

    OctreeNode* OctreeElement::getNode()
    {
        return m_node;
    }

    IZ_BOOL OctreeElement::remove()
    {
        IZ_BOOL ret = IZ_FALSE;

        if (m_node) {
            ret = m_node->remove(*this);
            m_node = nullptr;
        }

        return ret;
    }

    void OctreeElement::setNode(OctreeNode* node)
    {
        IZ_ASSERT(m_node == nullptr);
        m_node = node;
    }

    CStdList<OctreeElement>::Item* OctreeElement::getListItem()
    {
        return &m_listItem;
    }
}   // namespace math
}   // namespace izanagi
