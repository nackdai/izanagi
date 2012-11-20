#include "scenegraph/rendergraph/SceneRenderer.h"
#include "scenegraph/msh/MeshInstance.h"
#include "scenegraph/mtrl/izMaterial.h"
#include "izGraph.h"
#include "izShader.h"

using namespace izanagi;

// インスタンス作成
CSceneRenderer* CSceneRenderer::CreateSceneRenderer(IMemoryAllocator* pAllocator)
{
	void* pBuf = ALLOC_ZERO(pAllocator, sizeof(CSceneRenderer));
	VRETURN_NULL(pBuf != IZ_NULL);

	CSceneRenderer* pInstance = new(pBuf) CSceneRenderer;
	{
		pInstance->AddRef();
		pInstance->m_Allocator = pAllocator;
	}

	return pInstance;
}

// コンストラクタ
CSceneRenderer::CSceneRenderer()
{
	m_Allocator = IZ_NULL;

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
IZ_UINT CSceneRenderer::BeginRender(
	IShader* shader,
	IZ_UINT techIdx)
{
	IZ_ASSERT(shader != IZ_NULL);
	IZ_ASSERT(techIdx < shader->GetTechNum());

	if ((m_pCurShader == IZ_NULL)
		|| (m_pCurShader->GetKey() != shader->GetKey()))
	{
		// 異なるシェーダを使用する場合
		SAFE_REPLACE(m_pCurShader, shader);

		m_nCurShaderPassNum = shader->Begin(techIdx, IZ_FALSE);
	}

	// これから使用するシェーダのパス数を返す
	return m_nCurShaderPassNum;
}

// 描画途中
IZ_BOOL CSceneRenderer::IterRender(
	graph::CGraphicsDevice* device,
	IZ_UINT passIdx, 
	IRenderElement* element,
	IMshRenderHandler* renderHandler)
{
	IZ_ASSERT(m_nCurShaderPassNum > 0);
	IZ_ASSERT(m_nCurShaderPassNum > passIdx);

	CMaterial* mtrl = element->GetMaterial();
	IZ_ASSERT(mtrl != IZ_NULL);
	IZ_ASSERT(m_pCurShader != IZ_NULL);
	
	if (m_pCurShader->GetKey() != mtrl->GetShader()->GetKey()) {
		// 現在設定されているシェーダと異なるシェーダを利用しようとしている
		return IZ_FALSE;
	}

	if (m_nCurShaderPass != passIdx) {
		// Change pass.
		VRETURN(m_pCurShader->BeginPass(passIdx));
		m_nCurShaderPass = passIdx;
	}

	// マテリアルの準備をしたかどうか
	IZ_BOOL isPreparedMtrl = IZ_FALSE;

	if (m_pCurMtrl != mtrl) {
		// マテリアルの応じた設定
		isPreparedMtrl = mtrl->Prepare(device);
		VRETURN(isPreparedMtrl);

		SAFE_REPLACE(m_pCurMtrl, mtrl);
	}

	if (isPreparedMtrl) {
		// マテリアルに応じた変更が発生したので
		// シェーダのCommitChangeを行う
		VRETURN(m_pCurShader->CommitChanges());
	}

	// 要素の描画
	VRETURN(
		element->Render(
			device,
			renderHandler));

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
