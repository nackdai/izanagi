#include "izSampleKit.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class WaterDropApp : public izanagi::sample::CSampleApp {
public:
    WaterDropApp();
    virtual ~WaterDropApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

private:
    izanagi::CImage* m_Img;

    izanagi::graph::CVertexShader* m_VS;
    izanagi::graph::CPixelShader* m_PS;

    SHADER_PARAM_HANDLE m_ParamHandle;
};

WaterDropApp::WaterDropApp()
{
    m_Img = IZ_NULL;

    m_VS = IZ_NULL;
    m_PS = IZ_NULL;
}

WaterDropApp::~WaterDropApp()
{
}

static izanagi::SIntPoint points[20] = {0};

// 初期化.
IZ_BOOL WaterDropApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/WaterDrop.img"));

        m_Img = izanagi::CImage::CreateImage(
                    allocator,
                    device,
                    &in);

        VRETURN(m_Img != IZ_NULL);
    }

    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/WaterDropVS.o"));

        size_t size = (size_t)in.GetSize();
        void* buf = ALLOC(allocator, size);
        in.Read(buf, 0, size);

        m_VS = device->CreateVertexShader(buf);

        FREE(allocator, buf);
        VRETURN(m_VS != IZ_NULL);
    }

    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("data/WaterDropPS.o"));

        size_t size = (size_t)in.GetSize();
        void* buf = ALLOC(allocator, size);
        in.Read(buf, 0, size);

        m_PS = device->CreatePixelShader(buf);

        FREE(allocator, buf);
        VRETURN(m_PS != IZ_NULL);
    }

    device->SetUserDefs2DShader(m_VS, m_PS);

    {
        izanagi::math::CMathRand::Init((IZ_UINT)GetTimer(0).GetCurTime());

        for (IZ_UINT i = 0; i < COUNTOF(points); i++) {
            points[i].x = izanagi::math::CMathRand::GetRandBetween<IZ_UINT>(0, SCREEN_WIDTH);
            points[i].y = izanagi::math::CMathRand::GetRandBetween<IZ_UINT>(0, SCREEN_HEIGHT);
        }
    }

    return IZ_TRUE;
}

// 解放.
void WaterDropApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_VS);
    SAFE_RELEASE(m_PS);
}

// 更新.
void WaterDropApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    // Nothing is done...
}

// 描画.
void WaterDropApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        // スプライト
        m_Img->GetTexture(0)->SetAddress(
            izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP,
            izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP);

        device->SetTexture(0, m_Img->GetTexture(0));
        device->SetTexture(1, m_Img->GetTexture(1));

        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_USER_DEFS);

        if (!IS_VALID_SHADER_PARAM_HANDLE(m_ParamHandle)) {
            m_ParamHandle = device->Get2DShaderProgramIfRenderOpIsUserDefs()->GetHandleByName("g_vInvScreen");
        }
        device->Get2DShaderProgramIfRenderOpIsUserDefs()->SetVector(
            device,
            m_ParamHandle,
            izanagi::math::CVector(1.0f / SCREEN_WIDTH, 1.0f / SCREEN_HEIGHT, 0.0f, 0.0f));

        for (IZ_UINT i = 0; i < COUNTOF(points); i++) {
            device->Draw2DSprite(
                izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                izanagi::CIntRect(points[i].x, points[i].y, 64, 64));
        }

        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

WaterDropApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "WaterDrop",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
