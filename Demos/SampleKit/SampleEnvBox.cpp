#include "SampleEnvBox.h"

using namespace izanagi;
using namespace sample;

// 頂点
const CSampleEnvBox::SVertex CSampleEnvBox::sVtxList[CSampleEnvBox::VTX_NUM] = {
    // 下
    {
        {-1.0f, -1.0f, -1.0f, 1.0f},
        { 0.0f,  1.0f,  0.0f},
    },
    {
        {-1.0f, -1.0f,  1.0f, 1.0f},
        { 0.0f,  1.0f,  0.0f},
    },
    {
        { 1.0f, -1.0f, -1.0f, 1.0f},
        { 0.0f,  1.0f,  0.0f},
    },
    {
        { 1.0f, -1.0f,  1.0f, 1.0f},
        { 0.0f,  1.0f,  0.0f},
    },
    // 右
    {
        {-1.0f, -1.0f, -1.0f, 1.0f},
        { 1.0f,  0.0f,  0.0f},
    },
    {
        {-1.0f, -1.0f,  1.0f, 1.0f},
        { 1.0f,  0.0f,  0.0f},
    },
    {
        {-1.0f,  1.0f, -1.0f, 1.0f},
        { 1.0f,  0.0f,  0.0f},
    },
    {
        {-1.0f,  1.0f,  1.0f, 1.0f},
        { 1.0f,  0.0f,  0.0f},
    },
    // 左
    {
        { 1.0f, -1.0f, -1.0f, 1.0f},
        {-1.0f,  0.0f,  0.0f},
    },
    {
        { 1.0f, -1.0f,  1.0f, 1.0f},
        {-1.0f,  0.0f,  0.0f},
    },
    {
        { 1.0f,  1.0f, -1.0f, 1.0f},
        {-1.0f,  0.0f,  0.0f},
    },
    {
        { 1.0f,  1.0f,  1.0f, 1.0f},
        {-1.0f,  0.0f,  0.0f},
    },
    // 奥
    {
        { 1.0f, -1.0f, -1.0f, 1.0f},
        { 0.0f,  0.0f,  1.0f},
    },
    {
        {-1.0f, -1.0f, -1.0f, 1.0f},
        { 0.0f,  0.0f,  1.0f},
    },
    {
        { 1.0f,  1.0f, -1.0f, 1.0f},
        { 0.0f,  0.0f,  1.0f},
    },
    {
        {-1.0f,  1.0f, -1.0f, 1.0f},
        { 0.0f,  0.0f,  1.0f},
    },
};

const IZ_USHORT CSampleEnvBox::sIdxList[IDX_NUM] = 
{
    // 下
    0, 1, 2,
    1, 3, 2,

    // 右
    4, 6, 7,
    4, 7, 5,

    // 左
    8, 11, 10,
    8, 9, 11,

    // 奥
    12, 14, 15,
    12, 15, 13,
};

// 頂点宣言
static const izanagi::graph::SVertexElement VERTEX_ELEMENT[] = {
    {0, 0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION , 0},
    {0, 16, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_NORMAL , 0},
};

CSampleEnvBox* CSampleEnvBox::CreateSampleEnvBox(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(allocator != IZ_NULL);
    IZ_ASSERT(device != IZ_NULL);

    IZ_BOOL result = IZ_FALSE;

    void* buf = ALLOC_ZERO(allocator, sizeof(CSampleEnvBox));
    VRETURN_NULL(buf != IZ_NULL);

    CSampleEnvBox* instance = new(buf) CSampleEnvBox();
    {
        instance->AddRef();
        instance->m_Allocator = allocator;

        result = instance->Init(device);
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }

    return instance;
}

// コンストラクタ
CSampleEnvBox::CSampleEnvBox()
{
    m_Allocator = IZ_NULL;

    m_VB = IZ_NULL;
    m_IB = IZ_NULL;
    m_VD = IZ_NULL;
}

// デストラクタ
CSampleEnvBox::~CSampleEnvBox()
{
    SAFE_RELEASE(m_VB);
    SAFE_RELEASE(m_IB);
    SAFE_RELEASE(m_VD);
}

// 初期化
IZ_BOOL CSampleEnvBox::Init(izanagi::graph::CGraphicsDevice* device)
{
    // 頂点バッファ
    {
        m_VB = device->CreateVertexBuffer(
                    sizeof(SVertex),
                    VTX_NUM,
                    izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        VRETURN(m_VB != IZ_NULL);

        SVertex* data;
        m_VB->Lock(0, 0, (void**)&data, IZ_FALSE);
        memcpy(data, sVtxList, sizeof(sVtxList));
        m_VB->Unlock();
    }

    // インデックスバッファ
    { 
        m_IB = device->CreateIndexBuffer(
                    IDX_NUM,
                    izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX16,
                    izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        VRETURN(m_IB != IZ_NULL);

        IZ_UINT16* data;
        m_IB->Lock(0, 0, (void**)&data, IZ_FALSE);
        memcpy(data, sIdxList, sizeof(sIdxList));
        m_IB->Unlock();
    }

    // 頂点宣言
    {
        m_VD = device->CreateVertexDeclaration(
                    VERTEX_ELEMENT,
                    COUNTOF(VERTEX_ELEMENT));
        VRETURN(m_VD != IZ_NULL);
    }

    return IZ_TRUE;
}

// 描画
void CSampleEnvBox::Render(izanagi::graph::CGraphicsDevice* device)
{
    device->SetVertexBuffer(0, 0, sizeof(SVertex), m_VB);
    device->SetIndexBuffer(m_IB);
    device->SetVertexDeclaration(m_VD);

    IZ_UINT primNum = VTX_NUM / 2;

    device->DrawIndexedPrimitive(
        izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST,
        0,
        VTX_NUM, 0,
        primNum);
}
