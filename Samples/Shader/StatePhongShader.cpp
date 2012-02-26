#include "StatePhongShader.h"
#include "izGraph.h"
#include "izIo.h"
#include "StateManager.h"

CStatePhongShader::CStatePhongShader(
	izanagi::sample::CSampleApp* app,
	izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
	m_Shader = IZ_NULL;
	m_Sphere = IZ_NULL;
}

CStatePhongShader::~CStatePhongShader()
{
	Destroy();
}

// 初期化.
IZ_BOOL CStatePhongShader::Init()
{
	// Nothing is done...
	return IZ_TRUE;
}

// 更新
IZ_BOOL CStatePhongShader::Update()
{
	return IZ_TRUE;
}

namespace {
	inline void _SetShaderParam(
		izanagi::CShaderBasic* shader,
		const char* name,
		const void* value,
		IZ_UINT bytes)
	{
		izanagi::IZ_SHADER_HANDLE handle = shader->GetParameterByName(name);
		if (handle > 0) {
			shader->SetParamValue(
				handle,
				value,
				bytes);
		}
	}
}

// 描画.
IZ_BOOL CStatePhongShader::Render(izanagi::CGraphicsDevice* device)
{
	izanagi::SMatrix mtxL2W;
	izanagi::SMatrix::SetUnit(mtxL2W);

	m_Shader->Begin(0, IZ_FALSE);
	{
		if (m_Shader->BeginPass(0)) {
			// パラメータ設定
			_SetShaderParam(
				m_Shader,
				"g_mL2W",
				(void*)&mtxL2W,
				sizeof(mtxL2W));

			_SetShaderParam(
				m_Shader,
				"g_mW2C",
				(void*)&m_Camera.mtxW2C,
				sizeof(m_Camera.mtxW2C));

			_SetShaderParam(
				m_Shader,
				"g_vEye",
				(void*)&m_Camera.pos,
				sizeof(m_Camera.pos));

			{
				// ライトの方向をローカル座標に変換する

				// ライトの方向はワールド座標なので World -> Localマトリクスを計算する
				izanagi::SMatrix mtxW2L;
				izanagi::SMatrix::Inverse(mtxW2L, m_L2W);

				// World -> Local
				izanagi::SVector parallelLightLocalDir;
				izanagi::SMatrix::ApplyXYZ(
					parallelLightLocalDir,
					m_ParallelLight.vDir,
					m_L2W);

				_SetShaderParam(
					m_Shader,
					"g_vLitParallelDir",
					(void*)&parallelLightLocalDir,
					sizeof(parallelLightLocalDir));
			}

			m_Shader->CommitChanges();

			m_Sphere->Draw();
		}
	}
	m_Shader->End();

	RenderName(device, "PhongShader");

	return IZ_TRUE;
}

// 開始
IZ_BOOL CStatePhongShader::Enter(
	izanagi::IMemoryAllocator* allocator,
	void* val)
{
	izanagi::CGraphicsDevice* device = reinterpret_cast<izanagi::CGraphicsDevice*>(val);

	IZ_BOOL result = IZ_TRUE;

	// シェーダ
	{
		izanagi::CFileInputStream in;
		VRETURN(in.Open("data/PhongShader.shd"));

		m_Shader = izanagi::CShaderBasic::CreateShader<izanagi::CShaderBasic>(
					allocator,
					device,
					&in);
		VGOTO(result = (m_Shader != IZ_NULL), __EXIT__);
	}

	// ライトパラメータ
	{
		// Ambient Light Color
		izanagi::SAmbientLightParam ambient;
		ambient.color.Set(0.0f, 0.0f, 0.0f);

		// Parallel Light Color
		m_ParallelLight.color.Set(1.0f, 1.0f, 1.0f);

		// Parallel Light Direction
		m_ParallelLight.vDir.Set(-1.0f, -1.0f, -1.0f);
		izanagi::SVector::Normalize(m_ParallelLight.vDir, m_ParallelLight.vDir);

		// マテリアル
		izanagi::SMaterialParam mtrl;
		{
			mtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
			mtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
			mtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
		}

		_SetShaderParam(m_Shader, "g_vLitParallelColor", &m_ParallelLight.color, sizeof(m_ParallelLight.color));
		_SetShaderParam(m_Shader, "g_vLitAmbientColor", &ambient.color, sizeof(ambient.color));

		_SetShaderParam(m_Shader, "g_vMtrlDiffuse", &mtrl.vDiffuse, sizeof(mtrl.vDiffuse));
		_SetShaderParam(m_Shader, "g_vMtrlAmbient", &mtrl.vAmbient, sizeof(mtrl.vAmbient));
		_SetShaderParam(m_Shader, "g_vMtrlSpecular", &mtrl.vSpecular, sizeof(mtrl.vSpecular));
	}

	// 球
	{
		IZ_UINT flag = izanagi::E_DEBUG_MESH_VTX_FORM_POS
						| izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL
						| izanagi::E_DEBUG_MESH_VTX_FORM_COLOR;

		m_Sphere = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
						allocator,
						device,
						flag,
						IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0xff),
						5.0f,
						20, 20);
		VGOTO(result = (m_Sphere != IZ_NULL), __EXIT__);
	}

	izanagi::SMatrix::SetUnit(m_L2W);

__EXIT__:
	if (!result) {
		Leave();
	}

	return result;
}

// 終了.
IZ_BOOL CStatePhongShader::Destroy()
{
	return Leave();
}

// ステートから抜ける（終了）.
IZ_BOOL CStatePhongShader::Leave()
{
	SAFE_RELEASE(m_Shader);
	SAFE_RELEASE(m_Sphere);

	return IZ_TRUE;
}

// キー押下
IZ_BOOL CStatePhongShader::OnKeyDown(IZ_UINT nChar)
{
	if (nChar == VK_UP) {
		CStateManager::GetInstance().ChangeState(State_PhongShader + 1);
	}
	else if (nChar == VK_DOWN) {
		CStateManager::GetInstance().ChangeState(State_PhongShader - 1);
	}

	return IZ_TRUE;
}
