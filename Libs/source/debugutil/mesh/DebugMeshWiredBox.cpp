#include "debugutil/mesh/DebugMeshWiredBox.h"

using namespace izanagi;

namespace {

    // NOTE
    //
    //   4---------6
    //  /|        /|
    // / |       / |
    // 0--------2  |
    // | |      |  |
    // | 5-------- 7
    // |/       | /
    // 1--------3/
    //
    //       +y +z
    //       | /
    //  -x---+---+x
    //     / |
    //    -z -y
    //

    static const IZ_UINT WIRE_NUM = 12;

    // 位置
    const IZ_FLOAT POS[][3] = {
        {-0.5f,  0.5f, -0.5f},
        {-0.5f, -0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},

        {-0.5f,  0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
    };

    const IZ_UINT16 PRIM[WIRE_NUM][2] = {
        { 0, 1 },
        { 0, 2 },
        { 1, 3 },
        { 2, 3 },

        { 0, 4 },
        { 1, 5 },
        { 2, 6 },
        { 3, 7 },

        { 4, 5 },
        { 4, 6 },
        { 5, 7 },
        { 6, 7 },
    };
}   // namespace

// インスタンス作成
CDebugMeshWiredBox* CDebugMeshWiredBox::create(
    IMemoryAllocator* allocator,
    graph::CGraphicsDevice* device,
    IZ_COLOR color,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    IZ_UINT flag = E_DEBUG_MESH_VTX_FORM_POS | E_DEBUG_MESH_VTX_FORM_COLOR;

    void* buf = ALLOC_ZERO(allocator, sizeof(CDebugMeshWiredBox));
    VRETURN_VAL(buf != IZ_NULL, IZ_NULL);

    CDebugMeshWiredBox* instance = IZ_NULL;
    IZ_BOOL result = IZ_FALSE;

    // インスタンス作成
    instance = CreateMesh<CDebugMeshWiredBox>(buf, allocator, device, flag);
    VGOTO(result != (instance != IZ_NULL), __EXIT__);

    result = instance->Init(device, flag);
    VGOTO(result, __EXIT__);

    // 頂点データセット
    result = instance->SetVtx(
                device,
                flag,
                color,
                width,
                height,
                depth);
    VGOTO(result, __EXIT__);

__EXIT__:
    if (!result) {
        SAFE_RELEASE(instance);
    }
    return instance;
}

// 初期化
IZ_BOOL CDebugMeshWiredBox::Init(
    graph::CGraphicsDevice* device,
    IZ_UINT flag)
{
    static const IZ_UINT vtxNum = WIRE_NUM * 2;

    VRETURN(
        CreateVB(
            device,
            flag, 
            vtxNum));

    VRETURN(CreateVD(device, flag));

    m_PrimType = graph::E_GRAPH_PRIM_TYPE_LINELIST;
    m_nPrimCnt = WIRE_NUM;

    return IZ_TRUE;
}

// データセット
IZ_BOOL CDebugMeshWiredBox::SetVtx(
    graph::CGraphicsDevice* device,
    IZ_UINT flag,
    IZ_COLOR color,
    IZ_FLOAT width,
    IZ_FLOAT height,
    IZ_FLOAT depth)
{
    IZ_UINT8* vtx = IZ_NULL;
    m_pVB->Lock(device, 0, 0, (void**)&vtx, IZ_FALSE);

    for (IZ_UINT i = 0; i < WIRE_NUM; i++) {
        for (IZ_UINT n = 0; n < 2; n++)
        {
            IZ_UINT idx = PRIM[i][n];

            IZ_FLOAT* pos = (IZ_FLOAT*)vtx;

            pos[0] = POS[idx][0] * width;
            pos[1] = POS[idx][1] * height;
            pos[2] = POS[idx][2] * depth;
            pos[3] = 1.0f;

            vtx += CDebugMeshUtil::GetPosSize(flag);

            IZ_COLOR* dstColor = (IZ_COLOR*)vtx;
            *dstColor = color;

            vtx += CDebugMeshUtil::GetColorSize(flag);
        }
    }

    m_pVB->Unlock(device);

    return IZ_TRUE;
}
