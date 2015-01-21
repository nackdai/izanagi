#include "BevelShaderMesh.h"

namespace {

    // NOTE
    // 0---2 4---6    |+
    // |   | |   |  --+-->x
    // 1---3 5---7    |-
    //                z

    // 位置
    const IZ_FLOAT POS[][3] = {
#ifdef IZ_COORD_LEFT_HAND
        {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},

        { 0.5f,  0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
#else
        {-0.5f,  0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},

        { 0.5f,  0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f},
#endif
    };

    // 法線
    const IZ_FLOAT NML[][3] = {
#ifdef IZ_COORD_LEFT_HAND
        { 0.0f,  0.0f, -1.0f},
        { 1.0f,  0.0f,  0.0f},
        { 0.0f,  0.0f,  1.0f},
        {-1.0f,  0.0f,  0.0f},
        { 0.0f,  1.0f,  0.0f},
        { 0.0f, -1.0f,  0.0f},
#else
        { 0.0f,  0.0f,  1.0f},
        { 1.0f,  0.0f,  0.0f},
        { 0.0f,  0.0f, -1.0f},
        {-1.0f,  0.0f,  0.0f},
        { 0.0f,  1.0f,  0.0f},
        { 0.0f, -1.0f,  0.0f},
#endif
    };

    const IZ_FLOAT NML2[][3] = {
        { 0.0f,  1.0f,  0.0f},
        { 0.0f,  1.0f,  0.0f},
        { 0.0f,  1.0f,  0.0f},
        { 0.0f,  1.0f,  0.0f},
        { 0.0f,  0.0f,  1.0f},
        { 0.0f,  0.0f,  1.0f},
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
        {0, 1, 2, 3},
        {3, 2, 5, 4},
        {4, 5, 6, 7},
        {7, 6, 1, 0},
        {7, 0, 3, 4},
        {1, 6, 5, 2},
    };
}   // namespace

// インスタンス作成
BevelShaderMesh* BevelShaderMesh::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_COLOR color,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

    void* pBuf = ALLOC_ZERO(allocator, sizeof(BevelShaderMesh));
    VRETURN_VAL(pBuf != IZ_NULL, IZ_NULL);

    BevelShaderMesh* instance = IZ_NULL;
    IZ_BOOL result = IZ_FALSE;

    // インスタンス作成
    instance = CreateMesh<BevelShaderMesh>(pBuf, allocator, device, flag);
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
IZ_BOOL BevelShaderMesh::Init(
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

IZ_BOOL BevelShaderMesh::CreateVB(
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_UINT vtxNum)
{
    IZ_UINT stride = ComputeVtxStride(flag);

    stride += sizeof(IZ_FLOAT) * 4;

    m_pVB = device->CreateVertexBuffer(
                stride,
                vtxNum,
                izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);

    return (m_pVB != IZ_NULL);
}

IZ_BOOL BevelShaderMesh::CreateVD(
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag)
{
    // +1はENDの分
    izanagi::graph::SVertexElement elements[10];  // TODO
    FILL_ZERO(elements, sizeof(elements));

    IZ_UINT cnt = SetVtxElement(elements, flag);

    elements[cnt].Stream = 0;
    elements[cnt].Offset = elements[cnt - 1].Offset
        + izanagi::graph::GetSizeByVtxDeclType(elements[cnt - 1].Type);
    elements[cnt].Type = izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
    elements[cnt].Usage = izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
    elements[cnt].UsageIndex = 1;

    cnt++;

    m_pVD = device->CreateVertexDeclaration(elements, cnt);

    return (m_pVD != IZ_NULL);
}

IZ_UINT8* BevelShaderMesh::SetExtraVtxData(
    const void* vtx,
    IZ_UINT flag,
    IZ_UINT8* data)
{
    const SMeshVtx* meshVtx = (const SMeshVtx*)vtx;

    IZ_FLOAT* extra = (IZ_FLOAT*)data;

    extra[0] = meshVtx->nextNml[0];
    extra[1] = meshVtx->nextNml[1];
    extra[2] = meshVtx->nextNml[2];
    extra[3] = meshVtx->radius;

    data = (IZ_UINT8*)extra;

    return data;
}

// データセット
IZ_BOOL BevelShaderMesh::SetData(
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
            // 位置
            if (izanagi::CDebugMeshUtil::IsPos(flag)) {
                IZ_UINT idx = PRIM[i][n];

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

            pVtx->nextNml[0] = NML2[i][0];
            pVtx->nextNml[1] = NML2[i][1];
            pVtx->nextNml[2] = NML2[i][2];
            pVtx->radius = 0.1f;

            ++pVtx;
        }
    }

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

    return IZ_TRUE;
}

// インデックスデータセット
IZ_BOOL BevelShaderMesh::SetIdx()
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
