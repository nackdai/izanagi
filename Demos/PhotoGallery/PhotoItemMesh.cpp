#include "PhotoItemMesh.h"
#include "Configure.h"

////////////////////////////////////////

class PhotoItemMesh::FrontFace : public izanagi::CDebugMesh {
public:
    FrontFace() {}
    virtual ~FrontFace() {}

    // Initialization.
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator, 
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flag,
        IZ_UINT width,
        IZ_UINT height);

    // Set vertices.
    IZ_BOOL SetVtx(
        IZ_UINT flag,
        IZ_COLOR color,
        IZ_UINT width,
        IZ_UINT height);

    // Compute vertices.
    void ComputeVtx(
        SMeshVtx* vtx,
        IZ_UINT flag,
        IZ_COLOR color,
        const izanagi::CFloatPoint& pt);

    // Set indices.
    IZ_BOOL SetIdx();
};

IZ_BOOL PhotoItemMesh::FrontFace::Init(
    izanagi::IMemoryAllocator* allocator, 
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_UINT width,
    IZ_UINT height)
{
    // Vertices count.
    //  Front face = 4 [vertices]
    IZ_UINT vtxNum = 4;

    // Indices count.
    //  Front face = 2 [triangles] = 2 * 3 [indices]
    IZ_UINT idxNum = 2 * 3;

    // Index format.
    izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT idxFmt = izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX16;

    VRETURN(
        CreateVB(
            device,
            flag, 
            vtxNum));

    VRETURN(
        CreateIB(
            device,
            idxNum, 
            idxFmt));

    VRETURN(CreateVD(device, flag));

    // TODO
    m_PrimType = izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST;
    m_nPrimCnt = idxNum / 3;

    VRETURN(
        CreateDataBuffer(
            vtxNum,
            idxNum));

    VRETURN(
        SetVtx(
            flag,
            Color,
            width, height));

    VRETURN(SetIdx());

    // Copy vertices and indices to buffer.
    VRETURN(CopyDataToBuffer(flag));

    return IZ_TRUE;
}

IZ_BOOL PhotoItemMesh::FrontFace::SetVtx(
    IZ_UINT flag,
    IZ_COLOR color,
    IZ_UINT width,
    IZ_UINT height)
{
    SMeshVtx* vtx = GetVtx();

    // NOTE
    // 1------2
    // |      |
    // |      |
    // 0------3

    const IZ_FLOAT halfW = width * 0.5f;

    ComputeVtx(
        vtx,
        flag,
        color,
        izanagi::CFloatPoint(-halfW, 0.0f));
    vtx++;

    ComputeVtx(
        vtx,
        flag,
        color,
        izanagi::CFloatPoint(-halfW, (IZ_FLOAT)height));
    vtx++;

    ComputeVtx(
        vtx,
        flag,
        color,
        izanagi::CFloatPoint(halfW, (IZ_FLOAT)height));
    vtx++;

    ComputeVtx(
        vtx,
        flag,
        color,
        izanagi::CFloatPoint(halfW, 0.0f));
    vtx++;

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetVtx(), vtx) == GetVtxNum());

    return IZ_TRUE;
}

void PhotoItemMesh::FrontFace::ComputeVtx(
    izanagi::CDebugMesh::SMeshVtx* vtx,
    IZ_UINT flag,
    IZ_COLOR color,
    const izanagi::CFloatPoint& pt)
{
    if (IsPos(flag)) {
        vtx->pos.v[0] = pt.x;
        vtx->pos.v[1] = pt.y;
        vtx->pos.v[2] = 0.0f;
        vtx->pos.v[3] = 1.0f;
    }

    if (IsNormal(flag)) {
        vtx->nml.v[0] = 0.0f;
        vtx->nml.v[1] = 0.0f;
        vtx->nml.v[2] = 1.0f;
        vtx->nml.v[3] = 0.0f;
    }

    // 頂点カラー
    if (IsColor(flag)) {
        vtx->clr = color;
    }

    // UV座標
    if (IsUV(flag)) {
        //vtx->
    }
}

IZ_BOOL PhotoItemMesh::FrontFace::SetIdx()
{
    return IZ_TRUE;
}

////////////////////////////////////////

class PhotoItemMesh::SideFaces : public izanagi::CDebugMesh {
public:
    SideFaces() {}
    virtual ~SideFaces() {}

    // Initialization.
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator, 
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT width,
        IZ_UINT height);

    // Set vertices.
    IZ_BOOL SetVtx(
        IZ_UINT flag,
        IZ_COLOR color,
        IZ_UINT width,
        IZ_UINT height);

    // Compute vertices.
    void ComputeVtx(
        SMeshVtx* vtx,
        IZ_UINT flag,
        IZ_COLOR color,
        IZ_UINT width,
        IZ_UINT height);

    // Set indices.
    IZ_BOOL SetIdx();
};

////////////////////////////////////////

PhotoItemMesh* PhotoItemMesh::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT width,
    IZ_UINT height)
{
    void* buf = ALLOC(allocator, sizeof(PhotoItemMesh));
    VRETURN_NULL(buf != IZ_NULL);

    IZ_BOOL result = IZ_TRUE;

    PhotoItemMesh* instance = new (buf) PhotoItemMesh;
    {
        instance->AddRef();
        instance->m_Allocator = allocator;
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }

    return instance;
}

PhotoItemMesh::PhotoItemMesh()
{
}

PhotoItemMesh::~PhotoItemMesh()
{
}