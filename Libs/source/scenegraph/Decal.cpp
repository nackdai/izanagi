#include "scenegraph/Decal.h"
#include "scenegraph/SceneGraphUtil.h"

namespace izanagi {
namespace scenegraph {
    CDecal::CDecal()
    {
        m_Allocator = IZ_NULL;

        m_TriNum = 0;
        m_Triangles = IZ_NULL;

        m_NeedCreateGraphicsObject = IZ_FALSE;

        m_VB = IZ_NULL;
        m_VD = IZ_NULL;
    }

    CDecal::~CDecal()
    {
        FREE(m_Allocator, m_Triangles);

        SAFE_RELEASE(m_VB);
        SAFE_RELEASE(m_VD);
    }

    void CDecal::SetRectangle(
        const izanagi::math::SVector4& point,
        const izanagi::math::SVector4& normal,
        IZ_FLOAT rectangleLengthX,
        IZ_FLOAT rectangleLengthZ)
    {
        const math::CVector4 vecX(rectangleLengthX, 0.0f, 0.0f, 0.0f);
        const math::CVector4 vecZ(0.0f, 0.0f, rectangleLengthZ, 0.0f);

        m_Rectangle.Set(point, vecX, vecZ);

        // 矩形の法線が指定された法線に一致する回転を計算する
        izanagi::math::SQuat rotQuat;
        izanagi::math::SQuat::RotationArc(
            rotQuat,
            m_Rectangle.nml,
            normal);

        // 矩形の法線が指定された法線に一致するように回転
        izanagi::math::SMatrix44 rotMtx;
        izanagi::math::SQuat::MatrixFromQuat(rotMtx, rotQuat);
        m_Rectangle.Transform(rotMtx);

        IZ_FLOAT width = rectangleLengthX * 0.5f;
        IZ_FLOAT height = rectangleLengthZ * 0.5f;

        const izanagi::math::SVector4& t = m_Rectangle.v[0];
        const izanagi::math::SVector4& b = m_Rectangle.v[1];
        const izanagi::math::SVector4& n = m_Rectangle.nml;

        izanagi::math::CVector4 minusT(-t.x, -t.y, -t.z, 0.0f);
        izanagi::math::CVector4 minusB(-b.x, -b.y, -b.z, 0.0f);
        izanagi::math::CVector4 minusN(-n.x, -n.y, -n.z, 0.0f);

        IZ_FLOAT dotTP = izanagi::math::SVector4::Dot(t, point);
        IZ_FLOAT dotBP = izanagi::math::SVector4::Dot(b, point);
        IZ_FLOAT dotNP = izanagi::math::SVector4::Dot(n, point);

        // NOTE
        // +-----> T
        // |
        // |
        // B

        // NOTE
        // +---------> T
        //  L       R
        //  |       |
        //  +--> <--+
        //  |       |

        m_Planes[PLANE_LEFT].Set(t, width - dotTP);
        m_Planes[PLANE_RIGHT].Set(minusT, width + dotTP);

        m_Planes[PLANE_BOTTOM].Set(b, height - dotBP);
        m_Planes[PLANE_TOP].Set(minusB, height + dotBP);

        // TODO
        static const IZ_FLOAT d = 1.0f;

        m_Planes[PLANE_FRONT].Set(minusN, d + dotNP);
        m_Planes[PLANE_BACK].Set(n, d - dotNP);
    }

    // デカール描画用のグラフィックスオブジェクトを作成
    void CDecal::CreateGraphicsObject(izanagi::graph::CGraphicsDevice* device)
    {
        if (!m_NeedCreateGraphicsObject)
        {
            return;
        }

        IZ_ASSERT(device != IZ_NULL);

        m_NeedCreateGraphicsObject = IZ_FALSE;

        SAFE_RELEASE(m_VB);
        SAFE_RELEASE(m_VD);

        IZ_UINT vtxNum = m_TriNum * 3;

        // Vertex Buffer
        m_VB = device->CreateVertexBuffer(
            sizeof(SVtx),
            vtxNum,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        IZ_ASSERT(m_VB != IZ_NULL);

        SVtx* data = IZ_NULL;

        // Write vertices
        m_VB->Lock(device, 0, 0, (void**)&data, IZ_FALSE);
        {
            IZ_UINT vtxPos = 0;

            for (IZ_UINT i = 0; i < m_TriNum; i++)
            {
                const izanagi::math::CTriangle& tri = m_Triangles[i];

                for (IZ_UINT n = 0; n < 3; n++)
                {
                    data[vtxPos + n].point.Set(
                        tri.pt[n].x,
                        tri.pt[n].y,
                        tri.pt[n].z);

                    data[vtxPos + n].color = IZ_COLOR_RGBA(0xff, 0, 0, 0xff);

                    // TODO
                    data[vtxPos + n].uv[0] = 0.0f;
                    data[vtxPos + n].uv[1] = 0.0f;
                }

                vtxPos += 3;
            }
        }
        m_VB->Unlock(device);

        // Vertex Declaration

        static const izanagi::graph::SVertexElement elems[] =
        {
            {0,  0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0},    // 座標
            {0, 16, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_COLOR,  izanagi::graph::E_GRAPH_VTX_DECL_USAGE_COLOR,    0},    // 色
            {0, 20, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT2, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD, 0},    // UV
        };

        m_VD = device->CreateVertexDeclaration(elems, COUNTOF(elems));
        IZ_ASSERT(m_VD != IZ_NULL);
    }

    // デカール描画
    void CDecal::Draw(izanagi::graph::CGraphicsDevice* device)
    {
        IZ_ASSERT(device != IZ_NULL);

        if (m_VB == NULL || m_VD == NULL)
        {
            return;
        }

        device->SetVertexDeclaration(m_VD);
        device->SetVertexBuffer(0, 0, m_VB->GetStride(), m_VB);

#if 0
        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_ZENABLE,
            IZ_FALSE);

        device->DrawPrimitive(
            izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST,
            0, m_TriNum);

        device->SetRenderState(
            izanagi::graph::E_GRAPH_RS_ZENABLE,
            IZ_TRUE);
#else
        device->DrawPrimitive(
            izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST,
            0, m_TriNum);
#endif
    }

    // デカール中心を取得
    const izanagi::math::SVector4& CDecal::GetCenter() const
    {
        return m_Rectangle.pt;
    }

    ////////////////////////////////////////////////////

    // 指定された三角形をもとにデカールメッシュを計算
    void CDecalAlloc::ComputeDecalMesh(
        const izanagi::math::CTriangle tri[],
        IZ_UINT triNum)
    {
        // シザリングによって新しく作成される三角形の数を計算する
        ComputeNewTriNumByScissoring(tri, triNum);

        if (m_TriNum == 0)
        {
            // シザリングによって新しく作成される三角形がないので終了
            return;
        }

        // 念のため
        FREE(m_Allocator, m_Triangles);

        IZ_UINT tmpBufPos = 0;

        // 一時的なバッファ
        izanagi::math::CTriangle* tmp[2];
        {
            tmp[0] = (izanagi::math::CTriangle*)ALLOC_ZERO(m_Allocator, sizeof(izanagi::math::CTriangle) * m_TriNum);
            tmp[1] = (izanagi::math::CTriangle*)ALLOC_ZERO(m_Allocator, sizeof(izanagi::math::CTriangle) * m_TriNum);
        }

        // 生成された三角形の出力先
        izanagi::math::CTriangle* triDst = tmp[tmpBufPos];

        // 元の三角形
        const izanagi::math::CTriangle* triSrc = tri;
        IZ_UINT srcNum = triNum;

        tmpBufPos = 1 - tmpBufPos;

        // 面ごとにシザリングする
        for (IZ_UINT n = 0; n < PLANE_NUM; n++)
        {
            IZ_UINT newTriNum = 0;

            newTriNum = izanagi::CScissoring::Scissor(
                m_Planes[n],
                triSrc, srcNum,
                triDst,
                m_TriNum);

            IZ_ASSERT(newTriNum < m_TriNum);

            srcNum = newTriNum;

            if (srcNum == 0)
            {
                break;
            }

            // シザリングされた結果をさらにシザリングする
            triSrc = triDst;
            triDst = tmp[tmpBufPos];

            tmpBufPos = 1 - tmpBufPos;
        }

        // 最終的に生成された三角形数
        m_TriNum = srcNum;

        if (m_TriNum > 0)
        {
            // 結果を格納する
            m_Triangles = (izanagi::math::CTriangle*)ALLOC_ZERO(m_Allocator, sizeof(izanagi::math::CTriangle) * m_TriNum);

            memcpy(m_Triangles, &triSrc[0], sizeof(izanagi::math::CTriangle) * m_TriNum);

            m_NeedCreateGraphicsObject = IZ_TRUE;
        }

        FREE(m_Allocator, tmp[0]);
        FREE(m_Allocator, tmp[1]);
    }

    // シザリングによって新しく作成される三角形の数を計算する
    void CDecalAlloc::ComputeNewTriNumByScissoring(
        const izanagi::math::CTriangle tri[],
        IZ_UINT triNum)
    {
#if 1
        m_TriNum = 0;

        for (IZ_UINT n = 0; n < PLANE_NUM; n++)
        {
            m_TriNum += CScissoring::ComputeTriNumByScissoring(
                m_Planes[n],
                tri,
                triNum);
        }
#else
        m_TriNum = 0;

        for (IZ_UINT i = 0; i < triNum; i++)
        {
            IZ_FLOAT dot = izanagi::math::SVector4::Dot(
                m_Rectangle.nml,
                tri[i].nml);

            if (dot >= 0.0f)
            {
                m_TriNum++;
            }
        }
#endif
    }

    ////////////////////////////////////////////////////

    // 指定された三角形をもとにデカールメッシュを計算
    void CDecalSTL::ComputeDecalMesh(
        const izanagi::math::CTriangle tri[],
        IZ_UINT triNum)
    {
        // 念のため
        FREE(m_Allocator, m_Triangles);

        STriListProxy triListProxy;

        IZ_UINT tmpBufPos = 0;

        // 一時的なバッファ
        izanagi::math::CTriangle triBuf[2];

        // 生成された三角形の出力先
        TriangleVector* triDst = &triListProxy.Get(tmpBufPos);

        // 元の三角形
        const izanagi::math::CTriangle* triSrc = tri;
        IZ_UINT srcNum = triNum;

        tmpBufPos = 1 - tmpBufPos;

        // 面ごとにシザリングする
        for (IZ_UINT n = 0; n < PLANE_NUM; n++)
        {
            IZ_UINT newTriNum = 0;


            newTriNum = CScissoring::Scissor(
                m_Planes[n],
                triSrc, srcNum,
                *triDst);

            srcNum = newTriNum;

            if (srcNum == 0)
            {
                break;
            }

            // シザリングされた結果をさらにシザリングする
            triSrc = &(*triDst)[0];
            triDst = &triListProxy.Get(tmpBufPos);

            // 次に向けてクリア
            triDst->clear();

            tmpBufPos = 1 - tmpBufPos;
        }

        // 最終的に生成された三角形数
        m_TriNum = srcNum;

        if (m_TriNum > 0)
        {
            // 結果を格納する
            m_Triangles = (izanagi::math::CTriangle*)ALLOC_ZERO(m_Allocator, sizeof(izanagi::math::CTriangle) * m_TriNum);

            memcpy(m_Triangles, &triSrc[0], sizeof(izanagi::math::CTriangle) * m_TriNum);

            m_NeedCreateGraphicsObject = IZ_TRUE;
        }
    }
}   // namespace scenegraph
}   // namespace izanagi
