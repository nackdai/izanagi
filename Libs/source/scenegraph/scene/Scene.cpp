#include "scenegraph/scene/Scene.h"
#include "scenegraph/msh/MeshInstance.h"
#include "scenegraph/mtrl/izMaterial.h"
#include "izGraph.h"

using namespace izanagi;

// インスタンス作成
CScene* CScene::CreateScene(IMemoryAllocator* pAllocator)
{
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CScene));
	VRETURN_NULL(pBuf != IZ_NULL);

	CScene* pInstance = new(pBuf) CScene;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

	return pInstance;
}

// コンストラクタ
CScene::CScene()
{
	m_pAllocator = IZ_NULL;

	m_pSceneParam = IZ_NULL;

	m_pCurShader = IZ_NULL;
	m_nCurShaderPassNum = 0;
	m_nCurShaderPass = -1;
}

// デストラクタ
CScene::~CScene()
{
	SAFE_RELEASE(m_pSceneParam);
	SAFE_RELEASE(m_pCurShader);
}

// 描画開始
IZ_UINT CScene::BeginRender(CMeshSetInstance* pMesh)
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
IZ_BOOL CScene::IterRender(
	CGraphicsDevice* pDevice,
	IZ_UINT nPass, 
	CMeshSetInstance* pMesh)
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

	// Prepare material.
	VRETURN(pMtrl->Prepare(pDevice));

	VRETURN(m_pCurShader->CommitChanges());

	// TODO
	// Render mesh.

	return IZ_TRUE;
}

IZ_BOOL CScene::EndRender()
{
	IZ_ASSERT(m_pCurShader != IZ_NULL);

	VRETURN(m_pCurShader->End());

	SAFE_RELEASE(m_pCurShader);
	m_nCurShaderPassNum = 0;
	m_nCurShaderPass = -1;

	return IZ_TRUE;
}
