#include "ItemBox.h"
#include "Configure.h"

namespace {

    // NOTE
    // 0---3 7---4    |+
    // |   | |   |  --+-->x
    // 1---2 6---5    |-
    //                y
    //      |-
    //      |
    //  7---+---4
    //      |
    // -----y------>x
    //      |
    //  0---+---3
    //      |
    //      |+
    //      z
    //

    // 位置
    const IZ_FLOAT POS[][3] = {
        // 前
        {-0.5f,  0.5f, -0.5f},  // 0
        {-0.5f, -0.5f, -0.5f},  // 1
        { 0.5f, -0.5f, -0.5f},  // 2
        { 0.5f,  0.5f, -0.5f},  // 3

        // 後
        { 0.5f,  0.5f,  0.5f},  // 4
        { 0.5f, -0.5f,  0.5f},  // 5
        {-0.5f, -0.5f,  0.5f},  // 6
        {-0.5f,  0.5f,  0.5f},  // 7

        // 上
        {-0.5f,  0.5f,  0.5f},  // 7
        {-0.5f,  0.5f, -0.5f},  // 0
        { 0.5f,  0.5f, -0.5f},  // 3
        { 0.5f,  0.5f,  0.5f},  // 4

        // 下
        {-0.5f, -0.5f, -0.5f},  // 1
        {-0.5f, -0.5f,  0.5f},  // 6
        { 0.5f, -0.5f,  0.5f},  // 5
        { 0.5f, -0.5f, -0.5f},  // 2

        // 右
        { 0.5f,  0.5f, -0.5f},  // 3
        { 0.5f, -0.5f, -0.5f},  // 2
        { 0.5f, -0.5f,  0.5f},  // 5
        { 0.5f,  0.5f,  0.5f},  // 4

        // 左
        {-0.5f,  0.5f,  0.5f},  // 7
        {-0.5f, -0.5f,  0.5f},  // 6
        {-0.5f, -0.5f, -0.5f},  // 1
        {-0.5f,  0.5f, -0.5f},  // 0
    };

    // 法線
    const IZ_FLOAT NML[][3] = {
        { 0.0f,  0.0f, -1.0f},  // 前
        { 0.0f,  0.0f,  1.0f},  // 後
        { 0.0f,  1.0f,  0.0f},  // 上
        { 0.0f, -1.0f,  0.0f},  // 下
        { 1.0f,  0.0f,  0.0f},  // 右
        {-1.0f,  0.0f,  0.0f},  // 左
    };

    const IZ_FLOAT NEXT_NML[][4][3] = {
        // 前
        {
            { -1.0f,  0.0f,  0.0f}, // 左
            {  1.0f,  0.0f,  0.0f}, // 右

            {  0.0f,  1.0f,  0.0f}, // 上
            {  0.0f, -1.0f,  0.0f}, // 下
        },
        // 後
        {
            {  1.0f,  0.0f,  0.0f},
            { -1.0f,  0.0f,  0.0f},

            {  0.0f,  1.0f,  0.0f}, // 上
            {  0.0f, -1.0f,  0.0f}, // 下
        },
        // 上
        {
            { -1.0f,  0.0f,  0.0f},
            {  1.0f,  0.0f,  0.0f},

            {  0.0f,  0.0f,  1.0f}, // 奥
            {  0.0f,  0.0f, -1.0f}, // 手前
        },
        // 下
        {
            { -1.0f,  0.0f,  0.0f},
            {  1.0f,  0.0f,  0.0f},

            {  0.0f,  0.0f, -1.0f}, // 手前
            {  0.0f,  0.0f,  1.0f}, // 奥
        },
        // 右
        {
            {  0.0f,  0.0f, -1.0f},
            {  0.0f,  0.0f,  1.0f},

            {  0.0f,  1.0f,  0.0f}, // 上
            {  0.0f, -1.0f,  0.0f}, // 下
        },
        // 左
        {
            {  0.0f,  0.0f,  1.0f},
            {  0.0f,  0.0f, -1.0f},

            {  0.0f,  1.0f,  0.0f}, // 上
            {  0.0f, -1.0f,  0.0f}, // 下
        },
    };

    const IZ_FLOAT DIR[][4][3] = {
        // 前
        {
            {  1.0f,  0.0f,  0.0f},
            { -1.0f,  0.0f,  0.0f},

            {  0.0f, -1.0f,  0.0f},
            {  0.0f,  1.0f,  0.0f}, 
        },
        // 後
        {
            { -1.0f, 0.0f, 0.0f},
            {  1.0f, 0.0f, 0.0f},

            {  0.0f, -1.0f,  0.0f}, // 上
            {  0.0f,  1.0f,  0.0f}, // 下
        },
        // 上
        {
            {  1.0f,  0.0f,  0.0f},
            { -1.0f,  0.0f,  0.0f},

            {  0.0f,  0.0f, -1.0f},
            {  0.0f,  0.0f,  1.0f},
        },
        // 下
        {
            {  1.0f, 0.0f, 0.0f},
            { -1.0f, 0.0f, 0.0f},

            {  0.0f,  0.0f,  1.0f}, // 手前
            {  0.0f,  0.0f, -1.0f}, // 奥
        },
        // 右
        {
            {  0.0f, 0.0f,  1.0f},
            {  0.0f, 0.0f, -1.0f},

            {  0.0f, -1.0f,  0.0f}, // 上
            {  0.0f,  1.0f,  0.0f}, // 下
        },
        // 左
        {
            {  0.0f, 0.0f, -1.0f},
            {  0.0f, 0.0f,  1.0f},

            {  0.0f, -1.0f,  0.0f}, // 上
            {  0.0f,  1.0f,  0.0f}, // 下
        },
    };

    // UV
    const IZ_FLOAT UV[][2] = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
    };

    enum {
        PRIM_NUM = 6,
        VTX_NUM_PER_FACE = 4,
        IDX_NUM_PER_FACE = 6,
    };

    // 面
    const IZ_UINT16 PRIM[PRIM_NUM][IDX_NUM_PER_FACE] = {
#if 0
        {0, 1, 2, 3},   // 前
        {3, 2, 5, 4},   // 右
        {4, 5, 6, 7},   // 後
        {7, 6, 1, 0},   // 左
        {7, 0, 3, 4},   // 上
        {1, 6, 5, 2},   // 下
#else
        {0, 1, 2, 3},       // 前
        {4, 5, 6, 7},       // 後
        {8, 9, 10, 11},     // 上
        {12, 13, 14, 15},   // 下
        {16, 17, 18, 19},   // 右
        {20, 21, 22, 23},   // 左
#endif
    };
}   // namespace

// インスタンス作成
ItemBox* ItemBox::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_COLOR color,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    void* pBuf = ALLOC_ZERO(allocator, sizeof(ItemBox));
    VRETURN_VAL(pBuf != IZ_NULL, IZ_NULL);

    ItemBox* instance = IZ_NULL;
    IZ_BOOL result = IZ_FALSE;

    // インスタンス作成
    instance = CreateMesh<ItemBox>(pBuf, allocator, device, flag);
    VGOTO(result != (instance != IZ_NULL), __EXIT__);

    {
        instance->m_PrimType = izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST;
        instance->m_nPrimCnt = PRIM_NUM * 2;
    }

    result = instance->Init(allocator, device, flag);
    VGOTO(result, __EXIT__);

    // 頂点データセット
    result = instance->SetData(
                device,
                flag,
                color,
                width,
                height,
                depth);
    VGOTO(result, __EXIT__);

    // インデックスデータセット
    result = instance->SetIdx();
    VGOTO(result, __EXIT__);

    // データをVB、IBにコピーする
    result = instance->CopyDataToBuffer(flag);
    VGOTO(result, __EXIT__);

__EXIT__:
    if (!result) {
        SAFE_RELEASE(instance);
    }
    return instance;
}

// 初期化
IZ_BOOL ItemBox::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag)
{
    IZ_UINT nVtxNum = PRIM_NUM * VTX_NUM_PER_FACE;
    IZ_UINT nIdxNum = PRIM_NUM * IDX_NUM_PER_FACE;

    VRETURN(
        CreateVB(
            device,
            flag, 
            nVtxNum));

    VRETURN(
        CreateIB(
            device,
            nIdxNum, 
            izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32));

    VRETURN(CreateVD(device, flag));

    m_PrimType = izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST;
    m_nPrimCnt = nIdxNum / 3;

    VRETURN(
        CreateDataBuffer(
            nVtxNum,
            nIdxNum));

    return IZ_TRUE;
}

IZ_BOOL ItemBox::CreateVB(
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_UINT vtxNum)
{
    IZ_UINT stride = ComputeVtxStride(flag);

    stride += sizeof(IZ_FLOAT) * 4;
    stride += sizeof(IZ_FLOAT) * 3;
    stride += sizeof(IZ_FLOAT) * 4;
    stride += sizeof(IZ_FLOAT) * 3;

    m_pVB = device->CreateVertexBuffer(
                stride,
                vtxNum,
                izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);

    return (m_pVB != IZ_NULL);
}

IZ_BOOL ItemBox::CreateVD(
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag)
{
    // +1はENDの分
    izanagi::graph::SVertexElement elements[10];  // TODO
    FILL_ZERO(elements, sizeof(elements));

    IZ_UINT cnt = SetVtxElement(elements, flag);

    {
        elements[cnt].Stream = 0;
        elements[cnt].Offset = elements[cnt - 1].Offset
            + izanagi::graph::GetSizeByVtxDeclType(elements[cnt - 1].Type);
        elements[cnt].Type = izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
        elements[cnt].Usage = izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
        elements[cnt].UsageIndex = 1;
    }
    cnt++;

    {
        elements[cnt].Stream = 0;
        elements[cnt].Offset = elements[cnt - 1].Offset
            + izanagi::graph::GetSizeByVtxDeclType(elements[cnt - 1].Type);
        elements[cnt].Type = izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3;
        elements[cnt].Usage = izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
        elements[cnt].UsageIndex = 2;
    }
    cnt++;

    {
        elements[cnt].Stream = 0;
        elements[cnt].Offset = elements[cnt - 1].Offset
            + izanagi::graph::GetSizeByVtxDeclType(elements[cnt - 1].Type);
        elements[cnt].Type = izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
        elements[cnt].Usage = izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
        elements[cnt].UsageIndex = 3;
    }
    cnt++;

    {
        elements[cnt].Stream = 0;
        elements[cnt].Offset = elements[cnt - 1].Offset
            + izanagi::graph::GetSizeByVtxDeclType(elements[cnt - 1].Type);
        elements[cnt].Type = izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3;
        elements[cnt].Usage = izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
        elements[cnt].UsageIndex = 4;
    }
    cnt++;

    m_pVD = device->CreateVertexDeclaration(elements, cnt);

    return (m_pVD != IZ_NULL);
}

IZ_UINT8* ItemBox::SetExtraVtxData(
    const void* vtx,
    IZ_UINT flag,
    IZ_UINT8* data)
{
    const SMeshVtx* meshVtx = (const SMeshVtx*)vtx;

    IZ_FLOAT* extra = (IZ_FLOAT*)data;

    extra[0] = meshVtx->nextNml[0];
    extra[1] = meshVtx->nextNml[1];
    extra[2] = meshVtx->nextNml[2];
    extra[3] = meshVtx->nextNml[3];
    extra += 4;

    extra[0] = meshVtx->dir[0];
    extra[1] = meshVtx->dir[1];
    extra[2] = meshVtx->dir[2];
    extra += 3;

    extra[0] = meshVtx->nextNml2[0];
    extra[1] = meshVtx->nextNml2[1];
    extra[2] = meshVtx->nextNml2[2];
    extra[3] = meshVtx->nextNml2[3];
    extra += 4;

    extra[0] = meshVtx->dir2[0];
    extra[1] = meshVtx->dir2[1];
    extra[2] = meshVtx->dir2[2];
    extra += 3;

    data = (IZ_UINT8*)extra;

    return data;
}

// データセット
IZ_BOOL ItemBox::SetData(
    izanagi::graph::CGraphicsDevice* pDevice,
    IZ_UINT flag,
    IZ_COLOR color,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    SMeshVtx* pVtx = GetVtx();

    for (IZ_UINT i = 0; i < PRIM_NUM; ++i) {
        for (IZ_UINT n = 0; n < VTX_NUM_PER_FACE; ++n) {
            IZ_UINT idx = PRIM[i][n];

            // 位置
            if (izanagi::CDebugMeshUtil::IsPos(flag)) {
                pVtx->pos.v[0] = width * POS[idx][0];
                pVtx->pos.v[1] = height * POS[idx][1];
                pVtx->pos.v[2] = depth * POS[idx][2];
                pVtx->pos.v[3] = 1.0f;
            }

            // 法線
            if (izanagi::CDebugMeshUtil::IsNormal(flag)) {
                pVtx->nml.v[0] = NML[i][0];
                pVtx->nml.v[1] = NML[i][1];
                pVtx->nml.v[2] = NML[i][2];
                pVtx->nml.v[3] = 0.0f;
            }

            // カラー
            if (izanagi::CDebugMeshUtil::IsColor(flag)) {
                pVtx->clr = color;
            }
            
            // UV
            if (izanagi::CDebugMeshUtil::IsUV(flag)) {
                pVtx->uv[0] = UV[n][0];
                pVtx->uv[1] = UV[n][1];
            }

            pVtx->nextNml[0] = NEXT_NML[i][n/2][0];
            pVtx->nextNml[1] = NEXT_NML[i][n/2][1];
            pVtx->nextNml[2] = NEXT_NML[i][n/2][2];
            pVtx->nextNml[3] = Configure::DefaultBevelR;

            pVtx->dir[0] = DIR[i][n/2][0];
            pVtx->dir[1] = DIR[i][n/2][1];
            pVtx->dir[2] = DIR[i][n/2][2];

            static const IZ_FLOAT BevelScale = 0.25f;

            switch (n) {
            case 0:
            case 3:
                pVtx->nextNml2[0] = NEXT_NML[i][2][0];
                pVtx->nextNml2[1] = NEXT_NML[i][2][1];
                pVtx->nextNml2[2] = NEXT_NML[i][2][2];
                pVtx->nextNml2[3] = Configure::DefaultBevelR * BevelScale;
                pVtx->dir2[0] = DIR[i][2][0];
                pVtx->dir2[1] = DIR[i][2][1];
                pVtx->dir2[2] = DIR[i][2][2];
                break;
            case 1:
            case 2:
                pVtx->nextNml2[0] = NEXT_NML[i][3][0];
                pVtx->nextNml2[1] = NEXT_NML[i][3][1];
                pVtx->nextNml2[2] = NEXT_NML[i][3][2];
                pVtx->nextNml2[3] = Configure::DefaultBevelR * BevelScale;
                pVtx->dir2[0] = DIR[i][3][0];
                pVtx->dir2[1] = DIR[i][3][1];
                pVtx->dir2[2] = DIR[i][3][2];
                break;
            default:
                IZ_ASSERT(IZ_FALSE);
                break;
            }

            ++pVtx;
        }
    }

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

    return IZ_TRUE;
}

// インデックスデータセット
IZ_BOOL ItemBox::SetIdx()
{
    izanagi::SMeshFace* pFace = GetFace();

    IZ_UINT32 nCurIdx = 0;

    for (IZ_UINT i = 0; i < PRIM_NUM; ++i) {
        {
            pFace->idx[0] = nCurIdx + 0;
            pFace->idx[1] = nCurIdx + 1;
            pFace->idx[2] = nCurIdx + 2;

            BindFaceToVtx(pFace);
            ++pFace;
        }
        {
            pFace->idx[0] = nCurIdx + 0;
            pFace->idx[1] = nCurIdx + 2;
            pFace->idx[2] = nCurIdx + 3;

            BindFaceToVtx(pFace);
            ++pFace;
        }

        nCurIdx += 4;
    }

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetFace(), pFace) == m_nPrimCnt);

    return IZ_TRUE;
}
