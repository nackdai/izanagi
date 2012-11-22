#include "CubeMapApp.h"

CCubeMapApp::CCubeMapApp()
{
	m_Cube = IZ_NULL;
	m_Img = IZ_NULL;
	m_Shader = IZ_NULL;

	izanagi::SMatrix::SetUnit(m_L2W);
}

CCubeMapApp::~CCubeMapApp()
{
}

// 初期化.
IZ_BOOL CCubeMapApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::graph::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	IZ_BOOL result = IZ_TRUE;

	IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
					| izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;

	// Cube
	{
		m_Cube = izanagi::sample::CSampleEnvBox::CreateSampleEnvBox(
			allocator,
			device);
		VGOTO(result = (m_Cube != IZ_NULL), __EXIT__);
	}

	// テクスチャ
	{
		izanagi::CFileInputStream in;
		VGOTO(result = in.Open("data/EnvMap.img"), __EXIT__);

		m_Img = izanagi::CImage::CreateImage(
					allocator,
					device,
					&in);
		VGOTO(result = (m_Img != IZ_NULL), __EXIT__);
	}

	// シェーダ
	{
		izanagi::CFileInputStream in;
		VGOTO(result = in.Open("data/CubeMapBoxShader.shd"), __EXIT__);

		m_Shader = izanagi::CShaderBasic::CreateShader<izanagi::CShaderBasic>(
					allocator,
					device,
					&in);
		VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
	}

	// カメラ
	camera.Init(
		izanagi::CVector(0.0f, 0.0f,  0.0f, 1.0f),
		izanagi::CVector(0.0f, 0.0f, -1.0f, 1.0f),
		izanagi::CVector(0.0f, 1.0f,  0.0f, 1.0f),
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
void CCubeMapApp::ReleaseInternal()
{
	SAFE_RELEASE(m_Cube);
	SAFE_RELEASE(m_Img);
	SAFE_RELEASE(m_Shader);
}

// 更新.
void CCubeMapApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
	GetCamera().Update();

	izanagi::SMatrix::SetScale(m_L2W, 100.0f, 100.0f, 100.0f);

	// カメラの位置にあわせて移動する
	izanagi::SMatrix::Trans(
		m_L2W,
		m_L2W,
		GetCamera().GetParam().pos);
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
void CCubeMapApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
	izanagi::sample::CSampleCamera& camera = GetCamera();

	device->SetTexture(0, m_Img->GetTexture(1));

	// テクスチャあり
	m_Shader->Begin(0, IZ_FALSE);
	{
		if (m_Shader->BeginPass(0)) {
			// パラメータ設定
			_SetShaderParam(
				m_Shader,
				"g_mW2C",
				(void*)&camera.GetParam().mtxW2C,
				sizeof(izanagi::SMatrix));

			_SetShaderParam(
				m_Shader,
				"g_mL2W",
				(void*)&m_L2W,
				sizeof(m_L2W));

			m_Shader->CommitChanges();

			m_Cube->Render(device);

			m_Shader->EndPass();
		}
	}
	m_Shader->End();
}
