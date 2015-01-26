#include "2D.h"

C2D C2D::instance;

C2D& C2D::Instance()
{
    return instance;
}

C2D::C2D()
{
    m_Img = IZ_NULL;
}

C2D::~C2D()
{
}

IZ_BOOL C2D::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CFileInputStream in;
    VRETURN(in.Open("data/images.img"));

    m_Img = izanagi::CImage::CreateImage(
        allocator,
        device,
        &in);
    VRETURN(m_Img != IZ_NULL);

    m_Timeline.Init(500.0f, 0.0f);
    m_Timeline.EnableLoop(IZ_FALSE);
    m_Timeline.Start();

    m_EasingInterp = izanagi::animation::CEasingInterpolator::Create(
        allocator,
        0.0f, 1.0f,
        izanagi::animation::E_ANM_TWEENER_MODE_EXPO_EASE_IN_OUT);

    return IZ_TRUE;
}

void C2D::Terminate()
{
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_EasingInterp);
}

void C2D::Update(IZ_FLOAT time)
{
    m_Timeline.Advance(time);
}

void C2D::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_FLOAT value = m_EasingInterp->GetValue(m_Timeline);

    IZ_UINT8 alpha = value * 0xff;
    IZ_COLOR clr = IZ_COLOR_RGBA(0xff, 0xff, 0xff, alpha);

    device->Begin2D();
    {
        device->SetTexture(0, m_Img->GetTexture(0));

        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(0, 720 - 312 + 100 - 100 * value, 1280, 312),
            clr);

        device->Draw2DSprite(
            izanagi::CFloatRect(1.0f, 1.0f, 0.0f, 0.0f),
            izanagi::CIntRect(0, -100 + value * 100, 1280, 312),
            clr);
    }
    device->End2D();
}
