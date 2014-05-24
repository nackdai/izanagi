#include "izSampleKit.h"
#include "izAnimation.h"

class CAnimationInterpApp : public izanagi::sample::CSampleApp {
public:
    CAnimationInterpApp();
    virtual ~CAnimationInterpApp();

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

    izanagi::animation::CTimeline m_Timeline;

    izanagi::CFloatPoint m_LinearPoint;
    izanagi::CFloatPoint m_SplinePoint;
    izanagi::CFloatPoint m_EasingPoint;
};

CAnimationInterpApp::CAnimationInterpApp()
{
}

CAnimationInterpApp::~CAnimationInterpApp()
{
}

// 初期化.
IZ_BOOL CAnimationInterpApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    m_Timeline.Init(1000.0f, 0.0f);
    m_Timeline.EnableLoop(IZ_TRUE);
    m_Timeline.Start();

    m_Interp = izanagi::animation::CLinearInterpolator::Create(
        allocator,
        0.0f, 200.0f);

    m_LinearInterp = izanagi::animation::CLinearInterpolator::Create(
        allocator,
        200.0f, 0.0f);

    m_SplineInterp = izanagi::animation::CSplineInterpolator::Create(
        allocator,
        400.0f, 200.0f,
        0.0f, 1.0f,
        1.0f, 0.0f);

    m_EasingInterp = izanagi::animation::CEasingInterpolator::Create(
        allocator,
        600.0f, 400.0f,
        izanagi::animation::E_ANM_TWEENER_MODE_EXPO_EASE_IN);

    m_Timer.Begin();

    return IZ_TRUE;
}

// 解放.
void CAnimationInterpApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Interp);
    SAFE_RELEASE(m_LinearInterp);
    SAFE_RELEASE(m_SplineInterp);
    SAFE_RELEASE(m_EasingInterp);
}

// 更新.
void CAnimationInterpApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    IZ_FLOAT delta = m_Timer.End();

    m_Timeline.Advance(delta);

    m_LinearPoint.x = m_Interp->GetValue(m_Timeline);
    m_LinearPoint.y = m_LinearInterp->GetValue(m_Timeline);

    m_SplinePoint.x = m_Interp->GetValue(m_Timeline);
    m_SplinePoint.y = m_SplineInterp->GetValue(m_Timeline);

    m_EasingPoint.x = m_Interp->GetValue(m_Timeline);
    m_EasingPoint.y = m_EasingInterp->GetValue(m_Timeline);

    m_Timer.Begin();
}

// 描画.
void CAnimationInterpApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        device->Draw2DRect(
            izanagi::CIntRect(m_LinearPoint.x, m_LinearPoint.y, 20, 20),
            IZ_COLOR_RGBA(0xff, 0, 0, 0xff));

        device->Draw2DRect(
            izanagi::CIntRect(m_SplinePoint.x, m_SplinePoint.y, 20, 20),
            IZ_COLOR_RGBA(0, 0xff, 0, 0xff));

        device->Draw2DRect(
            izanagi::CIntRect(m_EasingPoint.x, m_EasingPoint.y, 20, 20),
            IZ_COLOR_RGBA(0, 0, 0xff, 0xff));

        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CAnimationInterpApp app;

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
