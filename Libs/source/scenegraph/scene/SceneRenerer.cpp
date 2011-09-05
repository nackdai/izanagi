#include "scenegraph/scene/SceneRenderer.h"
#include "scenegraph/msh/MeshInstance.h"
#include "scenegraph/mtrl/izMaterial.h"
#include "izGraph.h"

using namespace izanagi;

// インスタンス作成
CSceneRenderer* CSceneRenderer::CreateSceneRenderer(IMemoryAllocator* pAllocator)
{
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CSceneRenderer));
	VRETURN_NULL(pBuf != IZ_NULL);

	CSceneRenderer* pInstance = new(pBuf) CSceneRenderer;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

	return pInstance;
}

// コンストラクタ
CSceneRenderer::CSceneRenderer()
{
	m_pAllocator = IZ_NULL;

	m_pSceneParam = IZ_NULL;

	m_pCurShader = IZ_NULL;
	m_nCurShaderPassNum = 0;
	m_nCurShaderPass = -1;

	m_pCurMtrl = IZ_NULL;
}

// デストラクタ
CSceneRenderer::~CSceneRenderer()
{
	SAFE_RELEASE(m_pSceneParam);
	SAFE_RELEASE(m_pCurShader);
	SAFE_RELEASE(m_pCurMtrl);
}

// 描画開始
IZ_UINT CSceneRenderer::BeginRender(IMeshSet* pMesh)
{
	CMaterial* pMtrl = pMesh->GetMaterial();

	// TODO
#if 0
	if (pMtrl == IZ_NULL) {
		const S_MSH_MTRL& sMtrlInfo = pMesh->GetMaterialInfo();
		pMtrl = GetMaterial(sMtrlInfo.nameKey);
		pMesh->SetMaterial(pMtrl);
	}
#endif

	IZ_ASSERT(pMtrl != IZ_NULL);

	CShader* pShader = pMtrl->GetShader();
	VRETURN(pShader != IZ_NULL);

	if ((m_pCurShader == IZ_NULL)
		|| (m_pCurShader->GetKey() != pShader->GetKey()))
	{
		SAFE_REPLACE(m_pCurShader, pShader);

		IZ_INT nTechIdx = pMtrl->GetShaderTechnique();
		IZ_ASSERT(nTechIdx >= 0);

		m_nCurShaderPassNum = pShader->Begin(nTechIdx, IZ_FALSE);
	}

	return m_nCurShaderPassNum;
}

// 描画途中
IZ_BOOL CSceneRenderer::IterRender(
	CGraphicsDevice* pDevice,
	IZ_UINT nPass, 
	IMeshSet* pMesh,
	IMshRenderHandler* pRenderHandler)
{
	IZ_ASSERT(m_nCurShaderPassNum > 0);
	IZ_ASSERT(m_nCurShaderPassNum > nPass);

	CMaterial* pMtrl = pMesh->GetMaterial();
	IZ_ASSERT(pMtrl != IZ_NULL);
	IZ_ASSERT(m_pCurShader != IZ_NULL);
	
	if (m_pCurShader->GetKey() != pMtrl->GetShader()->GetKey()) {
		// Specify differing shader.
		return IZ_FALSE;
	}

	if (m_nCurShaderPass != nPass) {
		// Change pass.
		VRETURN(m_pCurShader->BeginPass(nPass));
		m_nCurShaderPass = nPass;
	}

	IZ_BOOL bIsPreparedMtrl = IZ_FALSE;

	if (m_pCurMtrl != pMtrl) {
		// Prepare material.
		bIsPreparedMtrl = pMtrl->Prepare(pDevice);
		VRETURN(bIsPreparedMtrl);

		SAFE_REPLACE(m_pCurMtrl, pMtrl);
	}

	if (bIsPreparedMtrl) {
		// マテリアルに応じた変更が発生したので
		// シェーダのCommitChangeを行う
		VRETURN(m_pCurShader->CommitChanges());
	}

	// Render mesh.
	VRETURN(
		pMesh->Render(
			pDevice,
			pRenderHandler));

	return IZ_TRUE;
}

IZ_BOOL CSceneRenderer::EndRender()
{
	IZ_ASSERT(m_pCurShader != IZ_NULL);

	VRETURN(m_pCurShader->End());

	SAFE_RELEASE(m_pCurShader);
	m_nCurShaderPassNum = 0;
	m_nCurShaderPass = -1;

	return IZ_TRUE;
}
