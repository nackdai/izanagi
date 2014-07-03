#include "izSampleKit.h"
#include "izUI.h"

class CGestureListener : public izanagi::ui::CGestureListenerImplement
{
public:
    CGestureListener() {}
    virtual ~CGestureListener() {}

public:
    virtual void OnTapUp(const izanagi::sys::CTouchEvent& ev)
    {
        IZ_PRINTF("OnTapUp [%d] [%d]\n", ev.x, ev.y);
    }

    virtual void OnShowPress()
    {
        IZ_PRINTF("OnShowPress\n");
    }

    virtual void OnLongPress()
    {
        IZ_PRINTF("OnLongPress\n");
    }

    virtual void OnDrag(
        const izanagi::sys::CTouchEvent& ev,
        IZ_INT moveX, IZ_INT moveY)
    {
        IZ_PRINTF("OnDrag\n");
    }

    virtual void OnFling(
        const izanagi::sys::CTouchEvent& ev,
        IZ_FLOAT velocityX, IZ_FLOAT velocityY)
    {
        IZ_PRINTF("OnFling [%f] [%f]\n", velocityX, velocityY);
    }

    virtual IZ_BOOL OnDown()
    {
        IZ_PRINTF("OnDown\n");
        return IZ_FALSE;
    }
};

class CGestureDetectorApp : public izanagi::sample::CSampleApp {
public:
    CGestureDetectorApp();
    virtual ~CGestureDetectorApp();

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

    virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point);
    virtual IZ_BOOL OnMouseLBtnUp(const izanagi::CIntPoint& point);
    virtual void OnMouseMove(const izanagi::CIntPoint& point);

private:
    izanagi::ui::CGestureDetector m_Detector;
    CGestureListener m_Listener;
};

CGestureDetectorApp::CGestureDetectorApp()
{
}

CGestureDetectorApp::~CGestureDetectorApp()
{
}

// 初期化.
IZ_BOOL CGestureDetectorApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    m_Detector.Init(allocator, &m_Listener);
    return IZ_TRUE;
}

// 解放.
void CGestureDetectorApp::ReleaseInternal()
{
}

// 更新.
void CGestureDetectorApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    m_Detector.Update();
}

IZ_BOOL CGestureDetectorApp::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    m_Detector.PostTouchEvent(
        izanagi::sys::CTouchEvent(
            izanagi::sys::E_SYS_TOUCH_EVENT_DOWN,
            point.x, point.y));
    return IZ_TRUE;
}

IZ_BOOL CGestureDetectorApp::OnMouseLBtnUp(const izanagi::CIntPoint& point)
{
    m_Detector.PostTouchEvent(
        izanagi::sys::CTouchEvent(
            izanagi::sys::E_SYS_TOUCH_EVENT_UP,
            point.x, point.y));
    return IZ_TRUE;
}

void CGestureDetectorApp::OnMouseMove(const izanagi::CIntPoint& point)
{
    m_Detector.PostTouchEvent(
        izanagi::sys::CTouchEvent(
            izanagi::sys::E_SYS_TOUCH_EVENT_MOVE,
            point.x, point.y));
}

// 描画.
void CGestureDetectorApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CGestureDetectorApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "GestureDetector",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
