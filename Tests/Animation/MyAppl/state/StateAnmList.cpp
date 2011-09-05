#include <stdafx.h>
#include "MyAppl.h"
#include "MySystem.h"
#include "StateAnmList.h"

static const char* anmTbl[] = {
	"data/anm/00R_121.anm",
	"data/anm/00R_2.anm",
	"data/anm/00R_2G.anm",
	"data/anm/00R_2K.anm",
	"data/anm/00R_2P.anm",
	"data/anm/00R_323.anm",
	"data/anm/00R_3236P.anm",
	"data/anm/00R_3K.anm",
	"data/anm/00R_4.anm",
	"data/anm/00R_44.anm",
	"data/anm/00R_5.anm",
	"data/anm/00R_5G.anm",
	"data/anm/00R_5K.anm",
	"data/anm/00R_5P+G.anm",
	"data/anm/00R_5P.anm",
	"data/anm/00R_5PP.anm",
	"data/anm/00R_6.anm",
	"data/anm/00R_66.anm",
	"data/anm/00R_Blow20.anm",
	"data/anm/00R_Blow50.anm",
	"data/anm/00R_D8P.anm",
	"data/anm/00R_Damage2Low0.anm",
	"data/anm/00R_Damage2Middle0.anm",
	"data/anm/00R_Damage5High0.anm",
	"data/anm/00R_Damage5Low0.anm",
	"data/anm/00R_Damage5Middle0.anm",
	"data/anm/00R_DamageDown0.anm",
	"data/anm/00R_Down0.anm",
	"data/anm/00R_Guard2.anm",
	"data/anm/00R_Guard5.anm",
	"data/anm/00R_Run.anm",
	"data/anm/00R_Standup2.anm",
	"data/anm/00R_Standup2K.anm",
	"data/anm/00R_Standup5.anm",
	"data/anm/00R_Standup5K.anm",
	"data/anm/00R_Thrown(00_5P+G).anm",
	"data/anm/00_121.anm",
	"data/anm/00_2.anm",
	"data/anm/00_2G.anm",
	"data/anm/00_2K.anm",
	"data/anm/00_2P.anm",
	"data/anm/00_323.anm",
	"data/anm/00_3236P.anm",
	"data/anm/00_3K.anm",
	"data/anm/00_4.anm",
	"data/anm/00_44.anm",
	"data/anm/00_5.anm",
	"data/anm/00_5G.anm",
	"data/anm/00_5K.anm",
	"data/anm/00_5P+G.anm",
	"data/anm/00_5P.anm",
	"data/anm/00_5PP.anm",
	"data/anm/00_6.anm",
	"data/anm/00_66.anm",
	"data/anm/00_Blow20.anm",
	"data/anm/00_Blow50.anm",
	"data/anm/00_D8P.anm",
	"data/anm/00_Damage2Low0.anm",
	"data/anm/00_Damage2Middle0.anm",
	"data/anm/00_Damage5High0.anm",
	"data/anm/00_Damage5Low0.anm",
	"data/anm/00_Damage5Middle0.anm",
	"data/anm/00_DamageDown0.anm",
	"data/anm/00_Down0.anm",
	"data/anm/00_Guard2.anm",
	"data/anm/00_Guard5.anm",
	"data/anm/00_Run.anm",
	"data/anm/00_Standup2.anm",
	"data/anm/00_Standup2K.anm",
	"data/anm/00_Standup5.anm",
	"data/anm/00_Standup5K.anm",
	"data/anm/00_Thrown(00_P+G).anm",
};

CStateAnmList::CStateAnmList()
{
	m_pAnm = IZ_NULL;
}

CStateAnmList::~CStateAnmList()
{
}

IZ_BOOL CStateAnmList::Create()
{
	return IZ_TRUE;
}

#define _Print CMySystem::GetInstance().GetDebugFont()->DBPrint

IZ_BOOL CStateAnmList::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

void CStateAnmList::Render3D()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	CScene* scene = CMyAppl::GetInstance().GetScene();
	CCharacter* character = CMyAppl::GetInstance().GetCharacter();

	scene->Render(
		character,
		pDevice);
}

void CStateAnmList::Render2D()
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

		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			0xffffffff,
			"[%s]\n",
			anmTbl[m_nCurAnmIdx]);

		CMySystem::GetInstance().GetDebugFont()->End();

		CMySystem::GetInstance().GetGraphicsDevice()->End2D();
	}
}

IZ_BOOL CStateAnmList::Update()
{
	IZ_FLOAT fElapsed = CMySystem::GetInstance().GetTimer(0).GetTime();
	fElapsed /= 1000.0f;

	//m_Timeline.Advance(fElapsed);
	m_Timeline.Advance(100);
	IZ_FLOAT time = m_Timeline.GetTime();

	//IZ_PRINTF("time [%f]\n", time);

	CCharacter* charater = CMyAppl::GetInstance().GetCharacter();
	charater->Update(time, m_pAnm);

	return IZ_TRUE;
}

IZ_BOOL CStateAnmList::Destroy()
{
	return Leave();
}

IZ_BOOL CStateAnmList::Enter()
{
	m_nCurAnmIdx = 0;
	ReadAnm();

	return IZ_TRUE;
}

IZ_BOOL CStateAnmList::Leave()
{
	SAFE_RELEASE(m_pAnm);

	return IZ_TRUE;
}

IZ_BOOL CStateAnmList::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	IZ_BOOL needUpdateAnm = IZ_FALSE;

	if (nChar == VK_UP) {
		m_nCurAnmIdx--;
		needUpdateAnm = IZ_TRUE;
	}
	else if (nChar == VK_DOWN) {
		m_nCurAnmIdx++;
		needUpdateAnm = IZ_TRUE;
	}

	if (needUpdateAnm) {
		ReadAnm();
	}

	return IZ_TRUE;
}

IZ_BOOL CStateAnmList::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateAnmList::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateAnmList::ReadAnm()
{
	if (m_nCurAnmIdx < 0) {
		m_nCurAnmIdx += COUNTOF(anmTbl);
	}
	else if (m_nCurAnmIdx >= COUNTOF(anmTbl)) {
		m_nCurAnmIdx = 0;
	}

	IZ_ASSERT(m_nCurAnmIdx < COUNTOF(anmTbl));

	SAFE_RELEASE(m_pAnm);

	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();
	izanagi::CFileInputStream input;

	VRETURN(input.Open(anmTbl[m_nCurAnmIdx]));

	m_pAnm = izanagi::CAnimation::CreateAnimation(pAllocator, &input);
	IZ_ASSERT(m_pAnm != IZ_NULL);

	input.Finalize();

	m_Timeline.Init(
		m_pAnm->GetAnimationTime(),
		0.0f);
	m_Timeline.SetIsLoop(IZ_TRUE);
	m_Timeline.SetIsReverse(IZ_FALSE);
	m_Timeline.Start();
	m_Timeline.Reset();

	return IZ_TRUE;
}
