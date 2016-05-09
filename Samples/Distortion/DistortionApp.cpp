#include "DistortionApp.h"

DistortionApp::DistortionApp()
{
}

DistortionApp::~DistortionApp()
{
}

// 初期化.
IZ_BOOL DistortionApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/image.img"));

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);

        IZ_ASSERT(m_Img);
    }

    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/DistortionShader.shd"));

        m_shd = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
            allocator,
            device,
            &in);
        IZ_ASSERT(m_shd);
    }

    return IZ_TRUE;
}

// 解放.
void DistortionApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_shd);
}

// 更新.
void DistortionApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    // Nothing is done...
}

// 描画.
void DistortionApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    auto shd = m_shd->GetShaderProgram(0, 0);
    auto vs = shd->GetVertexShader();
    auto ps = shd->GetPixelShader();

    device->SetUserDefs2DShader(vs, ps);

    if (device->Begin2D()) {
        auto tex = m_Img->GetTexture(0);
        tex->SetAddress(izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP, izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP);
        device->SetTexture(0, tex);

        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_USER_DEFS);

        auto userDefsShd = device->Get2DShaderProgramIfRenderOpIsUserDefs();

        auto paramHandle = userDefsShd->GetHandleByName("g_vInvScreen");

        userDefsShd->SetVector(
            device,
            paramHandle,
            izanagi::math::CVector4(1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT, 0.0f, 0.0f));

        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

        device->End2D();
    }
}

