#include "MyAppl.h"
#include "MySystem.h"
#include "StateAnmInterp.h"
#include "StateManager.h"

void CStateAnmInterp::CTimeOverHandler::Handle(const izanagi::CStdTimeline& timeline)
{
    IZ_ASSERT(m_State != IZ_NULL);

    if ((void*)m_State->m_CurAnm != (void*)m_State->m_AnmInterp) {
        return;
    }

    if (m_State->m_TargetAnmIdx == 1) {
        m_State->m_CurAnm = m_State->m_pAnm[0];
        m_State->m_TargetAnmIdx = 1;
    }
    else {
        m_State->m_CurAnm = m_State->m_pAnm[1];
        m_State->m_TargetAnmIdx = 0;
    }
    
    m_State->m_Timeline.Init(
        m_State->m_CurAnm->GetAnimationTime(),
        0.0f);
    m_State->m_Timeline.Reset();
    m_State->m_Timeline.Start();
}

CStateAnmInterp::CStateAnmInterp()
{
    m_pAnm[0] = IZ_NULL;
    m_pAnm[1] = IZ_NULL;

    m_AnmInterp = IZ_NULL;

    m_TargetAnmIdx = 1;

    m_IsBack = IZ_FALSE;
}

CStateAnmInterp::~CStateAnmInterp()
{
}

IZ_BOOL CStateAnmInterp::Init()
{
    return IZ_TRUE;
}

#define _Print CMySystem::GetInstance().GetDebugFont()->DBPrint

IZ_BOOL CStateAnmInterp::Render(izanagi::graph::CGraphicsDevice* device)
{
    Render3D();
    Render2D();

    return IZ_TRUE;
}

void CStateAnmInterp::Render3D()
{
    izanagi::graph::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

    CScene* scene = CMyAppl::GetInstance().GetScene();
    CCharacter* character = CMyAppl::GetInstance().GetCharacter();

    scene->Render(
        character,
        pDevice);
}

void CStateAnmInterp::Render2D()
{
    if (CMySystem::GetInstance().GetGraphicsDevice()->Begin2D()) {
        CMySystem::GetInstance().GetDebugFont()->Begin();

#if 1
        {
            IZ_FLOAT time = CMySystem::GetInstance().GetTimer(0).GetTime();
            IZ_FLOAT fps = 1000.0f / time;

            CMySystem::GetInstance().GetDebugFont()->DBPrint(
                "%.2f[ms] %.2f[fps]\n",
                time, fps);
        }
#endif

#if 1
        {
            IZ_FLOAT time = CMySystem::GetInstance().GetTimer(1).GetTime();
            IZ_FLOAT fps = 1000.0f / time;

            CMySystem::GetInstance().GetDebugFont()->DBPrint(
                "%.2f[ms] %.2f[fps]\n",
                time, fps);
        }
#endif

        CMySystem::GetInstance().GetDebugFont()->End();

        CMySystem::GetInstance().GetGraphicsDevice()->End2D();
    }
}

IZ_BOOL CStateAnmInterp::Update()
{
    if (m_IsBack) {
        CStateManager::GetInstance().ChangePrevState();
        m_IsBack = IZ_FALSE;
        return IZ_TRUE;
    }

    IZ_FLOAT time = m_Timeline.GetTime();

    CCharacter* charater = CMyAppl::GetInstance().GetCharacter();
    charater->Update(time, m_CurAnm);

    IZ_FLOAT fElapsed = CMySystem::GetInstance().GetTimer(0).GetTime();
    fElapsed /= 1000.0f;

    //m_Timeline.Advance(fElapsed);
    m_Timeline.Advance(100);

    //IZ_PRINTF("time [%f]\n", time);

    return IZ_TRUE;
}

IZ_BOOL CStateAnmInterp::Destroy()
{
    return Leave();
}

IZ_BOOL CStateAnmInterp::Enter(izanagi::IMemoryAllocator* allocator, void* val)
{
    izanagi::graph::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
    izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();
    izanagi::CFileInputStream input;

    {
        VRETURN(input.Open("data/anm/00R_5.anm"));

        m_pAnm[0] = izanagi::CAnimation::CreateAnimation(pAllocator, &input);
        IZ_ASSERT(m_pAnm != IZ_NULL);

        input.Finalize();
    }

    {
        VRETURN(input.Open("data/anm/00R_Run.anm"));

        m_pAnm[1] = izanagi::CAnimation::CreateAnimation(pAllocator, &input);
        IZ_ASSERT(m_pAnm != IZ_NULL);

        input.Finalize();
    }

    m_CurAnm = m_pAnm[0];

    m_AnmInterp = izanagi::CAnimationInterp::CreateAnimationInterp(pAllocator);
    IZ_ASSERT(m_AnmInterp != IZ_NULL);

    m_Timeline.Init(
        m_pAnm[0]->GetAnimationTime(),
        0.0f);
    m_Timeline.SetIsLoop(IZ_TRUE);
    m_Timeline.SetIsReverse(IZ_FALSE);
    m_Timeline.Start();
    m_Timeline.Reset();

    m_TimeOverHandler.Set(this);
    m_Timeline.SetTimeOverHandler(&m_TimeOverHandler);

    return IZ_TRUE;
}

IZ_BOOL CStateAnmInterp::Leave()
{
    SAFE_RELEASE(m_pAnm[0]);
    SAFE_RELEASE(m_pAnm[1]);

    SAFE_RELEASE(m_AnmInterp);

    return IZ_TRUE;
}

IZ_BOOL CStateAnmInterp::OnKeyDown(IZ_UINT nChar)
{
    static const izanagi::CAnimationInterp::E_INTERP_TYPE type = izanagi::CAnimationInterp::E_INTERP_TYPE_FROZEN;
    static const IZ_FLOAT interpTime = 1000.0f;

    if (nChar == VK_RETURN) {
        if (m_TargetAnmIdx == 1) {
            IZ_FLOAT time = m_Timeline.GetTime();

            m_AnmInterp->SetAnimation(
                interpTime,
                type,
                m_pAnm[0], time,
                m_pAnm[1], 0.0f);

            m_TargetAnmIdx = 0;
        }
        else {
            IZ_FLOAT time = m_Timeline.GetTime();

            m_AnmInterp->SetAnimation(
                interpTime,
                type,
                m_pAnm[1], time,
                m_pAnm[0], 0.0f);

            m_TargetAnmIdx = 1;
        }

        m_CurAnm = m_AnmInterp;

        m_Timeline.Init(
            m_AnmInterp->GetAnimationTime(),
            0.0f);
        m_Timeline.Reset();
        m_Timeline.Start();
    }
    else if (nChar == VK_BACK) {
        m_IsBack = IZ_TRUE;
    }

    return IZ_TRUE;
}

IZ_BOOL CStateAnmInterp::OnMouseMove(IZ_INT x, IZ_INT y)
{
    return IZ_TRUE;
}

IZ_BOOL CStateAnmInterp::OnMouseWheel(IZ_SHORT zDelta)
{
    return IZ_TRUE;
}
