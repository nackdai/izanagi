#include "scenegraph/scene/Scene.h"
#include "scenegraph/mtrl/izMaterial.h"
#include "izGraph.h"

using namespace izanagi;

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

CScene::CScene()
{
	m_pAllocator = IZ_NULL;

	m_pSceneParam = IZ_NULL;

	m_pCurShader = IZ_NULL;
	m_nCurShaderPassNum = 0;
	m_nCurShaderPass = -1;
}

CScene::~CScene()
{
	SAFE_RELEASE(m_pSceneParam);
	SAFE_RELEASE(m_pCurShader);
}

IZ_BOOL CScene::End()
{
	IZ_ASSERT(m_pCurShader != IZ_NULL);

	VRETURN(m_pCurShader->End());

	SAFE_RELEASE(m_pCurShader);
	m_nCurShaderPassNum = 0;
	m_nCurShaderPass = -1;

	return IZ_TRUE;
}
