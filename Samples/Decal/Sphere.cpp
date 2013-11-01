#include "Sphere.h"

CSphere* CSphere::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_FLOAT radius,
    IZ_UINT slices,
    IZ_UINT stacks)
{
    void* buf = ALLOC_ZERO(allocator, sizeof(CSphere));
    VRETURN_NULL(buf != IZ_NULL);

    IZ_BOOL result = IZ_FALSE;

    CSphere* instance = new(buf) CSphere;
    {
        instance->AddRef();
        instance->m_Allocator = allocator;

        // メッシュ作成
        result = instance->CreateMesh(
            allocator,
            device,
            radius,
            slices, stacks);
        VGOTO(result, __EXIT__);

        // 三角形作成
        result = instance->CreateTriangles(
            allocator,
            radius,
            slices, stacks);
        VGOTO(result, __EXIT__);
    }

__EXIT__:
    if (!result)
    {
        if (instance != IZ_NULL)
        {
            SAFE_RELEASE(instance);
        }
        else
        {
            FREE(allocator, buf);
        }
    }

    return instance;
}

CSphere::CSphere()
{
    m_Allocator = IZ_NULL;
    m_Mesh = IZ_NULL;

    m_TriNum = 0;
    m_Triangles = IZ_NULL;
}

CSphere::~CSphere()
{
    SAFE_RELEASE(m_Mesh);

    FREE(m_Allocator, m_Triangles);
}

// メッシュ作成
IZ_BOOL CSphere::CreateMesh(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_FLOAT radius,
    IZ_UINT slices,
    IZ_UINT stacks)
{
    static const IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_UV;

    // メッシュ作成
    m_Mesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
        allocator,
        device,
        flag,
        0xffffffff,
        radius,
        slices, stacks);

    return (m_Mesh != IZ_NULL);
}

// 三角形作成
IZ_BOOL CSphere::CreateTriangles(
    izanagi::IMemoryAllocator* allocator,
    IZ_FLOAT radius,
    IZ_UINT slices,
    IZ_UINT stacks)
{
    // 必要になる頂点の数を計算
    IZ_UINT vtxNum = (slices + 1) * (stacks + 1);

    // 必要になる三角形の数を計算
    IZ_UINT triNum = (stacks - 2) * slices * 2;
    triNum += 2 * slices;    // 上と下の分

    m_TriNum = triNum;

    // 三角形を確保
    m_Triangles = (izanagi::math::CTriangle*)ALLOC_ZERO(allocator, sizeof(izanagi::math::CTriangle) * triNum);
    VRETURN(m_Triangles != IZ_NULL);

    // 一時的な頂点格納用バッファを確保
    izanagi::math::SVector* vtx = (izanagi::math::SVector*)ALLOC_ZERO(allocator, sizeof(izanagi::math::SVector)* vtxNum);
    VRETURN(vtx != IZ_NULL);

    IZ_UINT vtxPos = 0;

    // 頂点を計算
    for (IZ_UINT i = 0; i <= stacks; i++)
    {
        IZ_FLOAT phi = IZ_MATH_PI * i / (IZ_FLOAT)stacks;
        IZ_FLOAT y = radius * ::cosf(phi);
        IZ_FLOAT r = radius * ::sinf(phi);

        for (IZ_UINT n = 0; n <= slices; n++)
        {
            IZ_FLOAT theta = IZ_MATH_PI2 * n / (IZ_FLOAT)slices;
            IZ_FLOAT x = r * ::cosf(theta);
            IZ_FLOAT z = r * ::sinf(theta);

            vtx[vtxPos].Set(x, y, z);
            vtxPos++;
        }
    }

    // 三角形を構築
    {
        IZ_UINT triPos = 0;

        // 上
        for (IZ_UINT i = 0; i < slices; i++)
        {
            IZ_UINT idx = i;
            izanagi::math::CVector p0(vtx[idx].x, vtx[idx].y, vtx[idx].z);

            idx = i + slices + 2;
            izanagi::math::CVector p1(vtx[idx].x, vtx[idx].y, vtx[idx].z);

            idx = i + slices + 1;
            izanagi::math::CVector p2(vtx[idx].x, vtx[idx].y, vtx[idx].z);

            m_Triangles[triPos].Set(p0, p1, p2);

            triPos++;
        }

        // 真ん中
        for (IZ_UINT i = 1; i < stacks - 1; i++)
        {
            for (IZ_UINT n = 0; n < slices; n++)
            {
                const IZ_UINT pos = (slices + 1) * i + n;

                // 上半分
                {
                    IZ_UINT idx = pos;
                    izanagi::math::CVector p0(vtx[idx].x, vtx[idx].y, vtx[idx].z);

                    idx = pos + 1;
                    izanagi::math::CVector p1(vtx[idx].x, vtx[idx].y, vtx[idx].z);

                    idx = pos + slices + 1;
                    izanagi::math::CVector p2(vtx[idx].x, vtx[idx].y, vtx[idx].z);

                    m_Triangles[triPos].Set(p0, p1, p2);
                }
                triPos++;

                // 下半分
                {
                    IZ_UINT idx = pos + slices + 1;
                    izanagi::math::CVector p0(vtx[idx].x, vtx[idx].y, vtx[idx].z);

                    idx = pos + 1;
                    izanagi::math::CVector p1(vtx[idx].x, vtx[idx].y, vtx[idx].z);

                    idx = pos + slices + 2;
                    izanagi::math::CVector p2(vtx[idx].x, vtx[idx].y, vtx[idx].z);

                    m_Triangles[triPos].Set(p0, p1, p2);
                }
                triPos++;
            }
        }

        // 一番下の点
        const IZ_UINT lastIdx = vtxNum - (slices + 1);

        // 下
        for (IZ_UINT i = 0; i < slices; i++)
        {
            IZ_UINT idx = lastIdx + i;
            izanagi::math::CVector p0(vtx[idx].x, vtx[idx].y, vtx[idx].z);

            idx = lastIdx - (slices + 1) + i;
            izanagi::math::CVector p1(vtx[idx].x, vtx[idx].y, vtx[idx].z);

            idx = lastIdx - (slices + 1) + i + 1;
            izanagi::math::CVector p2(vtx[idx].x, vtx[idx].y, vtx[idx].z);

            m_Triangles[triPos].Set(p0, p1, p2);

            triPos++;
        }
    }

    FREE(allocator, vtx);
    return IZ_TRUE;
}

IZ_BOOL CSphere::Draw()
{
    IZ_ASSERT(m_Mesh != IZ_NULL);
    return m_Mesh->Draw();
}

IZ_BOOL CSphere::GetCrossPoint(
    const izanagi::math::CRay& ray,
    izanagi::math::SVector& refPtr,
    izanagi::math::SVector* normal)
{
#if 0
    for (IZ_UINT i = 0; i < m_TriNum; i++)
    {
        if (m_Triangles[i].GetIntersectPoint(ray, refPtr))
        {
            if (normal != IZ_NULL)
            {
                // 法線を取得
                izanagi::math::SVector::CopyXYZ(*normal, m_Triangles[i].nml);
            }

            return IZ_TRUE;
        }
    }

    return IZ_FALSE;
#else
    IZ_FLOAT minLen = IZ_FLOAT_MAX;

    izanagi::math::CVector tmpPtr;
    IZ_BOOL isCross = IZ_FALSE;
    IZ_UINT crossTriPos = 0;

    for (IZ_UINT i = 0; i < m_TriNum; i++)
    {
        if (m_Triangles[i].GetIntersectPoint(ray, tmpPtr))
        {
            isCross = IZ_TRUE;

            IZ_FLOAT len = izanagi::math::SVector::Length2(ray.p, tmpPtr);
            if (len < minLen)
            {
                minLen = len;
                crossTriPos = i;
            }
        }
    }

    if (isCross)
    {
        m_Triangles[crossTriPos].GetIntersectPoint(ray, refPtr);

        if (normal != IZ_NULL)
        {
            // 法線を取得
            izanagi::math::SVector::CopyXYZ(
                *normal,
                m_Triangles[crossTriPos].nml);
        }
    }

    return isCross;
#endif
}
