#include <stdafx.h>
#include "StateBasic.h"
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
	void SetShader(izanagi::CShaderBasic* pShader)
	{
		SAFE_REPLACE(m_pShader, pShader);
	}

private:
	izanagi::CShaderBasic* m_pShader;

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
	// TODO

	m_pShader->CommitChanges();
}

static CTestMdlRenderHandler* s_pMdlRenderHandler = IZ_NULL;

//////////////////////////////////////////////////////

#define STR_CMP(str0, str1)	(memcmp(str0, str1, strlen(str1)) == 0)

izanagi::IZ_SHADER_HANDLE CShaderTest::GetParameterByName(IZ_PCSTR pszName)
{
	if (STR_CMP(pszName, "diffuse")) {
		return GetParameterByName("g_vMtrlDiffuse");
	}
	else if (STR_CMP(pszName, "specular")) {
		return GetParameterByName("g_vMtrlSpecular");
	}
	else if (STR_CMP(pszName, "emission")) {
		return GetParameterByName("g_vMtrlAmbient");
	}

	return 0;
}

izanagi::IZ_SHADER_HANDLE CShaderTest::GetParameterBySemantic(IZ_PCSTR pszSemantic)
{
	// Nothing is done.
	return 0;
}

//////////////////////////////////////////////////////

namespace {
	inline IZ_BOOL _SetShaderParameter(
		izanagi::CShaderBasic* pShader,
		IZ_PCSTR pszName,
		const void* value,
		IZ_UINT nBytes)
	{
		izanagi::IZ_SHADER_HANDLE handle = pShader->GetParameterByName(pszName);
		IZ_ASSERT(handle != NULL);

		pShader->SetParamValue(
			handle,
			value,
			nBytes);

		return IZ_TRUE;
	}
}	// namespace

CStateBasic::CStateBasic()
{
	m_pShader = IZ_NULL;

	m_pMdl = IZ_NULL;
	m_pMsh = IZ_NULL;
	m_pSkl = IZ_NULL;

	m_pTex[0] = IZ_NULL;
	m_pTex[1] = IZ_NULL;
}

CStateBasic::~CStateBasic()
{
}

IZ_BOOL CStateBasic::Create()
{
	return IZ_TRUE;
}

#define _Print CMySystem::GetInstance().GetDebugFont()->DBPrint

IZ_BOOL CStateBasic::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

void CStateBasic::Render3D()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	//pDevice->SetRenderState(izanagi::E_GRAPH_RS_CULLMODE, izanagi::E_GRAPH_CULL_CCW);

	// 描画開始
	IZ_UINT nPassCnt = m_pShader->Begin(0, IZ_FALSE);

	// シェーダパラメータセット
	{
		izanagi::SMatrix mtxL2W;
		izanagi::SetUnitMatrix(mtxL2W);

		const izanagi::SMatrix& mtxW2C = CMyCamera::GetInstance().GetRawInterface().mtxW2C;
		const izanagi::SVector& vecEye = CMyCamera::GetInstance().GetRawInterface().pos;

		_SetShaderParameter(m_pShader, "g_mL2W", &mtxL2W, sizeof(mtxL2W));
		_SetShaderParameter(m_pShader, "g_mW2C", &mtxW2C, sizeof(mtxW2C));
		_SetShaderParameter(m_pShader, "g_vEye", &vecEye, sizeof(vecEye));
	}

	m_GeomSorter->BeginRegister();
	{
		m_GeomSorter->Register(
			0.0f,
			m_pMdl,
			izanagi::E_SCENE_REGISTER_TYPE_NORMAL);
	}
	m_GeomSorter->EndRegister();

	m_GeomSorter->Render(
		pDevice,
		m_Scene,
		s_pMdlRenderHandler);
}

void CStateBasic::Render2D()
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

IZ_BOOL CStateBasic::Update()
{
	m_pMdl->Update();

	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Destroy()
{
	return Leave();
}

IZ_BOOL CStateBasic::Enter()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
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
		VRETURN(input.Open("data/00.mtrl"));

		IZ_BOOL result = izanagi::CMaterial::CreateMaterial(pAllocator, &input, m_MtrlList);
		IZ_ASSERT(result);
		input.Finalize();
	}

	// Shader
	{
		VRETURN(input.Open("data/SkinShader.shd"));

		m_pShader = izanagi::CShaderBasic::CreateShader<izanagi::CShaderBasic>(
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
			NormalizeVector(sParallel.vDir, sParallel.vDir);

			sParallel.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		izanagi::SAmbientLightParam sAmbient;
		izanagi::SetZeroVector(sAmbient.color);
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
		m_pTex[0] = pDevice->CreateTextureFromFile("data/1P_C.dds", izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
		IZ_ASSERT(m_pTex[0] != IZ_NULL);

		m_pTex[1] = pDevice->CreateTextureFromFile("data/Face_C.dds", izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
		IZ_ASSERT(m_pTex[1] != IZ_NULL);

		izanagi::CStdList<izanagi::CMaterial>::Item* pItem = m_MtrlList.GetTop();
		while (pItem != IZ_NULL) {
			izanagi::CMaterial* pMtrl = pItem->GetData();

			if (pMtrl->GetTexInfoByName("1P_C.dds")) {
				pMtrl->SetTexture("1P_C.dds", m_pTex[0]);
			}
			else if (pMtrl->GetTexInfoByName("Face_C.dds")) {
				pMtrl->SetTexture("Face_C.dds", m_pTex[1]);
			}

			pMtrl->SetShader(m_pShader);

			pItem = pItem->GetNext();
		}
	}

	input.Finalize();

	m_GeomSorter = izanagi::CGeometrySorter<4, izanagi::CModel, izanagi::CMeshSetInstance>::CreateGeometrySorter(pAllocator);
	IZ_ASSERT(m_GeomSorter != IZ_NULL);

	m_Scene = izanagi::CScene::CreateScene(pAllocator);
	IZ_ASSERT(m_Scene != IZ_NULL);

	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Leave()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMdl);
	SAFE_RELEASE(m_pMsh);
	SAFE_RELEASE(m_pSkl);
	SAFE_RELEASE(m_pAxis);
	SAFE_RELEASE(m_pTex[0]);
	SAFE_RELEASE(m_pTex[1]);

	SAFE_RELEASE(s_pMdlRenderHandler);

	SAFE_RELEASE(m_GeomSorter);
	SAFE_RELEASE(m_Scene);

	return IZ_TRUE;
}

IZ_BOOL CStateBasic::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}