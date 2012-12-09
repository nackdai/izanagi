#include "Scissoring.h"

CScissoring::CScissoring()
{
	memset(&m_Rectangle, 0, sizeof(m_Rectangle));
	memset(&m_Triangles, 0, sizeof(m_Triangles));
	m_TriNum = 0;
	m_Shader = IZ_NULL;
	
	m_EnableScissoring = IZ_FALSE;
	m_CopiedVtx = IZ_FALSE;
}

CScissoring::~CScissoring()
{
}

struct SVertex {
	IZ_FLOAT pos[4];
	IZ_COLOR color;
};

// 初期化.
IZ_BOOL CScissoring::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::graph::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	IZ_BOOL result = IZ_TRUE;

	static const IZ_FLOAT Length = 100.0f;

	{
		m_Rectangle.mesh = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
			allocator,
			device,
			izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR,
			IZ_COLOR_RGBA(0xff, 0x00, 0x00, 0x80),
			1, 1,
			Length, Length);

		m_Rectangle.rc.Set(
			izanagi::math::CVector(0.0f, 0.0f, 0.0f),
			Length,
			Length);

		izanagi::math::SMatrix::GetRotByZ(
			m_Rectangle.mtx,
			IZ_DEG2RAD(90.0f));

		m_Rectangle.rc.Transform(m_Rectangle.mtx);
	}

	{
		static const izanagi::graph::SVertexElement vtxElem[] =
		{
			{0,  0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT4, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION, 0},	// 座標
			{0, 16, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_COLOR, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_COLOR, 0},		// 色
		};

		static const SVertex vtx[] =
		{
			{ {-20.0f, 0.0f,  20.0f, 1.0f}, 0xffffffff},
			{ { 20.0f, 0.0f,  20.0f, 1.0f}, 0xffffffff},
			{ {-20.0f, 0.0f, -20.0f, 1.0f}, 0xffffffff},
		};
		IZ_C_ASSERT(COUNTOF(vtx) == COUNTOF(m_Triangles.tri.pt));

		m_Triangles.vd = device->CreateVertexDeclaration(vtxElem, COUNTOF(vtxElem));
		VGOTO(m_Triangles.vd != IZ_NULL, __EXIT__);

		m_Triangles.defaultVB = device->CreateVertexBuffer(
			sizeof(SVertex),
			COUNTOF(vtx),
			izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);
		VGOTO(m_Triangles.defaultVB != IZ_NULL, __EXIT__);

		void* data = IZ_NULL;
		m_Triangles.defaultVB->Lock(0, 0, &data, IZ_FALSE);
		memcpy(data, vtx, sizeof(SVertex) * COUNTOF(vtx));
		m_Triangles.defaultVB->Unlock();

		for (IZ_UINT i = 0; i < COUNTOF(vtx); i++)
		{
			m_Triangles.tri.pt[i].Set(vtx[i].pos[0], vtx[i].pos[1], vtx[i].pos[2]);
		}

		for (IZ_UINT i = 0; i < COUNTOF(m_Triangles.vb); i++)
		{
			m_Triangles.vb[i] = device->CreateVertexBuffer(
				sizeof(SVertex),
				COUNTOF(vtx),
				izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);
			VGOTO(m_Triangles.vb[i] != IZ_NULL, __EXIT__);
		}

		izanagi::math::SMatrix::SetUnit(m_Triangles.mtx);

		m_Triangles.tri.Transform(m_Triangles.mtx);
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
		izanagi::math::CVector(-50.0f, 20.0f, -200.0, 1.0f),
		izanagi::math::CVector(0.0f, 0.0f, 0.0f, 1.0f),
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
void CScissoring::ReleaseInternal()
{
	SAFE_RELEASE(m_Rectangle.mesh);

	SAFE_RELEASE(m_Triangles.defaultVB);
	SAFE_RELEASE(m_Triangles.vb[0]);
	SAFE_RELEASE(m_Triangles.vb[1]);
	SAFE_RELEASE(m_Triangles.vd);

	SAFE_RELEASE(m_Shader);
}

// 更新.
void CScissoring::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::CCamera& camera = GetCamera();
	camera.Update();

	if (m_EnableScissoring)
	{
		izanagi::math::SPlane plane;
		m_Rectangle.rc.GetPlane(plane);

		m_TriNum = izanagi::CSceneGraphUtil::ComputeTriNumBySissoring(
			plane,
			&m_Triangles.tri,
			1);

		if (m_TriNum > 0)
		{
			IZ_ASSERT(m_TriNum <= COUNTOF(m_NewTri));

			izanagi::CSceneGraphUtil::Sissoring(
				plane,
				&m_Triangles.tri, 1,
				m_NewTri, m_TriNum);
		}
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
void CScissoring::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
	izanagi::sample::CSampleCamera& camera = GetCamera();

	device->SetRenderState(
		izanagi::graph::E_GRAPH_RS_CULLMODE,
		izanagi::graph::E_GRAPH_CULL_NONE);

	if (m_EnableScissoring && m_TriNum > 0 && !m_CopiedVtx)
	{
		SVertex vtx[3];

		// シザリングした結果の頂点を作成
		for (IZ_UINT i = 0; i < m_TriNum; i++)
		{
			// 頂点作成
			for (IZ_UINT n = 0; n < 3; n++)
			{
				vtx[n].color = 0xffffffff;
				vtx[n].pos[0] = m_NewTri[i].pt[n].x;
				vtx[n].pos[1] = m_NewTri[i].pt[n].y;
				vtx[n].pos[2] = m_NewTri[i].pt[n].z;
				vtx[n].pos[3] = 1.0f;
			}

			void* data = IZ_NULL;
			m_Triangles.vb[i]->Lock(0, 0, &data, IZ_FALSE);
			memcpy(data, vtx, sizeof(vtx));
			m_Triangles.vb[i]->Unlock();
		}

		// コピー完了
		m_CopiedVtx = IZ_TRUE;
	}

	m_Shader->Begin(0, IZ_FALSE);
	{
		if (m_Shader->BeginPass(0)) {
			// 平面
			{
				_SetShaderParam(
					m_Shader,
					"g_mL2W",
					(void*)&m_Rectangle.mtx,
					sizeof(m_Rectangle.mtx));

				_SetShaderParam(
					m_Shader,
					"g_mW2C",
					(void*)&camera.GetParam().mtxW2C,
					sizeof(camera.GetParam().mtxW2C));

				// シェーダ設定
				m_Shader->CommitChanges();

				m_Rectangle.mesh->Draw();
			}
			// 三角形
			{
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

				// シェーダ設定
				m_Shader->CommitChanges();

				device->SetVertexDeclaration(m_Triangles.vd);
				if (m_EnableScissoring)
				{
					for (IZ_UINT i = 0; i < m_TriNum; i++)
					{
						device->SetVertexBuffer(0, 0, m_Triangles.vb[i]->GetStride(), m_Triangles.vb[i]);
						device->DrawPrimitive(
							izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST,
							0, 1);
					}
				}
				else
				{
					device->SetVertexBuffer(0, 0, m_Triangles.defaultVB->GetStride(), m_Triangles.defaultVB);
					device->DrawPrimitive(
						izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLELIST,
						0, 1);
				}
			}
			m_Shader->EndPass();
		}
	}

	m_Shader->End();
}

IZ_BOOL CScissoring::OnKeyDown(IZ_UINT nChar)
{
	if (nChar == 'S')
	{
		m_EnableScissoring = !m_EnableScissoring;
		m_CopiedVtx = (m_EnableScissoring ? IZ_FALSE : IZ_TRUE);
	}

	return IZ_TRUE;
}

void CScissoring::OnKeyUp(IZ_UINT nChar)
{
}

IZ_BOOL CScissoring::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
	return IZ_TRUE;
}
