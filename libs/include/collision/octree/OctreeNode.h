#if !defined(__IZANAGI_COLLISION_OCTREE_NODE_H__)
#define __IZANAGI_COLLISION_OCTREE_NODE_H__

#include <vector>

#include "izDefs.h"
#include "izStd.h"

namespace izanagi
{
namespace col
{
    class OctreeElement;

    class OctreeNode {
        template <typename NODE> friend class Octree;

    public:
        OctreeNode() {}
        ~OctreeNode() {}

        NO_COPIABLE(OctreeNode);

    public:
        void initialize(IZ_UINT mortonNumber);

        IZ_BOOL add(OctreeElement& element);

        IZ_BOOL remove(OctreeElement& element);

        IZ_UINT getElementCount() const;

        IZ_UINT getMortonNumber() const;

    private:
        IZ_UINT m_mortonNumber{ 0 };
        
        CStdList<OctreeElement> m_list;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_OCTREE_NODE_H__)
