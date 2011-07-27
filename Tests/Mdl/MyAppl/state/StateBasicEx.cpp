#include <stdafx.h>
#if 0
#include "StateBasicEx.h"
#include "MySystem.h"
#include "MyCamera.h"

///////////////////////////////////////////////

class CTestMdlRenderHandler : public izanagi::IMshRenderHandler {
	friend class izanagi::IMshRenderHandler;

private:
	CTestMdlRenderHandler()
	{
		m_pShader = IZ_NULL;
	}

	~CTestMdlRenderHandler()
	{
		SAFE_RELEASE(m_pShader);
	}

public:
	void BeginRenderMesh();
	void EndRenderMesh();

	void SetJointMatrix(
		IZ_UINT nIdx,
		const izanagi::SMatrix& mtx);

	void CommitChanges();

public:
	void SetShader(CSkinShader* pShader)
	{
		SAFE_REPLACE(m_pShader, pShader);
	}

private:
	CSkinShader* m_pShader;

	IZ_UINT m_nCnt;
	//izanagi::SVector m_Vec[12];
	izanagi::SMatrix m_Vec[4];
};

void CTestMdlRenderHandler::BeginRenderMesh()
{
	m_nCnt = 0;

	izanagi::SetUnitMatrix(m_Vec[0]);
	izanagi::SetUnitMatrix(m_Vec[1]);
	izanagi::SetUnitMatrix(m_Vec[2]);
	izanagi::SetUnitMatrix(m_Vec[3]);
}

void CTestMdlRenderHandler::EndRenderMesh()
{
}

void CTestMdlRenderHandler::SetJointMatrix(
	IZ_UINT nIdx,
	const izanagi::SMatrix& mtx)
{
#if 0
	izanagi::CopyVector(
		m_Vec[m_nCnt + 0],
		mtx.v[0]);
	izanagi::CopyVector(
		m_Vec[m_nCnt + 1],
		mtx.v[1]);
	izanagi::CopyVector(
		m_Vec[m_nCnt + 2],
		mtx.v[2]);

	m_nCnt += 3;
#else
	izanagi::CopyMatrix(m_Vec[m_nCnt], mtx);
	m_nCnt++;
#endif
}

void CTestMdlRenderHandler::CommitChanges()
{
	m_pShader->SetJointMatrix(
		4,
		m_Vec);

	m_pShader->CommitChanges();
}

static CTestMdlRenderHandler* s_pMdlRenderHandler = IZ_NULL;

///////////////////////////////////////////////

CStateBasicEx::CStateBasicEx()
{
	m_pShader = IZ_NULL;
	m_pMdl = IZ_NULL;
	m_pMsh = IZ_NULL;
	m_pSkl = IZ_NULL;
}

CStateBasicEx::~CStateBasicEx()
{
}

IZ_BOOL CStateBasicEx::Create()
{
	return IZ_TRUE;
}

#define _Print CMySystem::GetInstance().GetDebugFont()->DBPrint

IZ_BOOL CStateBasicEx::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

void CStateBasicEx::Render3D()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	//pDevice->SetRenderState(izanagi::E_GRAPH_RS_CULLMODE, izanagi::E_GRAPH_CULL_CCW);

	// 描画開始
	m_pShader->Begin(0);

	// 念のため確認
	IZ_UINT nPassCnt = m_pShader->GetPassCount();

	// シェーダパラメータセット
#if 0
	{
		izanagi::SMatrix mL2W;
		SetUnitMatrix(mL2W);
		m_pShader->SetL2W(mL2W);

		m_pShader->SetW2C(CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2C);
	}
#elif 1
	{
		izanagi::SMatrix mL2W;
		izanagi::SetUnitMatrix(mL2W);
		//izanagi::SetScaleMatrix(mL2W, 0.2f, 0.2f, 0.2f);
		m_pShader->SetL2W(mL2W);

		m_pShader->SetW2C(CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2C);
		m_pShader->SetCameraPos(CMyCamera::GetInstance().GetRawInterface().GetParam().pos);
	}
#else
		izanagi::SMatrix mL2W;
		izanagi::SetUnitMatrix(mL2W);
		m_pShader->SetL2W(mL2W);

		m_pShader->SetW2C(CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2C);
#endif

	m_pShader->BeginPass(1);
	m_pShader->CommitChanges();
	m_pAxis->Draw();
	m_pShader->EndPass();

	pDevice->SetTexture(0, m_pTex);

	m_pShader->BeginPass(0);
	//m_pShader->CommitChanges();
	m_pMdl->Render();
	m_pShader->EndPass();

	// 描画終了
	m_pShader->End();
}

void CStateBasicEx::Render2D()
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
			"test\n");

		CMySystem::GetInstance().GetDebugFont()->End();

		CMySystem::GetInstance().GetGraphicsDevice()->End2D();
	}
}

static IZ_FLOAT fElapsed = 0.0f;

IZ_BOOL CStateBasicEx::Update()
{
	IZ_FLOAT fElapsed = CMySystem::GetInstance().GetTimer(0).GetTime();
	fElapsed /= 1000.0f;

	m_Timeline.Advance(fElapsed);
	//m_Timeline.Advance(80);
	IZ_FLOAT time = m_Timeline.GetTime();

	//IZ_PRINTF("time [%f]\n", time);

	m_pMdl->ApplyAnimation(time, m_pAnm);

	m_pMdl->Update();
	return IZ_TRUE;
}

IZ_BOOL CStateBasicEx::Destroy()
{
	return Leave();
}

IZ_BOOL CStateBasicEx::Enter()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();
	izanagi::CFileInputStream input;

	// Mesh
	{
		IZ_ASSERT(input.Open("data/Seymour.msh"));

		m_pMsh = izanagi::CMesh::CreateMesh(
					pAllocator,
					pDevice,
					&input);
		IZ_ASSERT(m_pMsh != IZ_NULL);
		input.Finalize();
	}

	// Skeleton
	{
		IZ_ASSERT(input.Open("data/Seymour.skl"));

		m_pSkl = izanagi::CSkeleton::CreateSkeleton(
					pAllocator,
					&input);
		IZ_ASSERT(m_pSkl != IZ_NULL);
		input.Finalize();
	}

#if 0
	m_pShader = CShaderBase::CreateShader<CBasicShader>(
					pAllocator,
					pDevice,
					"data/BasicShader.bin");
	IZ_ASSERT(m_pShader != IZ_NULL);
#elif 0
	m_pShader = CShaderBase::CreateShader<CPhongShader>(
					pAllocator,
					pDevice,
					"data/PhongShader.bin");
	IZ_ASSERT(m_pShader != IZ_NULL);
	{
		// ライト
		izanagi::SParallelLightParam sParallel;
		{
			sParallel.vDir.Set(-1.0f, -1.0f, -1.0f, 0.0f);
			SVector::Normalize(sParallel.vDir, sParallel.vDir);

			sParallel.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		izanagi::SAmbientLightParam sAmbient;
		izanagi::SVector::SetZero(sAmbient.color);
		//sAmbient.color.Set(1.0f, 1.0f, 1.0f, 1.0f);

		// マテリアル
		izanagi::SMaterialParam sMtrl;
		{
			sMtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
		}

		m_pShader->SetLight(sParallel, sAmbient);
		m_pShader->SetMaterial(sMtrl);
	}
#else
	m_pShader = CShaderBase::CreateShader<CSkinShader>(
					pAllocator,
					pDevice,
					"data/SkinShader.bin");
	IZ_ASSERT(m_pShader != IZ_NULL);
	{
		// ライト
		izanagi::SParallelLightParam sParallel;
		{
			sParallel.vDir.Set(-1.0f, -1.0f, -1.0f, 0.0f);
			SVector::Normalize(sParallel.vDir, sParallel.vDir);

			sParallel.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		izanagi::SAmbientLightParam sAmbient;
		izanagi::SVector::SetZero(sAmbient.color);
		//sAmbient.color.Set(1.0f, 1.0f, 1.0f, 1.0f);

		// マテリアル
		izanagi::SMaterialParam sMtrl;
		{
			sMtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
		}

		m_pShader->SetLight(sParallel, sAmbient);
		m_pShader->SetMaterial(sMtrl);
	}
#endif

#if 1
	s_pMdlRenderHandler = izanagi::IMshRenderHandler::CreateMshRenderHandler<CTestMdlRenderHandler>(pAllocator);
	s_pMdlRenderHandler->SetShader(m_pShader);
#endif

#if 1
	// Model
	m_pMdl = izanagi::CModel::CreateModel(
				pAllocator,
				m_pMsh,
				m_pSkl,
				s_pMdlRenderHandler);
#else
	IZ_ASSERT(input.Open("data/test.mdl"));
	m_pMdl = izanagi::CModel::CreateModel(
				pAllocator,
				pDevice,
				&input,
				s_pMdlRenderHandler);
	input.Finalize();
#endif
	IZ_ASSERT(m_pMdl != IZ_NULL);

	// XYZ軸
	m_pAxis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(
				pAllocator,
				pDevice);
	IZ_ASSERT(m_pAxis != IZ_NULL);

	//m_pTex = pDevice->CreateTextureFromFile("data/duckCM.dds",
	m_pTex = pDevice->CreateTextureFromFile("data/boy_10.dds",
	//m_pTex = pDevice->CreateTextureFromFile("data/earth.bmp",
	//m_pTex = pDevice->CreateTextureFromFile("data/dice.tga",
	//m_pTex = pDevice->CreateTextureFromFile("data/Tiny_skin.dds",
				izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
	IZ_ASSERT(m_pTex != IZ_NULL);


	VRETURN(input.Open("data/Seymour.anm"));
	//VRETURN(input.Open("data/tiny.anm"));
	m_pAnm = izanagi::CAnimation::CreateAnimation(
				pAllocator,
				&input);
	IZ_ASSERT(m_pAnm != IZ_NULL);
	input.Finalize();

	m_Timeline.Init(
		m_pAnm->GetAnimationTime(),
		0.0f);
	m_Timeline.SetIsLoop(IZ_TRUE);
	m_Timeline.SetIsReverse(IZ_FALSE);
	m_Timeline.Start();

	return IZ_TRUE;
}

IZ_BOOL CStateBasicEx::Leave()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMdl);
	SAFE_RELEASE(m_pMsh);
	SAFE_RELEASE(m_pSkl);
	SAFE_RELEASE(m_pAxis);
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pAnm);

	SAFE_RELEASE(s_pMdlRenderHandler);

	return IZ_TRUE;
}

IZ_BOOL CStateBasicEx::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	if (nChar == 'P') {
		m_Timeline.Pause();
	}
	else if (nChar == 'S') {
		m_Timeline.Start();
	}
	else if (nChar == 'A') {
		m_Timeline.Advance(16.6f / 1000.0f);
	}
	return IZ_TRUE;
}

IZ_BOOL CStateBasicEx::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasicEx::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}
#endif
