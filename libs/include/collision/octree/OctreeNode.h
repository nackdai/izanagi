#if !defined(__IZANAGI_COLLISION_OCTREE_NODE_H__)
#define __IZANAGI_COLLISION_OCTREE_NODE_H__

#include <vector>

#include "izDefs.h"
#include "izStd.h"

#include "collision/ColAABB.h"

namespace izanagi
{
namespace col
{
    class OctreeElement;

    /** Base of octree' node.
     */
    class IOctreeNode {
    public:
        IOctreeNode() {}
        virtual ~IOctreeNode() {}

    public:
        PURE_VIRTUAL(void getAABB(AABB& aabb));

        virtual void initialize(IZ_UINT mortonNumber)
        {
            m_mortonNumber = mortonNumber;
        }

        IZ_UINT getMortonNumber() const
        {
            return m_mortonNumber;
        }

    private:
        IZ_UINT m_mortonNumber{ 0 };
    };

    /** Implementation of octree's node.
     */
    class OctreeNode : public IOctreeNode {
        template <typename NODE> friend class Octree;

    public:
        OctreeNode() {}
        ~OctreeNode() {}

        NO_COPIABLE(OctreeNode);

        IZ_DECL_PLACEMENT_NEW();

    public:
        virtual void getAABB(AABB& aabb) override;

        void setAABB(
            const math::SVector4& minPtr,
            const math::SVector4& maxPtr);

        IZ_BOOL add(OctreeElement& element);

        IZ_BOOL remove(OctreeElement& element);

        IZ_UINT getElementCount() const;

    private:
        CStdList<OctreeElement> m_list;

        AABB m_aabb;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_OCTREE_NODE_H__)
