#include <stdafx.h>
#include "StateBasicBoy.h"
#include "MySystem.h"
#include "MyCamera.h"
#include "Common.h"

static CTestMdlRenderHandler* s_pMdlRenderHandler = IZ_NULL;

namespace {
	inline IZ_BOOL _SetShaderParameter(
		izanagi::CShaderBasic* pShader,
		IZ_PCSTR pszName,
		const void* value,
		IZ_UINT nBytes)
	{
		izanagi::IZ_SHADER_HANDLE handle = pShader->GetParameterByName(pszName);

		if (handle > 0) {
			pShader->SetParamValue(
				handle,
				value,
				nBytes);
		}

		return IZ_TRUE;
	}
}	// namespace

CStateBasicBoy::CStateBasicBoy()
{
	m_pShader = IZ_NULL;

	m_pMdl = IZ_NULL;
	m_pMsh = IZ_NULL;
	m_pSkl = IZ_NULL;

	m_pTex = IZ_NULL;
}

CStateBasicBoy::~CStateBasicBoy()
{
}

IZ_BOOL CStateBasicBoy::Create()
{
	return IZ_TRUE;
}

#define _Print CMySystem::GetInstance().GetDebugFont()->DBPrint

IZ_BOOL CStateBasicBoy::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

void CStateBasicBoy::Render3D()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	//pDevice->SetRenderState(izanagi::E_GRAPH_RS_CULLMODE, izanagi::E_GRAPH_CULL_CCW);

	// 描画開始
	IZ_UINT nPassCnt = m_pShader->Begin(0, IZ_FALSE);

	// シェーダパラメータセット
	{
		izanagi::SMatrix mtxL2W;
		izanagi::SMatrix::SetUnit(mtxL2W);

		const izanagi::SMatrix& mtxW2C = CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2C;
		const izanagi::SVector& vecEye = CMyCamera::GetInstance().GetRawInterface().GetParam().pos;

		_SetShaderParameter(m_pShader, "g_mL2W", &mtxL2W, sizeof(mtxL2W));
		_SetShaderParameter(m_pShader, "g_mW2C", &mtxW2C, sizeof(mtxW2C));
		_SetShaderParameter(m_pShader, "g_vEye", &vecEye, sizeof(vecEye));
	}

	m_GeomSorter->BeginRegister();
	{
		izanagi::SVector pos;
		pos.Set(0.0f, 0.0f, 0.0f);

		m_GeomSorter->Register(
			CMyCamera::GetInstance().GetRawInterface(),
			pos,
			m_pMdl,
			izanagi::E_SCENE_REGISTER_TYPE_NORMAL);
	}
	m_GeomSorter->EndRegister();

	m_GeomSorter->Render(
		pDevice,
		m_Scene,
		s_pMdlRenderHandler);
}

void CStateBasicBoy::Render2D()
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

IZ_BOOL CStateBasicBoy::Update()
{
	m_pMdl->Update();

	return IZ_TRUE;
}

IZ_BOOL CStateBasicBoy::Destroy()
{
	return Leave();
}

IZ_BOOL CStateBasicBoy::Enter()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();
	izanagi::CFileInputStream input;

	// Mesh
	{
		VRETURN(input.Open("data/Seymour.msh"));

		m_pMsh = izanagi::CMesh::CreateMesh(
					pAllocator,
					pDevice,
					&input);
		IZ_ASSERT(m_pMsh != IZ_NULL);
		input.Finalize();
	}

	// Skeleton
	{
		VRETURN(input.Open("data/Seymour.skl"));

		m_pSkl = izanagi::CSkeleton::CreateSkeleton(pAllocator, &input);
		IZ_ASSERT(m_pSkl != IZ_NULL);
		input.Finalize();
	}

	static char tmp[128];

	// Material
	for (IZ_UINT i = 0; i < 4; i++) {
		sprintf_s(tmp, sizeof(tmp), "data/Seymour_%d.mtrl\0", i);

		VRETURN(input.Open(tmp));
		m_pMtrl[i] = izanagi::CMaterial::CreateMaterial(pAllocator, &input);
		IZ_ASSERT(m_pMtrl[i] != IZ_NULL);
		input.Finalize();
	}

	// Shader
	{
		VRETURN(input.Open("data/SkinShader.shd"));

		m_pShader = izanagi::CShaderBasic::CreateShader<CShaderTest>(
						pAllocator,
						pDevice,
						&input);
		IZ_ASSERT(m_pShader != IZ_NULL);
		input.Finalize();
	}

	{
		// ライト
		izanagi::SParallelLightParam sParallel;
		{
			sParallel.vDir.Set(-1.0f, -1.0f, -1.0f, 0.0f);
			izanagi::SVector::Normalize(sParallel.vDir, sParallel.vDir);

			sParallel.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		izanagi::SAmbientLightParam sAmbient;
		izanagi::SVector::SetZero(sAmbient.color);
		//sAmbient.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		
		_SetShaderParameter(
			m_pShader,
			"g_vLitParallelDir",
			&sParallel.vDir,
			sizeof(sParallel.vDir));
		_SetShaderParameter(
			m_pShader,
			"g_vLitParallelColor",
			&sParallel.color,
			sizeof(sParallel.color));
		_SetShaderParameter(
			m_pShader,
			"g_vLitAmbientColor",
			&sAmbient.color,
			sizeof(sAmbient.color));

		// マテリアル
		izanagi::SMaterialParam sMtrl;
		{
			sMtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
		}

		_SetShaderParameter(
			m_pShader,
			"g_vMtrlDiffuse",
			&sMtrl.vDiffuse,
			sizeof(sMtrl.vDiffuse));
		_SetShaderParameter(
			m_pShader,
			"g_vMtrlAmbient",
			&sMtrl.vAmbient,
			sizeof(sMtrl.vAmbient));
		_SetShaderParameter(
			m_pShader,
			"g_vMtrlSpecular",
			&sMtrl.vSpecular,
			sizeof(sMtrl.vSpecular));
	}

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

	{
		m_pTex = pDevice->CreateTextureFromFile("data/boy_10.dds", izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
		IZ_ASSERT(m_pTex != IZ_NULL);
	}

	input.Finalize();

	{
		m_pShader->SetName("DefaultShader");

		for (IZ_UINT i = 0; i < 4; i++) {
			IZ_PCSTR name = m_pMtrl[i]->GetName();
			m_pMtrl[i]->SetTexture("boy_10.tga", m_pTex);
			m_pMtrl[i]->SetShader(m_pShader);

			m_pMdl->GetMesh()->SetMaterial(0, m_pMtrl[i]);
		}
	}

	m_GeomSorter = izanagi::CGeometrySorter::CreateGeometrySorter(pAllocator, 4);
	IZ_ASSERT(m_GeomSorter != IZ_NULL);

	m_Scene = izanagi::CScene::CreateScene(pAllocator);
	IZ_ASSERT(m_Scene != IZ_NULL);

	return IZ_TRUE;
}

IZ_BOOL CStateBasicBoy::Leave()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMdl);
	SAFE_RELEASE(m_pMsh);
	SAFE_RELEASE(m_pSkl);
	SAFE_RELEASE(m_pAxis);
	SAFE_RELEASE(m_pTex);

	for (IZ_UINT i = 0; i < 4; i++) {
		SAFE_RELEASE(m_pMtrl[i]);
	}

	SAFE_RELEASE(s_pMdlRenderHandler);

	SAFE_RELEASE(m_GeomSorter);
	SAFE_RELEASE(m_Scene);

	return IZ_TRUE;
}

IZ_BOOL CStateBasicBoy::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasicBoy::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasicBoy::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}