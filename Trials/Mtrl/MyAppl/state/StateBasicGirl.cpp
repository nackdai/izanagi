#include "StateBasicGirl.h"
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

CStateBasicGirl::CStateBasicGirl()
{
	m_pShader = IZ_NULL;

	m_pMdl = IZ_NULL;
	m_pMsh = IZ_NULL;
	m_pSkl = IZ_NULL;

	m_pTex[0] = IZ_NULL;
	m_pTex[1] = IZ_NULL;
}

CStateBasicGirl::~CStateBasicGirl()
{
}

IZ_BOOL CStateBasicGirl::Init()
{
	return IZ_TRUE;
}

#define _Print CMySystem::GetInstance().GetDebugFont()->DBPrint

IZ_BOOL CStateBasicGirl::Render(izanagi::graph::CGraphicsDevice* device)
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

void CStateBasicGirl::Render3D()
{
	izanagi::graph::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	//pDevice->SetRenderState(izanagi::graph::E_GRAPH_RS_CULLMODE, izanagi::graph::E_GRAPH_CULL_CCW);

	// 描画開始
	IZ_UINT nPassCnt = m_pShader->Begin(0, IZ_FALSE);

	// シェーダパラメータセット
	{
		izanagi::math::SMatrix mtxL2W;
		izanagi::math::SMatrix::SetUnit(mtxL2W);

		const izanagi::math::SMatrix& mtxW2C = CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2C;
		const izanagi::math::SVector& vecEye = CMyCamera::GetInstance().GetRawInterface().GetParam().pos;

		_SetShaderParameter(m_pShader, "g_mL2W", &mtxL2W, sizeof(mtxL2W));
		_SetShaderParameter(m_pShader, "g_mW2C", &mtxW2C, sizeof(mtxW2C));
		_SetShaderParameter(m_pShader, "g_vEye", &vecEye, sizeof(vecEye));
	}

	m_GeomSorter->BeginRegister();
	{
		izanagi::math::SVector pos;
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

void CStateBasicGirl::Render2D()
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

IZ_BOOL CStateBasicGirl::Update()
{
	m_pMdl->Update();

	return IZ_TRUE;
}

IZ_BOOL CStateBasicGirl::Destroy()
{
	return Leave();
}

IZ_BOOL CStateBasicGirl::Enter(izanagi::IMemoryAllocator* allocator, void* val)
{
	izanagi::graph::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();
	izanagi::CFileInputStream input;

	// Mesh
	{
		VRETURN(input.Open("data/00.msh"));

		m_pMsh = izanagi::CMesh::CreateMesh(
					pAllocator,
					pDevice,
					&input);
		IZ_ASSERT(m_pMsh != IZ_NULL);
		input.Finalize();
	}

	// Skeleton
	{
		VRETURN(input.Open("data/00.skl"));

		m_pSkl = izanagi::CSkeleton::CreateSkeleton(pAllocator, &input);
		IZ_ASSERT(m_pSkl != IZ_NULL);
		input.Finalize();
	}

	// Material
	{
		VRETURN(input.Open("data/00_0.mtrl"));
		m_pMtrl[0] = izanagi::CMaterial::CreateMaterial(pAllocator, &input);
		IZ_ASSERT(m_pMtrl[0] != IZ_NULL);
		input.Finalize();

		VRETURN(input.Open("data/00_1.mtrl"));
		m_pMtrl[1] = izanagi::CMaterial::CreateMaterial(pAllocator, &input);
		IZ_ASSERT(m_pMtrl[1] != IZ_NULL);
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
			izanagi::math::SVector::Normalize(sParallel.vDir, sParallel.vDir);

			sParallel.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		izanagi::SAmbientLightParam sAmbient;
		izanagi::math::SVector::SetZero(sAmbient.color);
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
		m_pTex[0] = pDevice->CreateTextureFromFile("data/1P_C.dds", izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
		IZ_ASSERT(m_pTex[0] != IZ_NULL);

		m_pTex[1] = pDevice->CreateTextureFromFile("data/Face_C.dds", izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
		IZ_ASSERT(m_pTex[1] != IZ_NULL);
	}

	input.Finalize();

	{
		m_pShader->SetName("DefaultShader");

		m_pMtrl[0]->SetName("Material_0");
		m_pMtrl[0]->SetTexture("Face_C.dds", m_pTex[1]);
		m_pMtrl[0]->SetShader(m_pShader);

		m_pMtrl[1]->SetName("Material_1");
		m_pMtrl[1]->SetTexture("1P_C.dds", m_pTex[0]);
		m_pMtrl[1]->SetShader(m_pShader);

		m_pMdl->GetMesh()->SetMaterial(0, m_pMtrl[0]);
		m_pMdl->GetMesh()->SetMaterial(0, m_pMtrl[1]);
	}

	m_GeomSorter = izanagi::CGeometrySorter::CreateGeometrySorter(pAllocator, 4);
	IZ_ASSERT(m_GeomSorter != IZ_NULL);

	m_Scene = izanagi::CSceneRenderer::CreateSceneRenderer(pAllocator);
	IZ_ASSERT(m_Scene != IZ_NULL);

	return IZ_TRUE;
}

IZ_BOOL CStateBasicGirl::Leave()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMdl);
	SAFE_RELEASE(m_pMsh);
	SAFE_RELEASE(m_pSkl);
	SAFE_RELEASE(m_pAxis);
	SAFE_RELEASE(m_pTex[0]);
	SAFE_RELEASE(m_pTex[1]);
	SAFE_RELEASE(m_pMtrl[0]);
	SAFE_RELEASE(m_pMtrl[1]);

	SAFE_RELEASE(s_pMdlRenderHandler);

	SAFE_RELEASE(m_GeomSorter);
	SAFE_RELEASE(m_Scene);

	return IZ_TRUE;
}

IZ_BOOL CStateBasicGirl::OnKeyDown(IZ_UINT nChar)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasicGirl::OnMouseMove(IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasicGirl::OnMouseWheel(IZ_SHORT zDelta)
{
	return IZ_TRUE;
}