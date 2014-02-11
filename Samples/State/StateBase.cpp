#include "StateBase.h"
#include "izGraph.h"
#include "izSystem.h"

CStateBase::CStateBase()
{
    m_Img = IZ_NULL;
    m_IsBack = IZ_FALSE;
}

CStateBase::~CStateBase()
{
    Destroy();
}

// 初期化.
IZ_BOOL CStateBase::Init()
{
    // Nothing is done...
    return IZ_TRUE;
}

// 描画.
IZ_BOOL CStateBase::Render(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        // スプライト
        device->SetTexture(0, m_Img->GetTexture(GetIndex()));
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(300, 100, 256, 128));

        device->End2D();
    }

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStateBase::Enter(
    izanagi::IMemoryAllocator* allocator,
    void* val)
{
    izanagi::CFileInputStream in;
    VRETURN(in.Open("data/StateImage.img"));

    izanagi::graph::CGraphicsDevice* device = reinterpret_cast<izanagi::graph::CGraphicsDevice*>(val);

    m_Img = izanagi::CImage::CreateImage(
                allocator,
                device,
                &in);
    VRETURN(m_Img != IZ_NULL);

    return IZ_TRUE;
}

// 終了.
IZ_BOOL CStateBase::Destroy()
{
    return Leave();
}

// ステートから抜ける（終了）.
IZ_BOOL CStateBase::Leave()
{
    SAFE_RELEASE(m_Img);

    return IZ_TRUE;
}
