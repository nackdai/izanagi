#include "DecalApp.h"

CDecalApp::CDecalApp()
{
	m_Sphere = IZ_NULL;
	m_Img = IZ_NULL;
	m_Shader = IZ_NULL;
}

CDecalApp::~CDecalApp()
{
}

// 初期化.
IZ_BOOL CDecalApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	IZ_BOOL result = IZ_TRUE;

	// 球
	{
		m_Sphere = CSphere::Create(
			allocator,
			device,
			10.0f,
			4, 4);
		VGOTO(result = (m_Sphere != IZ_NULL), __EXIT__);
	}

	// テクスチャ
	{
		izanagi::CFileInputStream in;
		VGOTO(result = in.Open("data/asset.img"), __EXIT__);

		m_Img = izanagi::CImage::CreateImage(
					allocator,
					device,
					&in);
		VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
	}

	// シェーダ
	{
		izanagi::CFileInputStream in;
		VGOTO(result = in.Open("data/BasicShader.shd"), __EXIT__);

		m_Shader = izanagi::CShaderBasic::CreateShader<izanagi::CShaderBasic>(
					allocator,
					device,
					&in);
		VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
	}

	// カメラ
	camera.Init(
		izanagi::CVector(0.0f, 0.0f, -30.0, 1.0f),
		izanagi::CVector(0.0f, 0.0f, 0.0f, 1.0f),
		izanagi::CVector(0.0f, 1.0f, 0.0f, 1.0f),
		1.0f,
		500.0f,
		izanagi::CMath::Deg2Rad(60.0f),
		(IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
	camera.Update();

__EXIT__:
	if (!result) {
		ReleaseInternal();
	}

	return IZ_TRUE;
}

// 解放.
void CDecalApp::ReleaseInternal()
{
	SAFE_RELEASE(m_Sphere);
	SAFE_RELEASE(m_Img);
	SAFE_RELEASE(m_Shader);
}

// 更新.
void CDecalApp::UpdateInternal(
	izanagi::CCamera& camera,
	izanagi::CGraphicsDevice* device)
{
	camera.Update();
}

namespace {
	inline void _SetShaderParam(
		izanagi::CShaderBasic* shader,
		const char* name,
		const void* value,
		IZ_UINT bytes)
	{
		izanagi::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
		IZ_ASSERT(handle != 0);

		shader->SetParamValue(
			handle,
			value,
			bytes);
	}
}

// 描画.
void CDecalApp::RenderInternal(izanagi::CGraphicsDevice* device)
{
	izanagi::sample::CSampleCamera& camera = GetCamera();

	device->SetRenderState(
		izanagi::E_GRAPH_RS_CULLMODE,
		izanagi::E_GRAPH_CULL_NONE);

	device->SetTexture(0, m_Img->GetTexture(0));

	izanagi::SMatrix mtx;
	izanagi::SMatrix::SetUnit(mtx);

	m_Shader->Begin(0, IZ_FALSE);
	{
		if (m_Shader->BeginPass(0)) {
			_SetShaderParam(
				m_Shader,
				"g_mL2W",
				(void*)&mtx,
				sizeof(mtx));

			_SetShaderParam(
				m_Shader,
				"g_mW2C",
				(void*)&camera.GetRawInterface().GetParam().mtxW2C,
				sizeof(camera.GetRawInterface().GetParam().mtxW2C));

			// シェーダ設定
			m_Shader->CommitChanges();

			m_Sphere->Draw();

			m_Shader->EndPass();
		}
	}

	m_Shader->End();
}

IZ_BOOL CDecalApp::OnKeyDown(IZ_UINT nChar)
{
	return IZ_TRUE;
}

void CDecalApp::OnKeyUp(IZ_UINT nChar)
{
}

IZ_BOOL CDecalApp::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
	return IZ_TRUE;
}
