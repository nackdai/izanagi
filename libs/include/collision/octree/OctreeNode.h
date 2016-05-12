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

    /** Interface of octree's node.
     */
    class IOctreeNode : public CPlacementNew {
        template <typename NODE> friend class Octree;

    public:
        IOctreeNode()
        {
            FILL_ZERO(m_children, sizeof(m_children));
        }
        virtual ~IOctreeNode() {}

    public:
        PURE_VIRTUAL(void setAABB(const AABB& aabb));
        PURE_VIRTUAL(void getAABB(AABB& aabb));

        virtual void initialize(
            IZ_UINT mortonNumber,
            IZ_UINT8 level)
        {
            m_mortonNumber = mortonNumber;
            m_level = level;
        }

        IZ_UINT getMortonNumber() const
        {
            return m_mortonNumber;
        }

        IZ_UINT8 getLevel() const
        {
            return m_level;
        }

        void addChild(IZ_UINT idx, void* child)
        {
            IZ_ASSERT(idx < 8);

            m_children[idx] = child;
        }

        template <typename NODE>
        NODE** getChildren()
        {
            return (NODE**)m_children;
        }

        bool hasChildren()
        {
            return (m_children[0] != nullptr);
        }

        IZ_UINT getNodeArrayIdx()
        {
            return m_nodeArrayIdx;
        }

    private:
        void setNodeArrayIdx(IZ_UINT idx)
        {
            m_nodeArrayIdx = idx;
        }

    private:
        IZ_UINT m_nodeArrayIdx{ 0 };

        IZ_UINT m_mortonNumber{ 0 };
        IZ_UINT8 m_level{ 0 };

        void* m_children[8];
    };

    /** Implementation of octree's node.
     */
    class OctreeNode : public IOctreeNode {
        template <typename NODE> friend class Octree;

    public:
        OctreeNode() {}
        ~OctreeNode() {}

        NO_COPIABLE(OctreeNode);

        //IZ_DECL_PLACEMENT_NEW();

    public:
        virtual void setAABB(const AABB& aabb) override;
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
