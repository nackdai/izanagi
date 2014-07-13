#include "PhotoItemMesh.h"
#include "Configure.h"
#include "Utility.h"

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
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

    // Set vertices.
    IZ_BOOL SetVtx(
        IZ_UINT flag,
        IZ_COLOR color,
        IZ_FLOAT width,
        IZ_FLOAT height);

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
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    m_Allocator = allocator;

    // Vertices count.
    //  Front face = 4 [vertices]
    IZ_UINT vtxNum = 4;

    // Indices count.
    //  Front face = 2 [triangles] = 2 * 3 [indices]
    IZ_UINT idxNum = 2 * 3;

    // Index format.
    izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT idxFmt = izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32;

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
            Configure::DefaultColor,
            width, height));

    VRETURN(SetIdx());

    // Copy vertices and indices to buffer.
    VRETURN(CopyDataToBuffer(flag));

    return IZ_TRUE;
}

IZ_BOOL PhotoItemMesh::FrontFace::SetVtx(
    IZ_UINT flag,
    IZ_COLOR color,
    IZ_FLOAT width,
    IZ_FLOAT height)
{
    SMeshVtx* vtx = GetVtx();

    // NOTE
    // 1------2
    // |      |
    // |      |
    // 0------3
    //
    // +x <--> -x

    const IZ_FLOAT halfW = width * 0.5f;

    const izanagi::CFloatPoint pt[] = {
        izanagi::CFloatPoint(halfW, 0.0f),
        izanagi::CFloatPoint(halfW, height),
        izanagi::CFloatPoint(-halfW, height),
        izanagi::CFloatPoint(-halfW, 0.0f),
    };

    for (IZ_UINT i = 0; i < 4; i++) {
        ComputeVtx(
            vtx,
            flag,
            color,
            pt[i]);

        // UV
        if (IsUV(flag)) {
            switch (i) {
            case 0:
                vtx->uv[0] = 0.0f;
                vtx->uv[1] = 1.0f;
                break;
            case 1:
                vtx->uv[0] = 0.0f;
                vtx->uv[1] = 0.0f;
                break;
            case 2:
                vtx->uv[0] = 1.0f;
                vtx->uv[1] = 0.0f;
                break;
            case 3:
                vtx->uv[0] = 1.0f;
                vtx->uv[1] = 1.0f;
                break;
            }
        }

        vtx++;
    }

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
    }
}

IZ_BOOL PhotoItemMesh::FrontFace::SetIdx()
{
    const IZ_UINT vtxNum = GetVtxNum();

    SMeshFace* face = GetFace();
    VRETURN(face != IZ_NULL);

    face->idx[0] = 0;
    face->idx[1] = 2;
    face->idx[2] = 1;
    face++;

    face->idx[0] = 0;
    face->idx[1] = 3;
    face->idx[2] = 2;
    face++;

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetFace(), face) == m_nPrimCnt);

    return IZ_TRUE;
}

////////////////////////////////////////

class PhotoItemMesh::TopAndSideFaces : public izanagi::CDebugMesh {
public:
    TopAndSideFaces() {}
    virtual ~TopAndSideFaces() {}

    // Initialization.
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator, 
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flag,
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

    // Set vertices.
    IZ_BOOL SetVtx(
        IZ_UINT flag,
        IZ_COLOR color,
        IZ_FLOAT width,
        IZ_FLOAT height,
        IZ_FLOAT depth);

    // Compute vertices.
    void ComputeVtx(
        SMeshVtx* vtx,
        IZ_UINT flag,
        IZ_COLOR color,
        const izanagi::math::CVector& pt);

    // Set indices.
    IZ_BOOL SetIdx();

    enum FACE {
        TOP_FACE = 0,
        RIGHT_FACE,
        LEFT_FACE,
        FACE_NUM,
    };
};

IZ_BOOL PhotoItemMesh::TopAndSideFaces::Init(
    izanagi::IMemoryAllocator* allocator, 
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    m_Allocator = allocator;

    // Vertices count.
    //  Side faces = 3 [faces] = 3 * 4 [vertices]
    IZ_UINT vtxNum = 12;

    // Indices count.
    //  Side faces = 3 [faces] = 3 * 2 [triangles] = 3 * 2 * 3 [indices]
    IZ_UINT idxNum = 18;

    // Index format.
    izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT idxFmt = izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX32;

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
            Configure::PhotoItemSideAndTopFaceColor,
            width, height, depth));

    VRETURN(SetIdx());

    // Copy vertices and indices to buffer.
    VRETURN(CopyDataToBuffer(flag));

    return IZ_TRUE;
}

IZ_BOOL PhotoItemMesh::TopAndSideFaces::SetVtx(
    IZ_UINT flag,
    IZ_COLOR color,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    SMeshVtx* vtx = GetVtx();

    // NOTE
    // 1------2
    // |      |
    // |      |
    // 0------3
    //
    // +x <--> -x

    const IZ_FLOAT halfW = width * 0.5f;
    static const IZ_FLOAT outer = 0.5f;

    // Top face
    {
        const izanagi::math::CVector pt[] = {
            izanagi::math::CVector(halfW, height, 0.0f),
            izanagi::math::CVector(halfW + outer, height + outer, -depth),
            izanagi::math::CVector(-halfW - outer, height + outer, -depth),
            izanagi::math::CVector(-halfW, height, 0.0f),
        };

        for (IZ_UINT i = 0; i < 4; i++) {
            ComputeVtx(vtx, flag, color, pt[i]);

            if (IsNormal(flag)) {
                vtx->nml.v[0] = 0.0f;
                vtx->nml.v[1] = 1.0f;
                vtx->nml.v[2] = 0.0f;
            }

            vtx++;
        }
    }

    // Right face.
    {
        const izanagi::math::CVector pt[] = {
            izanagi::math::CVector(-halfW, 0.0f, 0.0f),
            izanagi::math::CVector(-halfW, height, 0.0f),
            izanagi::math::CVector(-halfW - outer, height + outer, -depth),
            izanagi::math::CVector(-halfW - outer, 0.0f, -depth),
        };

        for (IZ_UINT i = 0; i < 4; i++) {
            ComputeVtx(vtx, flag, color, pt[i]);

            if (IsNormal(flag)) {
                vtx->nml.v[0] = -1.0f;
                vtx->nml.v[1] = 0.00f;
                vtx->nml.v[2] = 0.0f;
            }

            vtx++;
        }
    }

    // Left face.
    {
        const izanagi::math::CVector pt[] = {
            izanagi::math::CVector(halfW, 0.0f, 0.0f),
            izanagi::math::CVector(halfW, height, 0.0f),
            izanagi::math::CVector(halfW + outer, height + outer, -depth),
            izanagi::math::CVector(halfW + outer, 0.0f, -depth),
        };

        for (IZ_UINT i = 0; i < 4; i++) {
            ComputeVtx(vtx, flag, color, pt[i]);

            if (IsNormal(flag)) {
                vtx->nml.v[0] = 1.0f;
                vtx->nml.v[1] = 0.00f;
                vtx->nml.v[2] = 0.0f;
            }

            vtx++;
        }
    }

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetVtx(), vtx) == GetVtxNum());

    return IZ_TRUE;
}

void PhotoItemMesh::TopAndSideFaces::ComputeVtx(
    izanagi::CDebugMesh::SMeshVtx* vtx,
    IZ_UINT flag,
    IZ_COLOR color,
    const izanagi::math::CVector& pt)
{
    if (IsPos(flag)) {
        vtx->pos.v[0] = pt.x;
        vtx->pos.v[1] = pt.y;
        vtx->pos.v[2] = pt.z;
        vtx->pos.v[3] = 1.0f;
    }

    // 頂点カラー
    if (IsColor(flag)) {
        vtx->clr = color;
    }
}

IZ_BOOL PhotoItemMesh::TopAndSideFaces::SetIdx()
{
    const IZ_UINT vtxNum = GetVtxNum();

    SMeshFace* face = GetFace();
    VRETURN(face != IZ_NULL);

    for (IZ_UINT i = 0; i < FACE_NUM; i++) {
        switch (i) {
        case TOP_FACE:
        case RIGHT_FACE:
            {
                face->idx[0] = i * 4 + 0;
                face->idx[1] = i * 4 + 2;
                face->idx[2] = i * 4 + 1;
                face++;

                face->idx[0] = i * 4 + 0;
                face->idx[1] = i * 4 + 3;
                face->idx[2] = i * 4 + 2;
                face++;
            }
            break;
        case LEFT_FACE:
            {
                face->idx[0] = i * 4 + 0;
                face->idx[1] = i * 4 + 1;
                face->idx[2] = i * 4 + 2;
                face++;

                face->idx[0] = i * 4 + 0;
                face->idx[1] = i * 4 + 2;
                face->idx[2] = i * 4 + 3;
                face++;
            }
            break;
        default:
            break;
        }
    }

    IZ_ASSERT(izanagi::CStdUtil::GetPtrAdvanced(GetFace(), face) == m_nPrimCnt);

    return IZ_TRUE;
}

////////////////////////////////////////

PhotoItemMesh* PhotoItemMesh::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    void* buf = ALLOC(allocator, sizeof(PhotoItemMesh));
    VRETURN_NULL(buf != IZ_NULL);

    IZ_BOOL result = IZ_TRUE;

    PhotoItemMesh* instance = new (buf) PhotoItemMesh;
    {
        instance->AddRef();
        instance->m_Allocator = allocator;
        result = instance->Init(
            allocator,
            device,
            width, height, depth);
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }

    return instance;
}

PhotoItemMesh::PhotoItemMesh()
{
    m_FrontFace = IZ_NULL;
    m_TopAndSideFaces = IZ_NULL;
}

PhotoItemMesh::~PhotoItemMesh()
{
    SAFE_RELEASE(m_FrontFace);
    SAFE_RELEASE(m_TopAndSideFaces);
}

template <typename _T>
_T* PhotoItemMesh::CreateFace(
    izanagi::IMemoryAllocator* allocator, 
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT flags,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    void* buf = ALLOC(allocator, sizeof(_T));
    VRETURN_NULL(buf != IZ_NULL);

    IZ_BOOL result = IZ_TRUE;

    _T* instance = new (buf) _T;
    {
        instance->AddRef();
        result = instance->Init(
            allocator,
            device,
            flags,
            width, height, depth);
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }

    return instance;
}

IZ_BOOL PhotoItemMesh::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    // Create front face.
    m_FrontFace = CreateFace<FrontFace>(
        allocator,
        device,
        Configure::MeshFlags | izanagi::E_DEBUG_MESH_VTX_FORM_UV,
        width, height, depth);
    VRETURN(m_FrontFace != IZ_NULL);

    // Create top and side faces.
    m_TopAndSideFaces = CreateFace<TopAndSideFaces>(
        allocator,
        device,
        Configure::MeshFlags,
        width, height, depth);
    VRETURN(m_TopAndSideFaces != IZ_NULL);

    // Set material parameter.
    m_Mtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
    m_Mtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
    m_Mtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);

    return IZ_TRUE;
}

void PhotoItemMesh::RenderFront(izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    IZ_ASSERT(m_FrontFace != IZ_NULL);
    m_FrontFace->Draw(device);
}

void PhotoItemMesh::RenderTopAndSide(izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    IZ_ASSERT(m_TopAndSideFaces != IZ_NULL);
    m_TopAndSideFaces->Draw(device);
}

void PhotoItemMesh::SetMaterialToShader(izanagi::shader::CShaderBasic* shader)
{
    IZ_ASSERT(shader != IZ_NULL);

    Utility::SetShaderParam(shader, "g_vMtrlDiffuse", &m_Mtrl.vDiffuse, sizeof(m_Mtrl.vDiffuse));
    Utility::SetShaderParam(shader, "g_vMtrlAmbient", &m_Mtrl.vAmbient, sizeof(m_Mtrl.vAmbient));
    Utility::SetShaderParam(shader, "g_vMtrlSpecular", &m_Mtrl.vSpecular, sizeof(m_Mtrl.vSpecular));
}
