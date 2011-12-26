#include "Scene.h"
#include "Character.h"
#include "MySystem.h"
#include "MyCamera.h"
#include "MyAppl.h"
#include "izIo.h"

static CTestMdlRenderHandler* s_pMdlRenderHandler = IZ_NULL;

CScene::CScene()
{
	m_pShader = IZ_NULL;

	m_pAxis = IZ_NULL;

	m_GeomSorter = IZ_NULL;
	m_SceneRenderer = IZ_NULL;
}

CScene::~CScene()
{
	Clear();
}

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

IZ_BOOL CScene::Init(
	izanagi::IMemoryAllocator* allocator,
	izanagi::CGraphicsDevice* device)
{
	IZ_ASSERT(allocator != IZ_NULL);
	IZ_ASSERT(device != IZ_NULL);

	izanagi::CFileInputStream input;

	// Shader
	{
		VRETURN(input.Open("data/SkinShader.shd"));

		m_pShader = izanagi::CShaderBasic::CreateShader<CShaderTest>(
						allocator,
						device,
						&input);
		IZ_ASSERT(m_pShader != IZ_NULL);
		input.Finalize();

		m_pShader->SetName("DefaultShader");
	}

	s_pMdlRenderHandler = izanagi::IMshRenderHandler::CreateMshRenderHandler<CTestMdlRenderHandler>(allocator);
	s_pMdlRenderHandler->SetShader(m_pShader);

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

	// XYZ軸
	m_pAxis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(
				allocator,
				device);
	IZ_ASSERT(m_pAxis != IZ_NULL);

	m_GeomSorter = izanagi::CGeometrySorter::CreateGeometrySorter(allocator, 4);
	IZ_ASSERT(m_GeomSorter != IZ_NULL);

	m_SceneRenderer = izanagi::CSceneRenderer::CreateSceneRenderer(allocator);
	IZ_ASSERT(m_SceneRenderer != IZ_NULL);

	return IZ_TRUE;
}

void CScene::Clear()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pAxis);

	SAFE_RELEASE(s_pMdlRenderHandler);

	SAFE_RELEASE(m_GeomSorter);
	SAFE_RELEASE(m_SceneRenderer);
}

void CScene::Render(
	CCharacter* character,
	izanagi::CGraphicsDevice* device)
{
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

	character->SetShader(m_pShader);

	m_GeomSorter->BeginRegister();
	{
		izanagi::SVector pos;
		pos.Set(0.0f, 0.0f, 0.0f);

		m_GeomSorter->Register(
			CMyCamera::GetInstance().GetRawInterface(),
			pos,
			character->GetMdl(),
			izanagi::E_SCENE_REGISTER_TYPE_NORMAL);
	}
	m_GeomSorter->EndRegister();

	m_GeomSorter->Render(
		device,
		m_SceneRenderer,
		s_pMdlRenderHandler);
}
