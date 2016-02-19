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

    template <typename NODE>
    class Octree {
    public:
        static const IZ_UINT MAX_LEVEL = 7;

    public:
        Octree() {}
        ~Octree() {}

        NO_COPIABLE(Octree);

    public:
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

        IZ_BOOL initialize(
            IMemoryAllocator* allocator,
            const math::SVector4& vMin,
            const math::SVector4& vMax,
            IZ_UINT level)
        {
            // レベルを決める.
            m_level = IZ_MIN(MAX_LEVEL, level);

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
            math::SVector4::Div(m_unit, m_size, div);
            m_unit.w = 0.0f;

            m_min = vMin;
            m_max = vMax;

            return IZ_TRUE;
        }

        NODE* getNode(IZ_UINT mortonNumber)
        {
            VRETURN_NULL(mortonNumber < m_nodeCount);
            
            IZ_ASSERT(m_nodes != nullptr);

            NODE* ret = m_nodes[mortonNumber];

            if (!ret) {
                m_nodes[mortonNumber] = createNode();
                ret = m_nodes[mortonNumber];
                ret->initialize(mortonNumber);
            }

            return ret;
        }

        void getViewNode(
            Frustum& frustum,
            std::vector<NODE*> nodes)
        {
#if 1
            getViewNode(0, frustum, nodes);
#else
            for (IZ_UINT i = 0; i < m_nodeCount; i++) {
                NODE* node = m_nodes[i];

                if (node) {
                    // Check if node is intersected by frustum.
                    AABB aabb;
                    node->getAABB(aabb);

                    auto isIntersect = frustum.isIntersect(aabb);

                    if (isIntersect) {
                        nodes.push_back(node);
                    }
                }
            }
#endif
        }

    private:
        void getViewNode(
            IZ_UINT mortonNumber,
            Frustum& frustum,
            std::vector<NODE*> nodes)
        {
            NODE* node = m_nodes[mortonNumber];

            if (node) {
                // Check if node is intersected by frustum.
                AABB aabb;
                node->getAABB(aabb);

                auto isIntersect = frustum.isIntersect(aabb);

                if (isIntersect) {
                    nodes.push_back(node);
                }
            }

            // 子供を処理.
            for (IZ_UINT i = 0; i < 7; i++) {
                IZ_UINT id = mortonNumber * 8 + 1 + i;

                getViewNode(id, frustum, nodes);
            }
        }

    private:
        inline IZ_UINT separeteBit(IZ_BYTE n)
        {
            IZ_UINT s = n;
            s = (s | s << 8) & 0x0000f00f;
            s = (s | s << 4) & 0x000c30c3;
            s = (s | s << 2) & 0x00249249;
            return s;
        }

        inline NODE* createNode()
        {
            void* buf = ALLOC(m_allocator, sizeof(NODE));
            NODE* node = new(buf)NODE;
            return node;
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
