#include "debugutil/mesh/DebugMeshLine.h"

using namespace izanagi;

// インスタンス作成
CDebugMeshLine* CDebugMeshLine::create(
    IMemoryAllocator* allocator,
    graph::CGraphicsDevice* device,
    IZ_COLOR color,
    const math::CVector3& p0,
    const math::CVector3& p1)
{
    IZ_UINT flag = E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR;

    void* buf = ALLOC_ZERO(allocator, sizeof(CDebugMeshLine));
    VRETURN_VAL(buf != IZ_NULL, IZ_NULL);

    CDebugMeshLine* instance = IZ_NULL;
    IZ_BOOL result = IZ_FALSE;

    // インスタンス作成
    instance = CreateMesh<CDebugMeshLine>(buf, allocator, device, flag);
    VGOTO(result != (instance != IZ_NULL), __EXIT__);

    result = instance->Init(device, flag);
    VGOTO(result, __EXIT__);

    // 頂点データセット
    result = instance->SetVtx(
        device,
        flag,
        color,
        p0, p1);
    VGOTO(result, __EXIT__);

__EXIT__:
    if (!result) {
        SAFE_RELEASE(instance);
    }
    return instance;
}

// 初期化
IZ_BOOL CDebugMeshLine::Init(
    graph::CGraphicsDevice* device,
    IZ_UINT flag)
{
    static const IZ_UINT vtxNum = 2;

    VRETURN(
        CreateVB(
            device,
            flag, 
            vtxNum));

    VRETURN(CreateVD(device, flag));

    m_PrimType = graph::E_GRAPH_PRIM_TYPE_LINELIST;
    m_nPrimCnt = 1;

    return IZ_TRUE;
}

// データセット
IZ_BOOL CDebugMeshLine::SetVtx(
    graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_COLOR color,
    const math::CVector3& p0,
    const math::CVector3& p1)
{
    IZ_UINT8* vtx = IZ_NULL;
    m_pVB->Lock(device, 0, 0, (void**)&vtx, IZ_FALSE);

    {
        IZ_FLOAT* pos = (IZ_FLOAT*)vtx;

        pos[0] = p0.x;
        pos[1] = p0.y;
        pos[2] = p0.z;
        pos[3] = 1.0f;

        vtx += CDebugMeshUtil::GetPosSize(flag);

        IZ_COLOR* dstColor = (IZ_COLOR*)vtx;
        *dstColor = color;

        vtx += CDebugMeshUtil::GetColorSize(flag);
    }

    {
        IZ_FLOAT* pos = (IZ_FLOAT*)vtx;

        pos[0] = p1.x;
        pos[1] = p1.y;
        pos[2] = p1.z;
        pos[3] = 1.0f;

        vtx += CDebugMeshUtil::GetPosSize(flag);

        IZ_COLOR* dstColor = (IZ_COLOR*)vtx;
        *dstColor = color;

        vtx += CDebugMeshUtil::GetColorSize(flag);
    }

    m_pVB->Unlock(device);

    return IZ_TRUE;
}
