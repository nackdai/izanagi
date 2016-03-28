#include "debugutil/mesh/DebugMeshScreenFillPlane.h"

using namespace izanagi;

// インスタンス作成
CDebugMeshScreenFillPlane* CDebugMeshScreenFillPlane::create(
    IMemoryAllocator* allocator,
    graph::CGraphicsDevice* device,
    IZ_COLOR color)
{
    void* buf = ALLOC_ZERO(allocator, sizeof(CDebugMeshScreenFillPlane));
    VRETURN_VAL(buf != IZ_NULL, IZ_NULL);

    CDebugMeshScreenFillPlane* instance = IZ_NULL;
    IZ_BOOL result = IZ_FALSE;

    // 強制
    IZ_UINT flag = (E_DEBUG_MESH_VTX_FORM_POS
        | E_DEBUG_MESH_VTX_FORM_COLOR
        | E_DEBUG_MESH_VTX_FORM_UV);

    // インスタンス作成
    instance = CreateMesh<CDebugMeshScreenFillPlane>(buf, allocator, device, flag);
    VGOTO(result != (instance != IZ_NULL), __EXIT__);

    // 初期化
    result = instance->Init(
        device,
        flag);
    VGOTO(result, __EXIT__);

    // 頂点データセット
    result = instance->SetVtx(device, flag, color);
    VGOTO(result, __EXIT__);

    result = instance->SetIdx(device);
    VGOTO(result, __EXIT__);

__EXIT__:
    if (!result) {
        SAFE_RELEASE(instance);
    }
    return instance;
}

// 初期化
IZ_BOOL CDebugMeshScreenFillPlane::Init(
    graph::CGraphicsDevice* device,
    IZ_UINT flag)
{
    static const IZ_UINT nVtxNum = 4;
    static const IZ_UINT nIdxNum = 4;

    VRETURN(CreateVB(device, flag, nVtxNum));
    VRETURN(CreateIB(device, nIdxNum, graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX16));
    VRETURN(CreateVD(device, flag));

    m_PrimType = graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP;
    m_nPrimCnt = 2;

    return IZ_TRUE;
}

// 頂点データセット
IZ_BOOL CDebugMeshScreenFillPlane::SetVtx(
    graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_COLOR color)
{
    auto width = device->GetBackBufferWidth();
    auto height = device->GetBackBufferHeight();

    struct SVtx {
        IZ_FLOAT pos[4];
        IZ_COLOR color;
        IZ_FLOAT uv[2];
    };

    SVtx* vtx = IZ_NULL;
    auto pitch = m_pVB->Lock(device, 0, 0, (void**)&vtx, IZ_FALSE);

    // 左上
    {
        vtx[0].pos[0] = 0.0f;
        vtx[0].pos[1] = 0.0f;
        vtx[0].pos[2] = 0.0f;
        vtx[0].pos[3] = 1.0f;

        vtx[0].color = color;

        vtx[0].uv[0] = 0.0f;
        vtx[0].uv[1] = 0.0f;
    }

    // 右上
    {
        vtx[1].pos[0] = width;
        vtx[1].pos[1] = 0.0f;
        vtx[1].pos[2] = 0.0f;
        vtx[1].pos[3] = 1.0f;

        vtx[1].color = color;

        vtx[1].uv[0] = 1.0f;
        vtx[1].uv[1] = 0.0f;
    }

    // 左下
    {
        vtx[2].pos[0] = 0.0f;
        vtx[2].pos[1] = height;
        vtx[2].pos[2] = 0.0f;
        vtx[2].pos[3] = 1.0f;

        vtx[2].color = color;

        vtx[2].uv[0] = 0.0f;
        vtx[2].uv[1] = 1.0f;
    }

    // 右下
    {
        vtx[3].pos[0] = width;
        vtx[3].pos[1] = height;
        vtx[3].pos[2] = 0.0f;
        vtx[3].pos[3] = 1.0f;

        vtx[3].color = color;

        vtx[3].uv[0] = 1.0f;
        vtx[3].uv[1] = 1.0f;
    }

    m_pVB->Unlock(device);

    return IZ_TRUE;
}

IZ_BOOL CDebugMeshScreenFillPlane::SetIdx(graph::CGraphicsDevice* device)
{
    IZ_UINT16* idx = IZ_NULL;
    auto pitch = m_pIB->Lock(device, 0, 0, (void**)&idx, IZ_FALSE);

    idx[0] = 0;
    idx[1] = 1;
    idx[2] = 2;
    idx[3] = 3;

    m_pIB->Unlock(device);

    return IZ_TRUE;
}
