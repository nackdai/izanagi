#include "izSampleKit.h"
#include "izAnimation.h"

class CStoryboardApp : public izanagi::sample::CSampleApp {
public:
    CStoryboardApp();
    virtual ~CStoryboardApp();

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
    izanagi::sys::CTimer m_Timer;
    izanagi::animation::CLinearInterpolator* m_Interp;

    izanagi::animation::CLinearInterpolator* m_LinearInterp;
    izanagi::animation::CSplineInterpolator* m_SplineInterp;
    izanagi::animation::CEasingInterpolator* m_EasingInterp;

    izanagi::animation::CStoryBoard* m_Storyboard;
};

CStoryboardApp::CStoryboardApp()
{
}

CStoryboardApp::~CStoryboardApp()
{
}

// 初期化.
IZ_BOOL CStoryboardApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    m_Interp = izanagi::animation::CLinearInterpolator::Create(
        allocator,
        0.0f, 600.0f,
        2000.0f);
    m_Interp->EnableLoop(IZ_TRUE);
    m_Interp->Start();

    m_LinearInterp = izanagi::animation::CLinearInterpolator::Create(
        allocator,
        600.0f, 400.0f,
        650.0f);
    m_LinearInterp->EnableLoop(IZ_TRUE);

    m_SplineInterp = izanagi::animation::CSplineInterpolator::Create(
        allocator,
        400.0f, 200.0f,
        650.0f,
        0.0f, 1.0f,
        1.0f, 0.0f);
    m_SplineInterp->EnableLoop(IZ_TRUE);

    m_EasingInterp = izanagi::animation::CEasingInterpolator::Create(
        allocator,
        200.0f, 0.0f,
        700.0f,
        izanagi::animation::E_ANM_TWEENER_MODE_EXPO_EASE_IN);
    m_EasingInterp->EnableLoop(IZ_TRUE);

    m_Storyboard = izanagi::animation::CStoryBoard::Create(allocator);
    m_Storyboard->Add(m_LinearInterp);
    m_Storyboard->Add(m_SplineInterp);
    m_Storyboard->Add(m_EasingInterp);
    m_Storyboard->Start();

    m_Timer.Begin();

    return IZ_TRUE;
}

// 解放.
void CStoryboardApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Interp);
    SAFE_RELEASE(m_LinearInterp);
    SAFE_RELEASE(m_SplineInterp);
    SAFE_RELEASE(m_EasingInterp);
    SAFE_RELEASE(m_Storyboard);
}

// 更新.
void CStoryboardApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    IZ_FLOAT delta = m_Timer.End();
    delta = 16.7f;

    m_Interp->Advance(delta);
    m_Storyboard->Advance(delta);

    m_Timer.Begin();
}

// 描画.
void CStoryboardApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        IZ_FLOAT x = m_Interp->GetValue();
        IZ_FLOAT y = m_Storyboard->GetValue();

        device->Draw2DRect(
            izanagi::CIntRect(x, y, 20, 20),
            IZ_COLOR_RGBA(0xff, 0, 0, 0xff));

        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CStoryboardApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "Animation",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
