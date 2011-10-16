#include "MyAppl.h"
#include "MySystem.h"
#include "StateAnmBlend.h"
#include "StateManager.h"

CStateAnmBlend::CStateAnmBlend()
{
	m_pAnm[0] = IZ_NULL;
	m_pAnm[1] = IZ_NULL;

	m_AnmBlender = IZ_NULL;

	m_IsBack = IZ_FALSE;
}

CStateAnmBlend::~CStateAnmBlend()
{
}

IZ_BOOL CStateAnmBlend::Create()
{
	return IZ_TRUE;
}

#define _Print CMySystem::GetInstance().GetDebugFont()->DBPrint

IZ_BOOL CStateAnmBlend::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

void CStateAnmBlend::Render3D()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	CScene* scene = CMyAppl::GetInstance().GetScene();
	CCharacter* character = CMyAppl::GetInstance().GetCharacter();

	scene->Render(
		character,
		pDevice);
}

void CStateAnmBlend::Render2D()
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

		{
			IZ_FLOAT anmTime = m_AnmBlender->GetAnimationTime();
			IZ_FLOAT time = m_Timeline.GetTime();

			IZ_FLOAT anm0 = m_pAnm[0]->GetAnimationTime();
			IZ_FLOAT anm1 = m_pAnm[1]->GetAnimationTime();

			CMySystem::GetInstance().GetDebugFont()->DBPrint(
				"%.2f / %.2f (%.2f, %.2f\n",
				time, anmTime,
				anm0,anm1);
		}

		CMySystem::GetInstance().GetDebugFont()->End();

		CMySystem::GetInstance().GetGraphicsDevice()->End2D();
	}
}

IZ_BOOL CStateAnmBlend::Update()
{
	if (m_IsBack) {
		CStateManager::GetInstance().ChangePrevState();
		m_IsBack = IZ_FALSE;
		return IZ_TRUE;
	}

	IZ_FLOAT time = m_Timeline.GetTime();

	CCharacter* charater = CMyAppl::GetInstance().GetCharacter();
	charater->Update(time, m_AnmBlender);

	IZ_FLOAT fElapsed = CMySystem::GetInstance().GetTimer(0).GetTime();
	fElapsed /= 1000.0f;

	//m_Timeline.Advance(fElapsed);
	m_Timeline.Advance(100);

	//IZ_PRINTF("time [%f]\n", time);

	return IZ_TRUE;
}

IZ_BOOL CStateAnmBlend::Destroy()
{
	return Leave();
}

IZ_BOOL CStateAnmBlend::Enter()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
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

	m_AnmBlender = izanagi::IAnimationBlender::CreateAnmBlender<izanagi::CAnmLinearBlender>(
						pAllocator);
	IZ_ASSERT(m_AnmBlender != IZ_NULL);

	m_AnmBlender->SetAnimation(
		0.5f,
		m_pAnm[0],
		m_pAnm[1]);

	m_Timeline.Init(
		m_AnmBlender->GetAnimationTime(),
		0.0f);
	m_Timeline.SetIsLoop(IZ_TRUE);
	m_Timeline.SetIsReverse(IZ_FALSE);
	m_Timeline.Start();
	m_Timeline.Reset();

	return IZ_TRUE;
}

IZ_BOOL CStateAnmBlend::Leave()
{
	SAFE_RELEASE(m_pAnm[0]);
	SAFE_RELEASE(m_pAnm[1]);

	SAFE_RELEASE(m_AnmBlender);

	return IZ_TRUE;
}

IZ_BOOL CStateAnmBlend::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	if (nChar == VK_BACK) {
		m_IsBack = IZ_TRUE;
	}

	return IZ_TRUE;
}
