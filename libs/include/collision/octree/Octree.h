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

    struct MortonNumber {
        IZ_UINT number{ 0 };
        IZ_UINT level{ 0 };

        MortonNumber() {}
        MortonNumber(IZ_UINT _n, IZ_UINT _l) : number(_n), level(_l) {}
    };

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

    private:
        static inline IZ_UINT getBit(IZ_UINT n)
        {
            auto ret = n & 0x01;
            ret |= (n >> 2) & 0x02;
            ret |= (n >> 4) & 0x04;
            ret |= (n >> 6) & 0x08;
            ret |= (n >> 8) & 0x10;
            ret |= (n >> 10) & 0x20;
            ret |= (n >> 12) & 0x40;
            ret |= (n >> 16) & 0x80;
            return ret;
        }

    public:
        static void getPosFromMortonNumber(
            const MortonNumber& mortonNumber,
            IZ_UINT& outX, IZ_UINT& outY, IZ_UINT& outZ)
        {
            auto n = mortonNumber.number;

            outX = n & 0x00249249;
            outX = getBit(outX);

            outY = n & 0x00492492;
            outY >>= 1;
            outY = getBit(outY);

            outZ = n & 0x00924924;
            outZ >>= 2;
            outZ = getBit(outZ);
        }

        inline IZ_UINT getIndex(const MortonNumber& mortonNumber)
        {
            // 指定レベルまでの要素数.
            IZ_UINT count = (m_nodesNum[mortonNumber.level] - 1) / 7;

            // 全体を通じたインデックスに変換.
            IZ_UINT idx = mortonNumber.number + count;

            return idx;
        }

        MortonNumber getMortonNumber(IZ_UINT idx)
        {
            MortonNumber mortonNumber;

            if (idx > 0) {
                auto tmp = idx - 1;
                auto level = 1;

                for (;;) {
                    tmp >>= 3;
                    if (tmp == 0) {
                        break;
                    }

                    IZ_ASSERT(tmp >= 1);
                    tmp -= 1;

                    level++;
                }

                IZ_UINT count = (m_nodesNum[level] - 1) / 7;
                IZ_UINT number = idx - count;

                mortonNumber.number = number;
                mortonNumber.level = level;
            }
            else {
                mortonNumber.number = 0;
                mortonNumber.level = 0;
            }

            return std::move(mortonNumber);
        }

        /** Compute morton number.
         * This method returns a morton number which belongs to max level.
         */
        void getMortonNumber(
            MortonNumber& ret,
            const math::SVector4& point)
        {
            getMortonNumberByLevel(ret, point, m_level - 1);
        }

        void getMortonNumberByLevel(
            MortonNumber& ret,
            const IZ_FLOAT* point,
            IZ_UINT level)
        {
            IZ_ASSERT(level < m_level);

#if 0
            math::CVector4 unit(m_unit);
            unit *= (1 << (m_level - 1 - level));
#else
            //math::CVector4 unit(m_units[level]);
            const auto& unit = m_divUnits[level];
#endif

#if 0
            math::SVector4 tmp;
            math::SVector4::SubXYZ(tmp, point, m_min);

            tmp.x /= unit.x;
            tmp.y /= unit.y;
            tmp.z /= unit.z;

            auto x = separeteBit((IZ_BYTE)tmp.x);
            auto y = separeteBit((IZ_BYTE)tmp.y);
            auto z = separeteBit((IZ_BYTE)tmp.z);

            IZ_UINT number = (x | (y << 1) | (z << 2));
#else
            float ptX = point[0] - m_min.x;
            float ptY = point[1] - m_min.y;
            float ptZ = point[2] - m_min.z;

            ptX *= unit.x;
            ptY *= unit.y;
            ptZ *= unit.z;

            int iX = _mm_cvttss_si32(_mm_load_ss(&ptX));
            int iY = _mm_cvttss_si32(_mm_load_ss(&ptY));
            int iZ = _mm_cvttss_si32(_mm_load_ss(&ptZ));

            ret.number = separeteBit(iX);
            ret.number |= separeteBit(iY) << 1;
            ret.number |= separeteBit(iZ) << 2;
#endif

            IZ_ASSERT(ret.number < m_nodesNum[level]);

            //ret.number = number;
            ret.level = level;
        }

        IZ_UINT getIndex(const math::SVector4& point)
        {
            return getIndex(point, m_level - 1);
        }

        /** Compute morton number which the region belongs to.
         */
        void getMortonNumber(
            MortonNumber& ret,
            const math::SVector4& vMin,
            const math::SVector4& vMax)
        {
            MortonNumber minNumber, maxNumber;
            getMortonNumber(minNumber, vMin);
            getMortonNumber(maxNumber, vMax);
            
            auto def = maxNumber.number ^ minNumber.number;

            IZ_UINT topLevel = 1;

            for (IZ_UINT i = 0; i < m_level; i++) {
                auto check = (def >> (i * 3)) & 0x7;
                if (check != 0) {
                    topLevel = i + 1;
                }
                else {
                    break;
                }
            }

            IZ_UINT number = maxNumber >> (topLevel * 3);

            ret.number = number;
            ret.level = topLevel - 1;
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
            IZ_UINT div = (1 << (m_level - 1));
            math::SVector4::Div(m_unit, m_size, (IZ_FLOAT)div);
            m_unit.w = 0.0f;

            m_units[m_level - 1] = m_unit;

            m_divUnits[m_level - 1].x = 1.0f / m_unit.x;
            m_divUnits[m_level - 1].y = 1.0f / m_unit.y;
            m_divUnits[m_level - 1].z = 1.0f / m_unit.z;
            m_divUnits[m_level - 1].w = 0.0;

            for (IZ_INT i = m_level - 2; i >= 0; i--) {
                m_units[i].x = m_units[i + 1].x * 2.0f;
                m_units[i].y = m_units[i + 1].y * 2.0f;
                m_units[i].z = m_units[i + 1].z * 2.0f;

                m_divUnits[i].x = 1.0f / m_units[i].x;
                m_divUnits[i].y = 1.0f / m_units[i].y;
                m_divUnits[i].z = 1.0f / m_units[i].z;
                m_divUnits[i].w = 0.0;
            }

            m_min = vMin;
            m_max = vMax;

            m_min.w = m_max.w = 0.0f;

            return IZ_TRUE;
        }

        IZ_UINT getMaxLevel() const
        {
            return m_level;
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
            const MortonNumber& mortonNumber,
            IZ_BOOL isCreateNodeIfNoExist = IZ_TRUE)
        {
            auto idx = getIndex(mortonNumber);
            return getNode(idx, isCreateNodeIfNoExist);
        }

        /** Get node by index.
         */
        NODE* getNode(
            IZ_UINT idx,
            IZ_BOOL isCreateNodeIfNoExist = IZ_TRUE)
        {
            VRETURN_NULL(idx < m_nodeCount);
            
            IZ_ASSERT(m_nodes != nullptr);

            NODE* ret = m_nodes[idx];

            if (!ret && isCreateNodeIfNoExist) {
                // If there is no node, create a new node.
                ret = createNodeByIdx(idx, nullptr, nullptr);
                m_nodes[idx] = ret;
            }

            return ret;
        }

        NODE* getNodeByMortonNumber(
            const MortonNumber& mortonNumber,
            IZ_BOOL isCreateNodeIfNoExist = IZ_TRUE)
        {
            auto idx = getIndex(mortonNumber);

            IZ_ASSERT(m_nodes != nullptr);

            NODE* ret = m_nodes[idx];

            if (!ret && isCreateNodeIfNoExist) {
                // If there is no node, create a new node.
                ret = createNodeByIdx(idx, &mortonNumber, nullptr);
                m_nodes[idx] = ret;
            }

            return ret;
        }

        /** Get children of the specified node.
         */
        NODE** getChildren(
            NODE* node,
            IZ_BOOL isCreateNodeIfNoExist = IZ_TRUE)
        {
            IZ_ASSERT(node);

            if (node->hasChildren()) {
                return node->getChildren<NODE>();
            }

            MortonNumber mortonNumber;
            mortonNumber.number = node->getMortonNumber();
            mortonNumber.level = node->getLevel();

            auto idx = getIndex(mortonNumber);

            // 子供の起点インデックス.
            auto childrenStartIdx = idx * 8 + 1;
            
            if (childrenStartIdx >= m_nodeCount) {
                return nullptr;
            }

            if (isCreateNodeIfNoExist && !node->hasChildren()) {
                for (IZ_UINT i = 0; i < 8; i++) {
                    auto id = childrenStartIdx + i;
                    IZ_ASSERT(id < m_nodeCount);

                    if (!m_nodes[id]) {
                        m_nodes[id] = createNodeByIdx(id, nullptr, node);
                    }
                }
            }

            return &m_nodes[childrenStartIdx];
        }

        /** Return value from function to traverse nodes.
         * First value means if node is accepted.
         * Secont value means if children will be traversed.
         */
        using RetvalTraverse = std::tuple < IZ_BOOL, IZ_BOOL > ;

        /** Function to traverse nodes.
         */
        using FuncTraverse = std::function < RetvalTraverse(NODE*) >;

        /** Traverse nodes.
         */
        void traverse(
            FuncTraverse func,
            std::vector<NODE*>& nodes,
            IZ_BOOL isCreateNodeIfNoExist = IZ_TRUE)
        {
            traverse(0, 0, func, nodes, isCreateNodeIfNoExist);
        }

        /** Get visible nodes by view furstum.
         */
        void getVisibleNodes(
            Frustum& frustum,
            std::vector<NODE*>& nodes,
            IZ_BOOL isCreateNodeIfNoExist = IZ_TRUE)
        {
            traverse(
                [&](NODE* node) {
                AABB aabb;
                node->getAABB(aabb);

                auto isVisible = frustum.isIntersect(&aabb);

                // 親が見えていないときは子供も見えないので、子供の探索をさせない.
                IZ_BOOL willTraverseChildren = isVisible;

                return RetvalTraverse(isVisible, willTraverseChildren);
            }, nodes, isCreateNodeIfNoExist);
        }

        /** Get count of nodes which the octree has.
         */
        IZ_UINT getNodeCount() const
        {
            return m_nodeCount;
        }

        IZ_UINT getNodeCount(IZ_UINT level) const
        {
            IZ_ASSERT(level < m_level);
            return m_nodesNum[level];
        }

        /** Get list of nodes which the octree has.
         */
        NODE** getNodes()
        {
            return m_nodes;
        }

        NODE** getNodes(IZ_UINT level)
        {
            if (level == 0) {
                return m_nodes;
            }

            IZ_ASSERT(level < m_level);
            IZ_UINT pos = (m_nodesNum[level] - 1) / 7;

            return &m_nodes[pos];
        }

    private:
        NODE* getParentByMortonNumber(const MortonNumber& mortonNumber)
        {
            if (mortonNumber.level == 0) {
                return nullptr;
            }

            auto level = mortonNumber.level - 1;
            auto number = mortonNumber.number / 8;

            MortonNumber parentMortonNumber(number, level);

            auto idx = getIndex(parentMortonNumber);

            auto ret = m_nodes[idx];
            
            if (!ret) {
                m_nodes[idx] = createNodeByIdx(idx, &parentMortonNumber, nullptr);
                ret = m_nodes[idx];
            }

            return ret;
        }

        NODE* getParentByIdx(IZ_UINT idx)
        {
            auto mortonNumber = getMortonNumber(idx);
            return getParentByMortonNumber(mortonNumber);
        }

        // Create node.
        NODE* createNodeByIdx(IZ_UINT idx, const MortonNumber* specifiedMortonNumber, NODE* parent)
        {
            if (m_nodes[idx]) {
                return m_nodes[idx];
            }

            m_nodes[idx] = createNode(m_allocator);
            auto ret = m_nodes[idx];

            MortonNumber mortonNumber;

            if (specifiedMortonNumber) {
                mortonNumber = *specifiedMortonNumber;
            }
            else {
                mortonNumber = getMortonNumber(idx);
            }

            auto level = mortonNumber.level;

            ret->initialize(mortonNumber.number, level);

            IOctreeNode* node = (IOctreeNode*)ret;
            node->setNodeArrayIdx(idx);

            // Set AABB.
            {
                IZ_UINT posX, posY, posZ;
                getPosFromMortonNumber(
                    mortonNumber,
                    posX, posY, posZ);

                // Compute size.
#if 0
                izanagi::math::CVector4 size(m_unit);
                izanagi::math::SVector4::ScaleXYZ(size, size, (IZ_FLOAT)(1 << (m_level - 1 - level)));

                // Compute min position.
                izanagi::math::CVector4 minPos(m_min);
                minPos.x += size.x * posX;
                minPos.y += size.y * posY;
                minPos.z += size.z * posZ;

                izanagi::math::CVector4 maxPos(minPos);
                maxPos.x += size.x;
                maxPos.y += size.y;
                maxPos.z += size.z;
#else
                izanagi::math::SVector4 size;
                izanagi::math::SVector4::ScaleXYZ(size, m_unit, (IZ_FLOAT)(1 << (m_level - 1 - level)));

                // Compute min position.
                izanagi::math::SVector4 minPos;
                minPos.x = m_min.x + size.x * posX;
                minPos.y = m_min.y + size.y * posY;
                minPos.z = m_min.z + size.z * posZ;

                izanagi::math::SVector4 maxPos;
                maxPos.x = minPos.x + size.x;
                maxPos.y = minPos.y + size.y;
                maxPos.z = minPos.z + size.z;
#endif

                AABB aabb;
                aabb.initialize(minPos, maxPos);
                ret->setAABB(aabb);
            }

            if (!parent) {
                parent = getParentByIdx(idx);
            }

            if (parent) {
                auto pos = mortonNumber.number & 0x07;
                parent->addChild(pos, ret);
            }

            return ret;
        }

        // Traverse nodes.
        void traverse(
            IZ_UINT idx,
            IZ_UINT level,
            FuncTraverse func,
            std::vector<NODE*>& nodes,
            IZ_BOOL isCreateNodeIfNoExist)
        {
            if (level >= m_level) {
                return;
            }

            NODE* node = getNode(idx, isCreateNodeIfNoExist);

            RetvalTraverse ret(IZ_FALSE, IZ_FALSE);

            if (node) {
                ret = func(node);

                auto isAccepted = std::get<0>(ret);

                if (isAccepted) {
                    nodes.push_back(node);
                }
            }

            auto willTraverseChildren = std::get<1>(ret);

            if (willTraverseChildren) {
                // 子供を処理.
                for (IZ_UINT i = 0; i < 8; i++) {
                    IZ_UINT id = idx * 8 + i + 1;

                    traverse(id, level + 1, func, nodes, isCreateNodeIfNoExist);
                }
            }
        }

    protected:
        // ノード作成.
        virtual NODE* createNode(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC(allocator, sizeof(NODE));
            NODE* node = new(buf)NODE;
            return node;
        }

        // ノード破棄.
        virtual void deleteNode(IMemoryAllocator* allocator, NODE* node)
        {
            delete node;
            FREE(m_allocator, node);
        }

    private:
        //static inline IZ_UINT separeteBit(IZ_BYTE n)
        static inline IZ_UINT separeteBit(IZ_UINT n)
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

        math::CVector4 m_min;
        math::CVector4 m_max;

        // 全体サイズ.
        math::CVector4 m_size;

        // 最小単位サイズ.
        math::CVector4 m_unit;

        math::CVector4 m_units[MAX_LEVEL];

        struct {
            float x, y, z, w;
        } m_divUnits[MAX_LEVEL];
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_OCTREE_H__)
