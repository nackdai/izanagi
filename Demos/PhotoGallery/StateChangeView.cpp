#include "StateChangeView.h"
#include "Configure.h"
#include "StateManager.h"

const IZ_FLOAT StateChangeView::ViewRotAngle[StateChangeView::ViewState_Num] = {

};

StateChangeView::StateChangeView(izanagi::CVectorCamera& camera)
    : StateBase(camera)
{
    izanagi::CVectorCamera tmpCam;
    
    // Low
    {
#if 0
        tmpCam.Init(
            izanagi::math::CVector(0.0f, 5.0f, -Configure::InnerRadius + Configure::CameraDistance, 1.0f),
            izanagi::math::CVector(0.0f, 5.0f, -Configure::InnerRadius, 1.0f),
            1.0f,
            500.0f,
            izanagi::math::CMath::Deg2Rad(60.0f),
            1.0f);
        tmpCam.Update();

        izanagi::math::SMatrix::Copy(m_ViewMtx[ViewState_Low], tmpCam.GetTransform());
#else
        izanagi::math::SMatrix::Copy(m_ViewMtx[ViewState_Low], camera.GetTransform());
#endif
    }

    // Mid
    {
        izanagi::math::CVector v(0.0f, 0.0f, Configure::CameraDistance, 0.0f);
        izanagi::math::SMatrix mtx;
        izanagi::math::SMatrix::GetRotByX(mtx, IZ_DEG2RAD(-30.0f));
        izanagi::math::SMatrix::Apply(v, v, mtx);

        tmpCam.Init(
            izanagi::math::CVector(0.0f, v.y, -Configure::InnerRadius + v.z, 1.0f),
            izanagi::math::CVector(0.0f, 15.0f, -Configure::InnerRadius + Configure::RadiusDiff + Configure::RadiusDiff * 0.5f, 1.0f),
            1.0f,
            500.0f,
            izanagi::math::CMath::Deg2Rad(60.0f),
            1.0f);
        tmpCam.Update();

        izanagi::math::SMatrix::Copy(m_ViewMtx[ViewState_Mid], tmpCam.GetTransform());
    }

    // High
    {
        izanagi::math::CVector v(0.0f, 0.0f, Configure::CameraDistance, 0.0f);
        izanagi::math::SMatrix mtx;
        izanagi::math::SMatrix::GetRotByX(mtx, IZ_DEG2RAD(-55.0f));
        izanagi::math::SMatrix::Apply(v, v, mtx);

        tmpCam.Init(
            izanagi::math::CVector(0.0f, v.y, -Configure::InnerRadius + v.z, 1.0f),
            izanagi::math::CVector(0.0f, 25.0f, -Configure::InnerRadius + Configure::RadiusDiff * 0.5f, 1.0f),
            1.0f,
            500.0f,
            izanagi::math::CMath::Deg2Rad(60.0f),
            1.0f);
        tmpCam.Update();

        izanagi::math::SMatrix::Copy(m_ViewMtx[ViewState_High], tmpCam.GetTransform());
    }

    m_State = ViewState_Low;
    m_IsAnimating = IZ_FALSE;

    m_Timeline.Init(
        Configure::ChangeViewDuration,
        0.0f);
    m_Timeline.EnableLoop(IZ_FALSE);
    m_Timeline.AutoReverse(IZ_FALSE);
}

StateChangeView::~StateChangeView()
{
}

StateChangeView::ViewState StateChangeView::GetNextState(IZ_BOOL isUp)
{
    IZ_INT ret = (IZ_INT)m_State + (isUp ? 1 : -1);
    ret = (ret >= ViewState_Num ? ViewState_High : ret);
    ret = (ret < 0 ? ViewState_Low : ret);

    return (ViewState)(ret);
}

IZ_BOOL StateChangeView::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
    ChangeViewMode mode = (ChangeViewMode)arg.GetValueAsInt32();

    if (mode == ChangeViewModeNone) {
        m_NextState = m_State;
    }
    else {
        m_NextState = GetNextState(mode == ChangeViewModeUp);
    }

    m_IsAnimating = IZ_FALSE;

    m_Timeline.Reset();

    return IZ_TRUE;
}

IZ_BOOL StateChangeView::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device)
{
    if (!m_IsAnimating
        && m_NextState != m_State)
    {
        m_IsAnimating = IZ_TRUE;
        m_Timeline.Start();
    }

    if (m_IsAnimating) {
        // Update animation.
        m_Timeline.Advance(time);

        IZ_FLOAT t = m_Timeline.GetNormalized();

        const izanagi::math::SMatrix& from = m_ViewMtx[m_State];
        const izanagi::math::SMatrix& to = m_ViewMtx[m_NextState];

        izanagi::math::SMatrix mtx;
        izanagi::math::SMatrix::Lerp(mtx, from, to, t);

        GetCamera().SetTransform(mtx);

        if (t == 1.0f) {
            m_IsAnimating = IZ_FALSE;
            m_State = m_NextState;
            m_Timeline.Reset();
        }
    }

    if (!m_IsAnimating && m_State == ViewState_Low) {
        StateManager::Instance().ChangeState(State_Default);
    }

    return IZ_TRUE;
}

IZ_BOOL StateChangeView::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (!m_IsAnimating) {
        switch (key) {
        case izanagi::sys::E_KEYBOARD_BUTTON_UP:
            m_NextState = GetNextState(IZ_TRUE);
            break;
        case izanagi::sys::E_KEYBOARD_BUTTON_DOWN:
            m_NextState = GetNextState(IZ_FALSE);
            break;
        default:
            break;
        }
    }

    return IZ_TRUE;
}
