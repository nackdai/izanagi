#include "MouseHit.h"

CMouseHit::CMouseHit()
{
	m_Triangles.vb = IZ_NULL;
	
	m_Shader = IZ_NULL;

	m_VD = IZ_NULL;

	m_IsPressKey = IZ_FALSE;

	m_MousePoint.x = -1;
	m_MousePoint.y = -1;

	m_IsCross = IZ_FALSE;
}

CMouseHit::~CMouseHit()
{
}

struct SVertex {
	izanagi::math::SVector pos;
	IZ_COLOR color;
};

// 初期化.
IZ_BOOL CMouseHit::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::graph::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	IZ_BOOL result = IZ_TRUE;

	static const izanagi::math::SVector triPoint[3] =
	{
		{ -10.0f, 0.0f,  0.0f, 1.0f },
		{ -10.0f, 0.0f, 20.0f, 1.0f },
		{  10.0f, 0.0f, 20.0f, 1.0f },
	};

	static const izanagi::graph::SVertexElement vtxElem[] =
	{
		{0,  0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0},	// 座標
		{0, 16, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_COLOR, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_COLOR, 0},		// 色
	};

	{
		izanagi::math::SMatrix::SetUnit(m_Triangles.mtx);
		m_Triangles.tri.Set(triPoint);

		m_Triangles.vb = device->CreateVertexBuffer(
			sizeof(SVertex),
			3,
			izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);

		SVertex* data = IZ_NULL;
		m_Triangles.vb->Lock(0, 0, (void**)&data, IZ_FALSE);
		{
			for (IZ_UINT i = 0; i < 3; i++)
			{
				izanagi::math::SVector::Copy(data[i].pos, triPoint[i]);
				data[i].color = 0xffffffff;
			}
		}
		m_Triangles.vb->Unlock();

		m_VD = device->CreateVertexDeclaration(vtxElem, COUNTOF(vtxElem));
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
		izanagi::math::CVector(0.0f, 5.0f, -30.0f, 1.0f),
		izanagi::math::CVector(0.0f, 5.0f, 0.0f, 1.0f),
		izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
		1.0f,
		500.0f,
		izanagi::math::CMath::Deg2Rad(60.0f),
		(IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
	camera.Update();

__EXIT__:
	if (!result) {
		ReleaseInternal();
	}

	return IZ_TRUE;
}

// 解放.
void CMouseHit::ReleaseInternal()
{
	SAFE_RELEASE(m_Triangles.vb);
	SAFE_RELEASE(m_Shader);
	SAFE_RELEASE(m_VD);
}

// 更新.
void CMouseHit::UpdateInternal(
	izanagi::CCamera& camera,
	izanagi::graph::CGraphicsDevice* device)
{
	camera.Update();

	m_IsCross = IZ_FALSE;
	izanagi::math::SVector dir;

	// 2D -> 3D
	if (m_MousePoint.x >= 0
		&& m_MousePoint.y >= 0)
	{
		izanagi::CSceneGraphUtil::Point2Ray(
			dir,
			camera.GetParam(),
			device->GetViewport(),
			m_MousePoint.x,
			m_MousePoint.y);

		izanagi::math::CRay ray(
			camera.GetParam().pos,
			dir);

		m_IsCross = m_Triangles.tri.IsCross(ray);
	}
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
void CMouseHit::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
	izanagi::sample::CSampleCamera& camera = GetCamera();

	device->SetRenderState(
		izanagi::graph::E_GRAPH_RS_CULLMODE,
		izanagi::graph::E_GRAPH_CULL_NONE);

	static const izanagi::math::SVector color[2] = {
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 0.5f },
	};

	m_Shader->Begin(0, IZ_FALSE);
	if (m_Shader->BeginPass(0)) {
		// パラメータ設定
		_SetShaderParam(
			m_Shader,
			"g_mL2W",
			(void*)&m_Triangles.mtx,
			sizeof(m_Triangles.mtx));

		_SetShaderParam(
			m_Shader,
			"g_mW2C",
			(void*)&camera.GetParam().mtxW2C,
			sizeof(camera.GetParam().mtxW2C));

		const izanagi::math::SVector& clr = (m_IsCross ? color[1] : color[0]);

		_SetShaderParam(
			m_Shader,
			"g_Color",
			(void*)&clr,
			sizeof(clr));

		// シェーダ設定
		m_Shader->CommitChanges();

		device->SetVertexDeclaration(m_VD);
		device->SetVertexBuffer(
			0, 0,
			m_Triangles.vb->GetStride(), 
			m_Triangles.vb);
		device->DrawPrimitive(
			izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST,
			0, 1);

		m_Shader->EndPass();
	}

	m_Shader->End();
}

IZ_BOOL CMouseHit::OnKeyDown(IZ_UINT nChar)
{
	m_IsPressKey = IZ_TRUE;
	return IZ_TRUE;
}

void CMouseHit::OnKeyUp(IZ_UINT nChar)
{
	m_IsPressKey = IZ_FALSE;
}

IZ_BOOL CMouseHit::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
	if (m_IsPressKey)
	{
		m_MousePoint.x = point.x;
		m_MousePoint.y = point.y;
	}

	return IZ_TRUE;
}
