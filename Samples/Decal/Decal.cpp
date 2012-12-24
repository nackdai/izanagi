#include "Decal.h"
#include "izSceneGraph.h"

CDecal* CDecal::Create(
    izanagi::IMemoryAllocator* allocator,
    const izanagi::math::SVector& point,
    const izanagi::math::SVector& normal,
    IZ_FLOAT rectangleLengthX,
    IZ_FLOAT rectangleLengthZ)
{
    izanagi::STLMemoryAllocatorProxy::SetAllocator(allocator);

    void* buf = ALLOC_ZERO(allocator, sizeof(CDecal));

    CDecal* instance = new(buf) CDecal;
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
    const izanagi::math::SVector& point,
    const izanagi::math::SVector& normal,
    IZ_FLOAT rectangleLengthX,
    IZ_FLOAT rectangleLengthZ)
{
    m_Rectangle.Set(
        point,
        rectangleLengthX,
        rectangleLengthZ);

    // 矩形の法線が指定された法線に一致する回転を計算する
    izanagi::math::SQuat rotQuat;
    izanagi::math::SQuat::RotationArc(
        rotQuat,
        m_Rectangle.nml,
        normal);

    // 矩形の法線が指定された法線に一致するように回転
    izanagi::math::SMatrix rotMtx;
    izanagi::math::SQuat::MatrixFromQuat(rotMtx, rotQuat);
    m_Rectangle.Transform(rotMtx);

    IZ_FLOAT width = m_Rectangle.GetWidth() * 0.5f;
    IZ_FLOAT height = m_Rectangle.GetHeight() * 0.5f;

    const izanagi::math::SVector& t = m_Rectangle.GetX();
    const izanagi::math::SVector& b = m_Rectangle.GetY();
    const izanagi::math::SVector& n = m_Rectangle.nml;

    izanagi::math::CVector minusT(-t.x, -t.y, -t.z, 0.0f);
    izanagi::math::CVector minusB(-b.x, -b.y, -b.z, 0.0f);
    izanagi::math::CVector minusN(-n.x, -n.y, -n.z, 0.0f);

    IZ_FLOAT dotTP = izanagi::math::SVector::Dot(t, point);
    IZ_FLOAT dotBP = izanagi::math::SVector::Dot(b, point);
    IZ_FLOAT dotNP = izanagi::math::SVector::Dot(n, point);

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

void CDecal::DoScissoring(
    const izanagi::math::CTriangle tri[],
    IZ_UINT triNum)
{
#ifndef ENABLE_STL
    ComputeNewTriNumByScissoring(tri, triNum);

    if (m_TriNum == 0)
    {
        return;
    }
#endif

    FREE(m_Allocator, m_Triangles);

    STriListProxy triListProxy;

    IZ_UINT tmpBufPos = 0;

    // 一時的なバッファ
#ifdef ENABLE_STL
    izanagi::math::CTriangle triBuf[2];

    TriangleVector* triDst = &triListProxy.Get(tmpBufPos);
#else
    izanagi::math::CTriangle* tmp[2];
    {
        tmp[0] = (izanagi::math::CTriangle*)ALLOC_ZERO(m_Allocator, sizeof(izanagi::math::CTriangle) * m_TriNum);
        tmp[1] = (izanagi::math::CTriangle*)ALLOC_ZERO(m_Allocator, sizeof(izanagi::math::CTriangle) * m_TriNum);
    }
    izanagi::math::CTriangle* triDst = tmp[tmpBufPos];
#endif
    const izanagi::math::CTriangle* triSrc = tri;
    IZ_UINT srcNum = triNum;

    tmpBufPos = 1 - tmpBufPos;

    for (IZ_UINT n = 0; n < PLANE_NUM; n++)
    {
        IZ_UINT newTriNum = 0;

#if 0
        // Fro Debug...
        for (IZ_UINT i = 0; i < srcNum; i++)
        {
            newTriNum += izanagi::CSceneGraphUtil::Sissoring(
                m_Planes[n],
                &triSrc[i], 1,
                &triDst[newTriNum],
                m_TriNum);

            IZ_ASSERT(newTriNum < m_TriNum);
        }
#else
#ifdef ENABLE_STL
        newTriNum = izanagi::CSceneGraphUtil::Sissoring(
            m_Planes[n],
            triSrc, srcNum,
            *triDst);
#else
        newTriNum = izanagi::CSceneGraphUtil::Sissoring(
            m_Planes[n],
            triSrc, srcNum,
            triDst,
            m_TriNum);

        IZ_ASSERT(newTriNum < m_TriNum);
#endif
#endif
        srcNum = newTriNum;

        if (srcNum == 0)
        {
            break;
        }

#ifdef ENABLE_STL
        triSrc = &(*triDst)[0];
        triDst = &triListProxy.Get(tmpBufPos);

        // 次に向けてクリア
        triDst->clear();
#else
        triSrc = triDst;
        triDst = tmp[tmpBufPos];
#endif
        tmpBufPos = 1 - tmpBufPos;
    }

    m_TriNum = srcNum;

    if (m_TriNum > 0)
    {
        m_Triangles = (izanagi::math::CTriangle*)ALLOC_ZERO(m_Allocator, sizeof(izanagi::math::CTriangle) * m_TriNum);

        memcpy(m_Triangles, &triSrc[0], sizeof(izanagi::math::CTriangle) * m_TriNum);

        m_NeedCreateGraphicsObject = IZ_TRUE;
    }

#ifndef ENABLE_STL
    FREE(m_Allocator, tmp[0]);
    FREE(m_Allocator, tmp[1]);
#endif
}

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

    m_VB = device->CreateVertexBuffer(
        sizeof(SVtx),
        vtxNum,
        izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);
    IZ_ASSERT(m_VB != IZ_NULL);

    struct {
        izanagi::math::SVector pos;
        IZ_COLOR color;
        IZ_FLOAT uv[2];
    }* data = IZ_NULL;
    m_VB->Lock(0, 0, (void**)&data, IZ_FALSE);
    {
        IZ_UINT vtxPos = 0;

        for (IZ_UINT i = 0; i < m_TriNum; i++)
        {
            const izanagi::math::CTriangle& tri = m_Triangles[i];

            for (IZ_UINT n = 0; n < 3; n++)
            {
                data[vtxPos + n].pos.Set(
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
    m_VB->Unlock();

    static const izanagi::graph::SVertexElement elems[] =
    {
        {0,  0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0},    // 座標
        {0, 16, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_COLOR,  izanagi::graph::E_GRAPH_VTX_DECL_USAGE_COLOR,    0},    // 色
        {0, 20, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT2, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD, 0},    // UV
    };

    m_VD = device->CreateVertexDeclaration(elems, COUNTOF(elems));
    IZ_ASSERT(m_VD != IZ_NULL);
}

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

const izanagi::math::SVector& CDecal::GetCenter() const
{
    return m_Rectangle.pt;
}

void CDecal::ComputeNewTriNumByScissoring(
    const izanagi::math::CTriangle tri[],
    IZ_UINT triNum)
{
#if 1
    m_TriNum = 0;

    for (IZ_UINT n = 0; n < PLANE_NUM; n++)
    {
        m_TriNum += izanagi::CSceneGraphUtil::ComputeTriNumBySissoring(
            m_Planes[n],
            tri,
            triNum);
    }
#else
    m_TriNum = 0;

    for (IZ_UINT i = 0; i < triNum; i++)
    {
        IZ_FLOAT dot = izanagi::math::SVector::Dot(
            m_Rectangle.nml,
            tri[i].nml);

        if (dot >= 0.0f)
        {
            m_TriNum++;
        }
    }
#endif
}
