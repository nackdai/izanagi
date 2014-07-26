#include "StateMoveToItem.h"
#include "PhotoItemManager.h"
#include "PhotoItem.h"
#include "Configure.h"
#include "StateManager.h"

StateMoveToItem::StateMoveToItem(izanagi::CVectorCamera& camera)
    : StateBase(camera)
{
    m_Timeline.Init(
        Configure::ChangeViewDuration,
        0.0f);
    m_Timeline.EnableLoop(IZ_FALSE);
    m_Timeline.AutoReverse(IZ_FALSE);
}

StateMoveToItem::~StateMoveToItem()
{
}

IZ_BOOL StateMoveToItem::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
    PhotoItem* hitItem = (PhotoItem*)arg.GetValueAsPtr();
    IZ_ASSERT(hitItem != IZ_NULL);

    // Keep camera matrix.
    izanagi::math::SMatrix::Copy(m_CamMtx, GetCamera().GetTransform());

    const izanagi::math::SMatrix& globalRotMtx = PhotoItemManager::Instance().GetRotationMtx();

    // Compute target matrix.
    {
        izanagi::math::SVector4 pos;
        hitItem->GetCenterPosition(pos);
        izanagi::math::SMatrix::Apply(pos, pos, globalRotMtx);

        izanagi::math::SVector4 nml;
        hitItem->GetNormal(nml);
        izanagi::math::SMatrix::ApplyXYZ(nml, nml, globalRotMtx);

        izanagi::math::CVector4 targetPos(
            pos.x + nml.x * Configure::CameraDistanceFromItem,
            pos.y + nml.y * Configure::CameraDistanceFromItem,
            pos.z + nml.z * Configure::CameraDistanceFromItem);
    
        izanagi::math::CVector4 targetAt(pos);

        izanagi::CVectorCamera tmpCam;
        tmpCam.Init(
            targetPos,
            targetAt,
            1.0f,
            500.0f,
            izanagi::math::CMath::Deg2Rad(60.0f),
            1.0f);
        tmpCam.Update();

        izanagi::math::SMatrix::Copy(m_TargetMtx, tmpCam.GetTransform());
    }

    m_State = State_Move;

    m_Timeline.Reset();
    m_Timeline.Start();

    return IZ_TRUE;
}

IZ_BOOL StateMoveToItem::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device)
{
    if (m_State == State_Move) {
        m_Timeline.Advance(time);

        IZ_FLOAT t = m_Timeline.GetNormalized();

        const izanagi::math::SMatrix& from = GetCamera().GetTransform();

        izanagi::math::SMatrix mtx;
        izanagi::math::SMatrix::Lerp(mtx, from, m_TargetMtx, t);

        GetCamera().SetTransform(mtx);

        if (t == 1.0f) {
            m_State = State_Moved;
            m_Timeline.Reset();
        }
    }
    else if (m_State == State_Return) {
        m_Timeline.Advance(time);

        IZ_FLOAT t = m_Timeline.GetNormalized();

        izanagi::math::SMatrix mtx;
        izanagi::math::SMatrix::Lerp(mtx, m_TargetMtx, m_CamMtx, t);

        GetCamera().SetTransform(mtx);

        if (t == 1.0f) {
            StateManager::Instance().ChangeState(State_Default);
        }
    }

    return IZ_TRUE;
}

IZ_BOOL StateMoveToItem::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    return IZ_TRUE;
}

IZ_BOOL StateMoveToItem::OnMouseLBtnUp(const izanagi::CIntPoint& point)
{
    if (m_State == State_Moved) {
        m_State = State_Return;
        m_Timeline.Start();
    }

    return IZ_TRUE;
}
