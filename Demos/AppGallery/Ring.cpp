#include "Ring.h"
#include "Configure.h"

Ring::Ring()
{
}

Ring::~Ring()
{
}

namespace {
    inline IZ_UINT ComputePrimCnt(IZ_UINT slices)
    {
#if 0
        // NOTE
        // 最後の * 2 は 上と下、内と外 で二つあるので

        // 上下面
        IZ_UINT cnt = slices * 2 * 2;

        // 内外面
        cnt += slices * 2 * 2;
#else
        IZ_UINT cnt = slices * 2 * 2;
        cnt += slices * 2 * 2;
#endif
        return cnt;
    }

    inline IZ_UINT ComputeVtxNum(IZ_UINT slices)
    {
#if 0
        // 上下面
        IZ_UINT cnt = slices * 4 * 2;

        // 内外面
        cnt += slices * 4 * 2;
#else
        IZ_UINT cnt = slices * 4 * 2;
        cnt += slices * 4 * 2;
#endif
        return cnt;
    }

    inline IZ_UINT ComputeIdxNum(IZ_UINT slices)
    {
        // NOTE
        // TriListなので 面数 * 3 がインデックス数
        IZ_UINT cnt = ComputePrimCnt(slices);
        cnt *= 3;
        return cnt;
    }
}   // namespace

// インスタンス作成
Ring* Ring::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    void* pBuf = ALLOC_ZERO(allocator, sizeof(Ring));
    VRETURN_VAL(pBuf != IZ_NULL, IZ_NULL);

    Ring* instance = IZ_NULL;
    IZ_BOOL result = IZ_FALSE;

    IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
        | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL;

    // インスタンス作成
    instance = CreateMesh<Ring>(pBuf, allocator, device, flag);
    VGOTO(result != (instance != IZ_NULL), __EXIT__);

    result = instance->Init(allocator, device, flag);
    VGOTO(result, __EXIT__);

    // 頂点データセット
    result = instance->SetData(
                device,
                flag,
                Configure::RING_SLICES,
                Configure::RING_COLOR,
                Configure::INNER_RADIUS,
                Configure::OUTER_RADIUS,
                Configure::RING_HEIGHT);
    VGOTO(result, __EXIT__);

    // インデックスデータセット
    result = instance->SetIdx(Configure::RING_SLICES);
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
IZ_BOOL Ring::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag)
{
    IZ_UINT nVtxNum = ComputeVtxNum(Configure::RING_SLICES);
    IZ_UINT nIdxNum = ComputeIdxNum(Configure::RING_SLICES);

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

IZ_BOOL Ring::CreateVB(
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_UINT vtxNum)
{
    IZ_UINT stride = ComputeVtxStride(flag);

    stride += sizeof(IZ_FLOAT) * 4;
    stride += sizeof(IZ_FLOAT) * 3;
    stride += sizeof(IZ_FLOAT) * 3;
    stride += sizeof(IZ_FLOAT) * 3;

    m_pVB = device->CreateVertexBuffer(
                stride,
                vtxNum,
                izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);

    return (m_pVB != IZ_NULL);
}

IZ_BOOL Ring::CreateVD(
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag)
{
    // +1はENDの分
    izanagi::graph::SVertexElement elements[10];  // TODO
    FILL_ZERO(elements, sizeof(elements));

    IZ_UINT cnt = SetVtxElement(elements, flag);

    // For NextNormal
    {
        elements[cnt].Stream = 0;
        elements[cnt].Offset = elements[cnt - 1].Offset
            + izanagi::graph::GetSizeByVtxDeclType(elements[cnt - 1].Type);
        elements[cnt].Type = izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4;
        elements[cnt].Usage = izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
        elements[cnt].UsageIndex = 1;
    }
    cnt++;

    // For Direction to edge
    {
        elements[cnt].Stream = 0;
        elements[cnt].Offset = elements[cnt - 1].Offset
            + izanagi::graph::GetSizeByVtxDeclType(elements[cnt - 1].Type);
        elements[cnt].Type = izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3;
        elements[cnt].Usage = izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
        elements[cnt].UsageIndex = 2;
    }
    cnt++;

    // For NextNormal 2
    {
        elements[cnt].Stream = 0;
        elements[cnt].Offset = elements[cnt - 1].Offset
            + izanagi::graph::GetSizeByVtxDeclType(elements[cnt - 1].Type);
        elements[cnt].Type = izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3;
        elements[cnt].Usage = izanagi::graph::E_GRAPH_VTX_DECL_USAGE_TEXCOORD;
        elements[cnt].UsageIndex = 3;
    }
    cnt++;

    // For Direction to edge 2
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

IZ_UINT8* Ring::SetExtraVtxData(
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
    extra += 4;

    extra[0] = meshVtx->dir[0];
    extra[1] = meshVtx->dir[1];
    extra[2] = meshVtx->dir[2];
    extra += 3;

    extra[0] = meshVtx->nextNml2[0];
    extra[1] = meshVtx->nextNml2[1];
    extra[2] = meshVtx->nextNml2[2];
    extra += 3;

    extra[0] = meshVtx->dir2[0];
    extra[1] = meshVtx->dir2[1];
    extra[2] = meshVtx->dir2[2];
    extra += 3;

    data = (IZ_UINT8*)extra;

    return data;
}

// データセット
IZ_BOOL Ring::SetData(
    izanagi::graph::CGraphicsDevice* pDevice,
    IZ_UINT flag,
    IZ_UINT slices,
    IZ_COLOR color,
    IZ_FLOAT innerR,
    IZ_FLOAT outerR,
    IZ_FLOAT height)
{
    SMeshVtx* pVtx = GetVtx();
    
    pVtx = SetDataUpFace(
        pVtx,
        flag,
        slices,
        color,
        innerR, outerR, height);

    pVtx = SetDataDownFace(
        pVtx,
        flag,
        slices,
        color,
        innerR, outerR, height);

    pVtx = SetDataOuterFace(
        pVtx,
        flag,
        slices,
        color,
        innerR, outerR, height);

    pVtx = SetDataInnerFace(
        pVtx,
        flag,
        slices,
        color,
        innerR, outerR, height);

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetVtx(), pVtx) == GetVtxNum());

    return IZ_TRUE;
}

SMeshVtx* Ring::SetDataUpFace(
    SMeshVtx* vtx,
    IZ_UINT flag,
    IZ_UINT slices,
    IZ_COLOR color,
    IZ_FLOAT innerR,
    IZ_FLOAT outerR,
    IZ_FLOAT height)
{
    IZ_FLOAT d = IZ_MATH_PI2 / slices;

    IZ_FLOAT theta = 0.0f;

    for (IZ_UINT i = 0; i < slices; i++) {
        IZ_FLOAT c = ::cosf(theta);
        IZ_FLOAT s = ::sinf(theta);

        {
            vtx[0].pos.x = innerR * s;
            vtx[0].pos.y = height * 0.5f;
            vtx[0].pos.z = innerR * c;

            // 内向き
            vtx[0].nextNml[0] = -s;
            vtx[0].nextNml[1] = 0.0f;
            vtx[0].nextNml[2] = -c;
            vtx[0].dir[0] = s;
            vtx[0].dir[1] = 0.0f;
            vtx[0].dir[2] = c;
        }
        {
            vtx[2].pos.x = outerR * s;
            vtx[2].pos.y = height * 0.5f;
            vtx[2].pos.z = outerR * c;

            // 外向き
            vtx[2].nextNml[0] = s;
            vtx[2].nextNml[1] = 0.0f;
            vtx[2].nextNml[2] = c;
            vtx[2].dir[0] = -s;
            vtx[2].dir[1] = 0.0f;
            vtx[2].dir[2] = -c;
        }

        theta += d;
        c = ::cosf(theta);
        s = ::sinf(theta);

        {
            vtx[1].pos.x = innerR * s;
            vtx[1].pos.y = height * 0.5f;
            vtx[1].pos.z = innerR * c;

            // 内向き
            vtx[1].nextNml[0] = -s;
            vtx[1].nextNml[1] = 0.0f;
            vtx[1].nextNml[2] = -c;
            vtx[1].dir[0] = s;
            vtx[1].dir[1] = 0.0f;
            vtx[1].dir[2] = c;
        }
        {
            vtx[3].pos.x = outerR * s;
            vtx[3].pos.y = height * 0.5f;
            vtx[3].pos.z = outerR * c;

            // 外向き
            vtx[3].nextNml[0] = s;
            vtx[3].nextNml[1] = 0.0f;
            vtx[3].nextNml[2] = c;
            vtx[3].dir[0] = -s;
            vtx[3].dir[1] = 0.0f;
            vtx[3].dir[2] = -c;
        }

        for (IZ_UINT i = 0; i < 4; i++) {
            vtx[i].nml.x = 0.0f;
            vtx[i].nml.y = 1.0f;
            vtx[i].nml.z = 0.0f;

            vtx[i].nextNml2[0] = 0.0f;
            vtx[i].nextNml2[1] = 1.0f;
            vtx[i].nextNml2[2] = 0.0f;
        }

        vtx += 4;
    }

    return vtx;
}

SMeshVtx* Ring::SetDataDownFace(
    SMeshVtx* vtx,
    IZ_UINT flag,
    IZ_UINT slices,
    IZ_COLOR color,
    IZ_FLOAT innerR,
    IZ_FLOAT outerR,
    IZ_FLOAT height)
{
    IZ_FLOAT d = IZ_MATH_PI2 / slices;

    IZ_FLOAT theta = 0.0f;

    for (IZ_UINT i = 0; i < slices; i++) {
        IZ_FLOAT c = ::cosf(theta);
        IZ_FLOAT s = ::sinf(theta);

        {
            vtx[0].pos.x = innerR * s;
            vtx[0].pos.y = -height * 0.5f;
            vtx[0].pos.z = innerR * c;

            // 内向き
            vtx[0].nextNml[0] = -s;
            vtx[0].nextNml[1] = 0.0f;
            vtx[0].nextNml[2] = -c;
            vtx[0].dir[0] = s;
            vtx[0].dir[1] = 0.0f;
            vtx[0].dir[2] = c;
        }
        {
            vtx[2].pos.x = outerR * s;
            vtx[2].pos.y = -height * 0.5f;
            vtx[2].pos.z = outerR * c;

            // 外向き
            vtx[2].nextNml[0] = s;
            vtx[2].nextNml[1] = 0.0f;
            vtx[2].nextNml[2] = c;
            vtx[2].dir[0] = -s;
            vtx[2].dir[1] = 0.0f;
            vtx[2].dir[2] = -c;
        }

        theta -= d;
        c = ::cosf(theta);
        s = ::sinf(theta);

        {
            vtx[1].pos.x = innerR * s;
            vtx[1].pos.y = -height * 0.5f;
            vtx[1].pos.z = innerR * c;

            // 内向き
            vtx[1].nextNml[0] = -s;
            vtx[1].nextNml[1] = 0.0f;
            vtx[1].nextNml[2] = -c;
            vtx[1].dir[0] = s;
            vtx[1].dir[1] = 0.0f;
            vtx[1].dir[2] = c;
        }
        {
            vtx[3].pos.x = outerR * s;
            vtx[3].pos.y = -height * 0.5f;
            vtx[3].pos.z = outerR * c;

            // 外向き
            vtx[3].nextNml[0] = s;
            vtx[3].nextNml[1] = 0.0f;
            vtx[3].nextNml[2] = c;
            vtx[3].dir[0] = -s;
            vtx[3].dir[1] = 0.0f;
            vtx[3].dir[2] = -c;
        }

        for (IZ_UINT i = 0; i < 4; i++) {
            vtx[i].nml.x = 0.0f;
            vtx[i].nml.y = -1.0f;
            vtx[i].nml.z = 0.0f;

            vtx[i].nextNml2[0] = 0.0f;
            vtx[i].nextNml2[1] = -1.0f;
            vtx[i].nextNml2[2] = 0.0f;
        }

        vtx += 4;
    }

    return vtx;
}

SMeshVtx* Ring::SetDataOuterFace(
    SMeshVtx* vtx,
    IZ_UINT flag,
    IZ_UINT slices,
    IZ_COLOR color,
    IZ_FLOAT innerR,
    IZ_FLOAT outerR,
    IZ_FLOAT height)
{
        IZ_FLOAT d = IZ_MATH_PI2 / slices;

    IZ_FLOAT theta = 0.0f;

    for (IZ_UINT i = 0; i < slices; i++) {
        IZ_FLOAT c = ::cosf(theta);
        IZ_FLOAT s = ::sinf(theta);

        {
            vtx[0].pos.x = outerR * s;
            vtx[0].pos.y = height * 0.5f;
            vtx[0].pos.z = outerR * c;

            vtx[0].nml.x = s;
            vtx[0].nml.y = 0.0f;
            vtx[0].nml.z = c;

            // 上向き
            vtx[0].nextNml[0] = 0.0f;
            vtx[0].nextNml[1] = 1.0f;
            vtx[0].nextNml[2] = 0.0f;
            vtx[0].dir[0] = 0.0f;
            vtx[0].dir[1] = -1.0f;
            vtx[0].dir[2] = 0.0f;
        }
        {
            vtx[2].pos.x = outerR * s;
            vtx[2].pos.y = height * -0.5f;
            vtx[2].pos.z = outerR * c;

            vtx[2].nml.x = s;
            vtx[2].nml.y = 0.0f;
            vtx[2].nml.z = c;

            // 下向き
            vtx[2].nextNml[0] = 0.0f;
            vtx[2].nextNml[1] = -1.0f;
            vtx[2].nextNml[2] = 0.0f;
            vtx[2].dir[0] = 0.0f;
            vtx[2].dir[1] = 1.0f;
            vtx[2].dir[2] = 0.0f;
        }

        theta += d;
        c = ::cosf(theta);
        s = ::sinf(theta);

        {
            vtx[1].pos.x = outerR * s;
            vtx[1].pos.y = height * 0.5f;
            vtx[1].pos.z = outerR * c;

            vtx[1].nml.x = s;
            vtx[1].nml.y = 0.0f;
            vtx[1].nml.z = c;

            // 上向き
            vtx[1].nextNml[0] = 0.0f;
            vtx[1].nextNml[1] = 1.0f;
            vtx[1].nextNml[2] = 0.0f;
            vtx[1].dir[0] = 0.0f;
            vtx[1].dir[1] = -1.0f;
            vtx[1].dir[2] = 0.0f;
        }
        {
            vtx[3].pos.x = outerR * s;
            vtx[3].pos.y = height * -0.5f;
            vtx[3].pos.z = outerR * c;

            vtx[3].nml.x = s;
            vtx[3].nml.y = 0.0f;
            vtx[3].nml.z = c;

            // 下向き
            vtx[3].nextNml[0] = 0.0f;
            vtx[3].nextNml[1] = -1.0f;
            vtx[3].nextNml[2] = 0.0f;
            vtx[3].dir[0] = 0.0f;
            vtx[3].dir[1] = 1.0f;
            vtx[3].dir[2] = 0.0f;
        }

        for (IZ_UINT i = 0; i < 4; i++) {
            vtx[i].nextNml2[0] = vtx[i].nml.x;
            vtx[i].nextNml2[1] = vtx[i].nml.y;
            vtx[i].nextNml2[2] = vtx[i].nml.z;
        }

        vtx += 4;
    }

    return vtx;

}

SMeshVtx* Ring::SetDataInnerFace(
    SMeshVtx* vtx,
    IZ_UINT flag,
    IZ_UINT slices,
    IZ_COLOR color,
    IZ_FLOAT innerR,
    IZ_FLOAT outerR,
    IZ_FLOAT height)
{
    IZ_FLOAT d = IZ_MATH_PI2 / slices;

    IZ_FLOAT theta = 0.0f;

    for (IZ_UINT i = 0; i < slices; i++) {
        IZ_FLOAT c = ::cosf(theta);
        IZ_FLOAT s = ::sinf(theta);

        {
            vtx[0].pos.x = innerR * s;
            vtx[0].pos.y = height * 0.5f;
            vtx[0].pos.z = innerR * c;

            vtx[0].nml.x = -s;
            vtx[0].nml.y = 0.0f;
            vtx[0].nml.z = -c;

            // 上向き
            vtx[0].nextNml[0] = 0.0f;
            vtx[0].nextNml[1] = 1.0f;
            vtx[0].nextNml[2] = 0.0f;
            vtx[0].dir[0] = 0.0f;
            vtx[0].dir[1] = -1.0f;
            vtx[0].dir[2] = 0.0f;
        }
        {
            vtx[2].pos.x = innerR * s;
            vtx[2].pos.y = height * -0.5f;
            vtx[2].pos.z = innerR * c;

            vtx[2].nml.x = -s;
            vtx[2].nml.y = 0.0f;
            vtx[2].nml.z = -c;

            // 下向き
            vtx[2].nextNml[0] = 0.0f;
            vtx[2].nextNml[1] = -1.0f;
            vtx[2].nextNml[2] = 0.0f;
            vtx[2].dir[0] = 0.0f;
            vtx[2].dir[1] = 1.0f;
            vtx[2].dir[2] = 0.0f;
        }

        theta -= d;
        c = ::cosf(theta);
        s = ::sinf(theta);

        {
            vtx[1].pos.x = innerR * s;
            vtx[1].pos.y = height * 0.5f;
            vtx[1].pos.z = innerR * c;

            vtx[1].nml.x = -s;
            vtx[1].nml.y = 0.0f;
            vtx[1].nml.z = -c;

            // 上向き
            vtx[1].nextNml[0] = 0.0f;
            vtx[1].nextNml[1] = 1.0f;
            vtx[1].nextNml[2] = 0.0f;
            vtx[1].dir[0] = 0.0f;
            vtx[1].dir[1] = -1.0f;
            vtx[1].dir[2] = 0.0f;
        }
        {
            vtx[3].pos.x = innerR * s;
            vtx[3].pos.y = height * -0.5f;
            vtx[3].pos.z = innerR * c;

            vtx[3].nml.x = -s;
            vtx[3].nml.y = 0.0f;
            vtx[3].nml.z = -c;

            // 下向き
            vtx[3].nextNml[0] = 0.0f;
            vtx[3].nextNml[1] = -1.0f;
            vtx[3].nextNml[2] = 0.0f;
            vtx[3].dir[0] = 0.0f;
            vtx[3].dir[1] = 1.0f;
            vtx[3].dir[2] = 0.0f;
        }

        for (IZ_UINT i = 0; i < 4; i++) {
            vtx[i].nextNml2[0] = vtx[i].nml.x;
            vtx[i].nextNml2[1] = vtx[i].nml.y;
            vtx[i].nextNml2[2] = vtx[i].nml.z;
        }

        vtx += 4;
    }

    return vtx;

}

// インデックスデータセット
IZ_BOOL Ring::SetIdx(IZ_UINT slices)
{
    izanagi::SMeshFace* face = GetFace();

    IZ_UINT32 nCurIdx = 0;

    face = SetIdxUpFace(slices, face, nCurIdx);
    face = SetIdxDownFace(slices, face, nCurIdx);
    face = SetIdxOuterFace(slices, face, nCurIdx);
    face = SetIdxInnerFace(slices, face, nCurIdx);

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetFace(), face) == m_nPrimCnt);

    return IZ_TRUE;
}

izanagi::SMeshFace* Ring::SetIdxUpFace(
    IZ_UINT slices,
    izanagi::SMeshFace* face,
    IZ_UINT& idx)
{
    for (IZ_UINT i = 0; i < slices; ++i) {
        {
            face->idx[0] = idx + 0;
            face->idx[1] = idx + 1;
            face->idx[2] = idx + 2;

            BindFaceToVtx(face);
            ++face;
        }
        {
            face->idx[0] = idx + 1;
            face->idx[1] = idx + 3;
            face->idx[2] = idx + 2;

            BindFaceToVtx(face);
            ++face;
        }

        idx += 4;
    }

    return face;
}

izanagi::SMeshFace* Ring::SetIdxDownFace(
    IZ_UINT slices,
    izanagi::SMeshFace* face,
    IZ_UINT& idx)
{
    return SetIdxUpFace(slices, face, idx);
}


izanagi::SMeshFace* Ring::SetIdxOuterFace(
    IZ_UINT slices,
    izanagi::SMeshFace* face,
    IZ_UINT& idx)
{
    return SetIdxUpFace(slices, face, idx);
}

izanagi::SMeshFace* Ring::SetIdxInnerFace(
    IZ_UINT slices,
    izanagi::SMeshFace* face,
    IZ_UINT& idx)
{
    return SetIdxUpFace(slices, face, idx);
}
