#if !defined(__IZANAGI_COLLISION_OCTREE_ELEMENT_H__)
#define __IZANAGI_COLLISION_OCTREE_ELEMENT_H__

#include <vector>

#include "izDefs.h"
#include "izStd.h"

namespace izanagi
{
namespace col
{
    class OctreeNode;

    /** Ocrtee's node.
     */
    class OctreeElement {
        friend class OctreeNode;

    public:
        OctreeElement();
        ~OctreeElement();

        NO_COPIABLE(OctreeElement);

    public:
        void setData(void* data);

        void* getData();

        OctreeNode* getNode();

        IZ_BOOL remove();

    private:
        void setNode(OctreeNode* node);

        CStdList<OctreeElement>::Item* getListItem();

    private:
        OctreeNode* m_node{ nullptr };
        void* m_data{ nullptr };
        CStdList<OctreeElement>::Item m_listItem;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_OCTREE_ELEMENT_H__)
