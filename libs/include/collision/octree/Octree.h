#if !defined(__IZANAGI_COLLISION_OCTREE_H__)
#define __IZANAGI_COLLISION_OCTREE_H__

#include <vector>

#include "izDefs.h"
#include "izMath.h"

#include "collision/ColFrustum.h"
#include "collision/ColAABB.h"

namespace izanagi
{
namespace col
{
    // NOTE
    // http://marupeke296.com/COL_2D_No8_QuadTree.html
    // http://marupeke296.com/COL_3D_No15_Octree.html

    /** Octree.
     */
    template <typename NODE>
    class Octree {
    public:
        static const IZ_UINT MAX_LEVEL = 7;

    public:
        Octree() {}
        ~Octree()
        {
            clear();
        }

        NO_COPIABLE(Octree);

    public:
        /** Compute morton number.
         */
        IZ_UINT getMortonNumber(const math::SVector4& point)
        {
            math::SVector4 tmp;
            math::SVector4::SubXYZ(tmp, point, m_min);

            tmp.x /= m_unit.x;
            tmp.y /= m_unit.y;
            tmp.z /= m_unit.z;

            auto x = separeteBit((IZ_BYTE)tmp.x);
            auto y = separeteBit((IZ_BYTE)tmp.y);
            auto z = separeteBit((IZ_BYTE)tmp.z);

            IZ_UINT ret = (x | (y << 1) | (z << 2));
            return ret;
        }

        /** Initialize octree.
         */
        IZ_BOOL initialize(
            IMemoryAllocator* allocator,
            const math::SVector4& vMin,
            const math::SVector4& vMax,
            IZ_UINT level)
        {
            // レベルを決める.
            m_level = IZ_MIN(MAX_LEVEL, level);
            m_level = (m_level == 0 ? MAX_LEVEL : m_level);

            // 各レベルでのノード数とノード総数を計算.
            m_nodeCount = 1;
            m_nodesNum[0] = 1;
            for (IZ_UINT i = 1; i < m_level; i++) {
                m_nodesNum[i] = m_nodesNum[i - 1] * 8;
                m_nodeCount += m_nodesNum[i];
            }

            // 配列確保.
            m_allocator = allocator;
            m_nodes = (NODE**)ALLOC_ZERO(m_allocator, sizeof(NODE*)* m_nodeCount);
            VRETURN(m_nodes != IZ_NULL);

            // 全体サイズを計算.
            math::SVector4::Sub(m_size, vMax, vMin);
            m_size.w = 0.0f;

            // 最小単位サイズを計算.
            IZ_UINT div = (1 << m_level);
            math::SVector4::Div(m_unit, m_size, (IZ_FLOAT)div);
            m_unit.w = 0.0f;

            m_min = vMin;
            m_max = vMax;

            return IZ_TRUE;
        }

        /** Clear octree.
         */
        void clear()
        {
            for (IZ_UINT i = 0; i < m_nodeCount; i++) {
                if (m_nodes[i]) {
                    deleteNode(m_allocator, m_nodes[i]);
                }
            }

            FREE(m_allocator, m_nodes);

            m_level = 0;
            m_nodeCount = 0;
        }

        /** Get node by morton number.
         */
        NODE* getNode(
            IZ_UINT mortonNumber,
            IZ_BOOL isCreateNodeIfNoExist = IZ_TRUE)
        {
            VRETURN_NULL(mortonNumber < m_nodeCount);
            
            IZ_ASSERT(m_nodes != nullptr);

            NODE* ret = m_nodes[mortonNumber];

            if (!ret && isCreateNodeIfNoExist) {
                // If there is no node, create a new node.
                m_nodes[mortonNumber] = createNode(m_allocator);
                ret = m_nodes[mortonNumber];
                ret->initialize(mortonNumber);

                // TODO
                // Set AABB.
            }

            return ret;
        }

        /** Get visible nodes by view furstum.
         */
        void getVisibleNodes(
            Frustum& frustum,
            std::vector<NODE*> nodes,
            IZ_BOOL isCreateNodeIfNoExist = IZ_TRUE)
        {
            getVisibleNodes(0, frustum, nodes, isCreateNodeIfNoExist);
        }

        IZ_UINT getNodeCount() const
        {
            return m_nodeCount;
        }

        NODE** getNodes()
        {
            return m_nodes;
        }

    private:
        /** Get visible nodes by view furstum.
         */
        void getVisibleNodes(
            IZ_UINT mortonNumber,
            Frustum& frustum,
            std::vector<NODE*> nodes,
            IZ_BOOL isCreateNodeIfNoExist)
        {
            NODE* node = getNode(mortonNumber);

            if (node) {
                // Check if node is intersected by frustum.
                AABB aabb;
                node->getAABB(aabb);

                auto isIntersect = frustum.isIntersect(&aabb);

                if (isIntersect) {
                    nodes.push_back(node);
                }
            }

            // 子供を処理.
            for (IZ_UINT i = 0; i < 7; i++) {
                IZ_UINT id = mortonNumber * 8 + 1 + i;

                getVisibleNodes(id, frustum, nodes, isCreateNodeIfNoExist);
            }
        }

    protected:
        virtual NODE* createNode(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC(allocator, sizeof(NODE));
            NODE* node = new(buf)NODE;
            return node;
        }

        virtual void deleteNode(IMemoryAllocator* allocator, NODE* node)
        {
            FREE(m_allocator, node);
        }

    private:
        static inline IZ_UINT separeteBit(IZ_BYTE n)
        {
            IZ_UINT s = n;
            s = (s | s << 8) & 0x0000f00f;
            s = (s | s << 4) & 0x000c30c3;
            s = (s | s << 2) & 0x00249249;
            return s;
        }

    private:
        IMemoryAllocator* m_allocator{ nullptr };

        NODE** m_nodes{ nullptr };

        // ノード総数.
        IZ_UINT m_nodeCount{ 0 };

        // 分割レベル.
        IZ_UINT m_level{ 0 };

        // 各レベルでのノード数.
        IZ_UINT m_nodesNum[MAX_LEVEL];

        math::SVector4 m_min;
        math::SVector4 m_max;

        // 全体サイズ.
        math::SVector4 m_size;

        // 最小単位サイズ.
        math::SVector4 m_unit;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_OCTREE_H__)
