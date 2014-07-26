#if !defined(__IZ_SCENEGRAPH_DECAL_H__)
#define __IZ_SCENEGRAPH_DECAL_H__

#include "izGraph.h"
#include "izMath.h"

#include <vector>

#define ENABLE_STL

namespace izanagi {
namespace scenegraph {
    /** デカール
     */
    class CDecal : public izanagi::CObject
    {
    protected:
        /** インスタンス作成
         *
         * @param[in] allocator
         * @param[in] point デカール中心
         * @param[in] normal デカール法線
         * @param[in] rectangleLengthX デカール矩形のX軸方向長さ
         * @param[in] rectangleLengthZ デカール矩形のZ軸方向長さ
         */
        template <typename _DECAL>
        static CDecal* Create(
            izanagi::IMemoryAllocator* allocator,
            const izanagi::math::SVector4& point,
            const izanagi::math::SVector4& normal,
            IZ_FLOAT rectangleLengthX,
            IZ_FLOAT rectangleLengthZ)
        {
            izanagi::STLMemoryAllocatorProxy::SetAllocator(allocator);

            void* buf = ALLOC_ZERO(allocator, sizeof(CDecal));

            CDecal* instance = new(buf) _DECAL;
            {
                instance->m_Allocator = allocator;
                instance->AddRef();

                instance->SetRectangle(
                    point, 
                    normal,
                    rectangleLengthX,
                    rectangleLengthZ);
            }

            return instance;
        }

    protected:
        CDecal();
        virtual ~CDecal();

        NO_COPIABLE(CDecal);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** デカール矩形情報をセット
         *
         * @param[in] point デカール中心
         * @param[in] normal デカール法線
         * @param[in] rectangleLengthX デカール矩形のX軸方向長さ
         * @param[in] rectangleLengthZ デカール矩形のZ軸方向長さ
         */
        void SetRectangle(
            const izanagi::math::SVector4& point,
            const izanagi::math::SVector4& normal,
            IZ_FLOAT rectangleLengthX,
            IZ_FLOAT rectangleLengthZ);

        /** 指定された三角形をもとにデカールメッシュを計算
         *
         * @param[in] シザリングを行う三角形の配列
         * @param[in] 三角形の数
         */
        virtual void ComputeDecalMesh(
            const izanagi::math::CTriangle tri[],
            IZ_UINT triNum) = 0;

        /** デカール描画用のグラフィックスオブジェクトを作成
         *
         * @param[in] device グラフィックスデバイス
         */
        void CreateGraphicsObject(izanagi::graph::CGraphicsDevice* device);

        /** デカール描画
         *
         * @param[in] device グラフィックスデバイス
         */
        void Draw(izanagi::graph::CGraphicsDevice* device);

        /** デカール中心を取得
         *
         * @return デカール中心
         */
        const izanagi::math::SVector4& GetCenter() const;

    protected:
        izanagi::IMemoryAllocator* m_Allocator;

        izanagi::math::CRectangle m_Rectangle;

        enum {
            PLANE_LEFT,
            PLANE_RIGHT,
            PLANE_BOTTOM,
            PLANE_TOP,
            PLANE_FRONT,
            PLANE_BACK,

            PLANE_NUM,
        };

        // シザリング用の面
        izanagi::math::CPlane m_Planes[PLANE_NUM];

        // デカールを構成する三角形数
        IZ_UINT m_TriNum;

        // デカールを構成する三角形
        izanagi::math::CTriangle* m_Triangles;

        struct SVtx {
            izanagi::math::SVector4 point;
            IZ_COLOR color;
            IZ_FLOAT uv[2];
        };

        // グラフィックスオブジェクトを作成する必要があるかどうか
        IZ_BOOL m_NeedCreateGraphicsObject;

        izanagi::graph::CVertexBuffer* m_VB;
        izanagi::graph::CVertexDeclaration* m_VD;
    };

    /**
     */
    class CDecalAlloc : public CDecal {
        friend class CDecal;

    public:
        static CDecal* Create(
            izanagi::IMemoryAllocator* allocator,
            const izanagi::math::SVector4& point,
            const izanagi::math::SVector4& normal,
            IZ_FLOAT rectangleLengthX,
            IZ_FLOAT rectangleLengthZ)
        {
            CDecal* ret = CDecal::Create<CDecalAlloc>(
                allocator,
                point, normal,
                rectangleLengthX, rectangleLengthZ);
            return ret;
        }

    protected:
        CDecalAlloc() {}
        virtual ~CDecalAlloc() {}

    public:
        /** 指定された三角形をもとにデカールメッシュを計算
         *
         * @param[in] シザリングを行う三角形の配列
         * @param[in] 三角形の数
         */
        virtual void ComputeDecalMesh(
            const izanagi::math::CTriangle tri[],
            IZ_UINT triNum);

    private:
        // シザリングによって新しく作成される三角形の数を計算する
        void ComputeNewTriNumByScissoring(
            const izanagi::math::CTriangle tri[],
            IZ_UINT triNum);
    };

    /**
     */
    class CDecalSTL : public CDecal {
     friend class CDecal;

    public:
        static CDecal* Create(
            izanagi::IMemoryAllocator* allocator,
            const izanagi::math::SVector4& point,
            const izanagi::math::SVector4& normal,
            IZ_FLOAT rectangleLengthX,
            IZ_FLOAT rectangleLengthZ)
        {
            CDecal* ret = CDecal::Create<CDecalSTL>(
                allocator,
                point, normal,
                rectangleLengthX, rectangleLengthZ);
            return ret;
        }

    protected:
        CDecalSTL() {}
        virtual ~CDecalSTL() {}

    public:
        /** 指定された三角形をもとにデカールメッシュを計算
         *
         * @param[in] シザリングを行う三角形の配列
         * @param[in] 三角形の数
         */
        virtual void ComputeDecalMesh(
            const izanagi::math::CTriangle tri[],
            IZ_UINT triNum);

    private:
        typedef std::vector<izanagi::math::CTriangle, izanagi::STLMemoryAllocator<izanagi::math::CTriangle> > TriangleVector;

        struct STriListProxy
        {
            TriangleVector triList_0;
            TriangleVector triList_1;

            TriangleVector& Get(IZ_UINT idx)
            {
                if (idx == 0)
                {
                    return triList_0;
                }
                else
                {
                    return triList_1;
                }
            }

            void Clear()
            {
                triList_0.clear();
                triList_1.clear();
            }
        };
    };
}   // namespace scenegraph
}   // namespace izanagi

#endif    // #if !defined(__IZ_SCENEGRAPH_DECAL_H__)
