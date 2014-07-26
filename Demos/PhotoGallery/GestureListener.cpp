#include "GestureListener.h"
#include "Configure.h"
#include "PhotoItemManager.h"
#include "StateManager.h"
#include "PhotoGalleryApp.h"

CGestureListener::CGestureListener()
{
    m_App = IZ_NULL;
}

CGestureListener::~CGestureListener()
{
}

void CGestureListener::OnTapUp(const izanagi::sys::CTouchEvent& ev)
{
    if (PhotoItemManager::Instance().IsRotateAnimating()) {
        StateManager::Instance().ChangeState(State_Default);
        return;
    }

    IZ_ASSERT(m_App != IZ_NULL);

    izanagi::math::SVector4 dir;

    // Compute a ray from camera point to tap point.
    izanagi::CSceneGraphUtil::Point2Ray(
        dir,
        m_App->GetCamera().GetParam(),
        m_Viewport,
        ev.x,
        ev.y);

    izanagi::math::CRay ray(
        m_App->GetCamera().GetParam().pos,
        dir);

    // Find a hit item by the ray.
    PhotoItem* hitItem = PhotoItemManager::Instance().HitTest(ray);

    if (hitItem != IZ_NULL) {
        StateManager::Instance().ChangeState(State_MoveToItem, izanagi::CValue(hitItem));
    }
}

void CGestureListener::OnFling(
    const izanagi::sys::CTouchEvent& ev,
    IZ_FLOAT velocityX, IZ_FLOAT velocityY)
{
    if (izanagi::math::CMath::Absf(velocityY) > izanagi::math::CMath::Absf(velocityX)) {
        // Ignore fling.
        return;
    }

    // Compute angle rate.
    IZ_FLOAT angle = Configure::MaxAngleRate * velocityX;

    PhotoItemManager::Instance().SetAngleRate(angle);

    StateManager::Instance().ChangeState(State_RotateByFling);
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

    PhotoItemManager::Instance().SetAngleForImmediateRot(-angle);

    StateManager::Instance().ChangeState(State_RotateByDrag);
}

void CGestureListener::OnDragEnd(const izanagi::sys::CTouchEvent& ev)
{
    // If drag is end, state return to default.
    StateManager::Instance().ChangeState(State_Default);
}

void CGestureListener::Init(
    PhotoGalleryApp* app,
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
    PhotoGalleryApp* app,
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
