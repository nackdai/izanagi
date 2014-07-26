#include "Cube.h"

CCube* CCube::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_FLOAT size)
{
    void* buf = ALLOC_ZERO(allocator, sizeof(CCube));
    VRETURN_NULL(buf != IZ_NULL);

    IZ_BOOL result = IZ_FALSE;

    CCube* instance = new(buf) CCube;
    {
        instance->AddRef();
        instance->m_Allocator = allocator;

        // メッシュ作成
        result = instance->CreateMesh(
            allocator,
            device,
            size);
        VGOTO(result, __EXIT__);

        // 三角形作成
        result = instance->CreateTriangles(
            allocator,
            size);
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

CCube::CCube()
{
    m_Allocator = IZ_NULL;
    m_Mesh = IZ_NULL;

    m_TriNum = 0;
    m_Triangles = IZ_NULL;
}

CCube::~CCube()
{
    SAFE_RELEASE(m_Mesh);

    FREE(m_Allocator, m_Triangles);
}

// メッシュ作成
IZ_BOOL CCube::CreateMesh(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_FLOAT size)
{
    static const IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_UV;

    // メッシュ作成
    m_Mesh = izanagi::CDebugMeshBox::CreateDebugMeshBox(
        allocator,
        device,
        flag,
        0xffffffff,
        size, size, size);

    return (m_Mesh != IZ_NULL);
}

// 三角形作成
IZ_BOOL CCube::CreateTriangles(
    izanagi::IMemoryAllocator* allocator,
    IZ_FLOAT size)
{
    // 必要になる三角形の数を計算
    IZ_UINT triNum = 2 * 6;

    m_TriNum = triNum;

    // 三角形を確保
    m_Triangles = (izanagi::math::CTriangle*)ALLOC_ZERO(allocator, sizeof(izanagi::math::CTriangle) * triNum);
    VRETURN(m_Triangles != IZ_NULL);

    // 位置
    izanagi::math::SVector4 POS[] = {
        {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},

        { 0.5f,  0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
    };

    for (IZ_UINT i = 0; i < COUNTOF(POS); i++)
    {
        izanagi::math::SVector4::ScaleXYZ(POS[i], POS[i], size);
    }

    // 面
    static const IZ_UINT16 PRIM[][2][3] = {
        {{0, 2, 1}, {0, 3, 2}},
        {{3, 5, 2}, {3, 4, 5}},
        {{4, 6, 5}, {4, 7, 6}},
        {{7, 1, 6}, {7, 0, 1}},
        {{7, 3, 0}, {7, 4, 3}},
        {{1, 5, 6}, {1, 2, 5}},
    };

    IZ_UINT triPos = 0;

    // 三角形を構築
    for (IZ_UINT i = 0; i < 6; i++)
    {
        for (IZ_UINT n = 0; n < 2; n++)
        {
            m_Triangles[triPos++].Set(
                POS[PRIM[i][n][0]],
                POS[PRIM[i][n][1]],
                POS[PRIM[i][n][2]]);
        }
    }

    return IZ_TRUE;
}

IZ_BOOL CCube::Draw(izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(m_Mesh != IZ_NULL);
    return m_Mesh->Draw(device);
}

IZ_BOOL CCube::GetCrossPoint(
    const izanagi::math::CRay& ray,
    izanagi::math::SVector4& refPtr,
    izanagi::math::SVector4* normal)
{
#if 0
    for (IZ_UINT i = 0; i < m_TriNum; i++)
    {
        if (m_Triangles[i].GetCrossPoint(ray, refPtr))
        {
            if (normal != IZ_NULL)
            {
                // 法線を取得
                izanagi::math::SVector4::CopyXYZ(*normal, m_Triangles[i].nml);
            }

            return IZ_TRUE;
        }
    }

    return IZ_FALSE;
#else
    IZ_FLOAT minLen = IZ_FLOAT_MAX;

    izanagi::math::CVector4 tmpPtr;
    IZ_BOOL isCross = IZ_FALSE;
    IZ_UINT crossTriPos = 0;

    for (IZ_UINT i = 0; i < m_TriNum; i++)
    {
        if (m_Triangles[i].GetIntersectPoint(ray, tmpPtr))
        {
            isCross = IZ_TRUE;

            IZ_FLOAT len = izanagi::math::SVector4::Length2(ray.p, tmpPtr);
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
            izanagi::math::SVector4::CopyXYZ(
                *normal,
                m_Triangles[crossTriPos].nml);
        }
    }

    return isCross;
#endif
}
