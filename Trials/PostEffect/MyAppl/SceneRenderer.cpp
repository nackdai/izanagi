#include "SceneRenderer.h"

#include "MyCamera.h"

CSceneRenderer CSceneRenderer::s_cInstance;

// 開放
void CSceneRenderer::Release()
{
	SAFE_RELEASE(m_pPhongShader);
}

// シェーダ初期化
IZ_BOOL CSceneRenderer::InitShader(
	izanagi::IMemoryAllocator* pAllocator,
	izanagi::graph::CGraphicsDevice* pDevice,
	LPCSTR lpszName)
{
	IZ_BOOL ret = IZ_FALSE;

	if (m_pPhongShader == IZ_NULL) {
		// シェーダ作成
		m_pPhongShader = CShaderBase::CreateShader<CPhongShader>(
							pAllocator,
							pDevice,
							lpszName);
		ret = (m_pPhongShader != IZ_NULL);
		IZ_ASSERT(ret);
	}
	else {
		ret = m_pPhongShader->Reset(pDevice, lpszName);
		IZ_ASSERT(ret);
	}

	// TODO
	// テストなので、ここで各種パラメータ設定
	if (ret) {
		// ライト
		izanagi::SParallelLightParam sParallel;
		{
			sParallel.vDir.Set(1.0f, 1.0f, 1.0f, 0.0f);
			izanagi::math::SVector::Normalize(sParallel.vDir, sParallel.vDir);

			sParallel.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		izanagi::SAmbientLightParam sAmbient;
		izanagi::math::SVector::SetZero(sAmbient.color);

		// マテリアル
		izanagi::SMaterialParam sMtrl;
		{
			sMtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
		}

		m_pPhongShader->SetLight(sParallel, sAmbient);
		m_pPhongShader->SetMaterial(sMtrl);
	}

	return ret;
}

// 描画
void CSceneRenderer::Render(izanagi::graph::CGraphicsDevice* pDevice)
{
#if 0
	pDevice->SetTextureMappingRenderState(0, IZ_FALSE);
	CMeshManager::GetInstance().Render(CMeshManager::MESH_TEAPOT);

	// NOTE
	// LPD3DXMESHの描画で勝手にFVFを変更しているので、ここで強制的に適当なFVFをセットしてクリアする
	pDevice->SetFVF(0);

	pDevice->SetTextureMappingRenderState(0, IZ_TRUE);
#else
#if 0
	pDevice->SetRenderState(
		izanagi::graph::E_GRAPH_RS_CULLMODE,
		izanagi::graph::E_GRAPH_CULL_CCW);
#endif

	izanagi::math::SMatrix mL2W;
	izanagi::math::SMatrix::SetUnit(mL2W);
	m_pPhongShader->SetL2W(mL2W);

	m_pPhongShader->SetW2C(CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2C);
	m_pPhongShader->SetCameraPos(CMyCamera::GetInstance().GetRawInterface().GetParam().pos);

	m_pPhongShader->Begin(0);
	IZ_UINT nPassCnt = m_pPhongShader->GetPassCount();

	for (IZ_UINT i = 0; i < nPassCnt; i++) {
		if (m_pPhongShader->BeginPass(i)) {
			CMeshManager::GetInstance().Render(CMeshManager::MESH_TEAPOT);

			m_pPhongShader->EndPass();
		}
	}

	m_pPhongShader->End();

	pDevice->SetRenderState(
		izanagi::graph::E_GRAPH_RS_CULLMODE,
		izanagi::graph::E_GRAPH_CULL_CW);
#endif
}
