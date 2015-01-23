#include "GestureListener.h"
#include "Configure.h"
#include "StateManager.h"
#include "AppGalleryApp.h"

CGestureListener::CGestureListener()
{
    m_App = IZ_NULL;
}

CGestureListener::~CGestureListener()
{
}

void CGestureListener::OnTapUp(const izanagi::sys::CTouchEvent& ev)
{
}

void CGestureListener::OnFling(
    const izanagi::sys::CTouchEvent& ev,
    IZ_FLOAT velocityX, IZ_FLOAT velocityY)
{
    if (izanagi::math::CMath::Absf(velocityY) > izanagi::math::CMath::Absf(velocityX)) {
        // Ignore fling.
        return;
    }
}

void CGestureListener::OnDrag(
    const izanagi::sys::CTouchEvent& ev,
    IZ_INT moveX, IZ_INT moveY)
{
    if (izanagi::math::CMath::Absf(moveY) > izanagi::math::CMath::Absf(moveX)) {
        // Ignore drag.
        return;
    }

    // NOTE
    // 画面からの端から端のドラッグで90度回転させる
    IZ_FLOAT angle = IZ_MATH_PI1_2 * moveX / (IZ_FLOAT)Configure::SCREEN_WIDTH;
}

void CGestureListener::OnDragEnd(const izanagi::sys::CTouchEvent& ev)
{
    // If drag is end, state return to default.
    StateManager::Instance().ChangeState(State_Default);
}

void CGestureListener::Init(
    AppGalleryApp* app,
    const izanagi::graph::SViewport& vp)
{
    m_App = app;
    memcpy(&m_Viewport, &vp, sizeof(vp));
}

///////////////////////////////////

GestureDetector GestureDetector::s_Instance;

GestureDetector& GestureDetector::Instance()
{
    return s_Instance;
}

GestureDetector::GestureDetector()
{
}

GestureDetector::~GestureDetector()
{
}

void GestureDetector::Init(
    izanagi::IMemoryAllocator* allocator,
    AppGalleryApp* app,
    const izanagi::graph::SViewport& vp)
{
    m_Listener.Init(app, vp);
    m_Detector.Init(allocator, &m_Listener);
}

void GestureDetector::Update()
{
    m_Detector.Update();
}

IZ_BOOL GestureDetector::PostTouchEvent(const izanagi::sys::CTouchEvent& ev)
{
    return m_Detector.PostTouchEvent(ev);
}
