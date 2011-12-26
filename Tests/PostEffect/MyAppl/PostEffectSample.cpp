#include "PostEffectSample.h"
#include "MyCamera.h"

CPostEffectSample CPostEffectSample::s_cInstance;

// コンストラクタ
CPostEffectSample::CPostEffectSample()
{
	m_pPostEffectSystem = IZ_NULL;
	m_pPostEffect = IZ_NULL;
	m_pPostEffectTexCreator = IZ_NULL;

	m_pSrcTex = IZ_NULL;

	m_bEnablePostEffect = IZ_TRUE;

	m_nCurPostEffectTech = 0;

	memset(m_pszCurFile, 0, sizeof(m_pszCurFile));
}

// デストラクタ
CPostEffectSample::~CPostEffectSample()
{
	Release();
}

// 初期化
IZ_BOOL CPostEffectSample::Init(
	izanagi::IMemoryAllocator* pAllocator,
	izanagi::CGraphicsDevice* pDevice)
{
	static const IZ_UINT nDefaultTexMgrCreateMax = 32;

	IZ_BOOL ret = IZ_TRUE;

	m_pAllocator = pAllocator;
	SAFE_REPLACE(m_pDevice, pDevice);

	// ポストエフェクトシステム作成
	// 全てのポストエフェクトの元になる
	// 必ず最初に作成すること
	m_pPostEffectSystem = izanagi::CPostEffectSystem::CreatePostEffectSystem(
							pAllocator,
							pDevice);
	ret = (m_pPostEffectSystem != IZ_NULL);

	if (ret) {
		// テクスチャクリエータ作成
		// ポストエフェクトで使用されるテクスチャは、テクスチャクリエータを通じて作成、管理される
		m_pPostEffectTexCreator = izanagi::CPostEffectTextureCreator::CreatePostEffectTextureCreator(
									pAllocator,
									pDevice,
									nDefaultTexMgrCreateMax);	// 管理可能テクスチャ最大数
		ret = (m_pPostEffectTexCreator != IZ_NULL);

		if (ret) {
			// システムにセット
			m_pPostEffectSystem->SetTextureCreator(m_pPostEffectTexCreator);
		}
	}
	
	return ret;
}

// 開放
void CPostEffectSample::Release()
{
	m_pAllocator = IZ_NULL;

	SAFE_RELEASE(m_pPostEffect);
	SAFE_RELEASE(m_pPostEffectSystem);
	SAFE_RELEASE(m_pPostEffectTexCreator);

	SAFE_RELEASE(m_pSrcTex);

	SAFE_RELEASE(m_pDevice);
}

// ポストエフェクト読み込み
IZ_BOOL CPostEffectSample::Read(LPCSTR lpszName)
{
	IZ_BOOL ret = IZ_FALSE;

	izanagi::CFileInputStream in;
	if (!in.Open(lpszName)) {
		IZ_ASSERT(IZ_FALSE);
		return IZ_FALSE;
	}

	SAFE_RELEASE(m_pPostEffect);

	if (m_pPostEffectSystem != IZ_NULL) {
		m_pPostEffectSystem->GetTextureCreator()->ClearAll();

		// ポストエフェクト作成
		m_pPostEffect = m_pPostEffectSystem->CreatePostEffect(&in);
		ret = (m_pPostEffect != IZ_NULL);

		if (ret) {
			// 入力シーンテクスチャ取得
			SAFE_REPLACE(
				m_pSrcTex,
				m_pPostEffect->GetInputSceneTex());

			ret = (m_pSrcTex != IZ_NULL);
		}
	}

	in.Close();

	if (ret) {
		sprintf_s(
			m_pszCurFile,
			sizeof(m_pszCurFile),
			"%s\0",
			lpszName);
	}
	else {
		memset(m_pszCurFile, 0, sizeof(m_pszCurFile));
	}

	IZ_ASSERT(ret);
	return ret;
}

// シーン描画開始
IZ_BOOL CPostEffectSample::BeginScene()
{
	IZ_BOOL ret = IZ_TRUE;

	if (EnablePostEffect()) {
		izanagi::CSurface* surfs[2];
		IZ_UINT nSurfNum = 0;

		surfs[nSurfNum++] = m_pSrcTex->GetSurface(0);

		IZ_BOOL ret = m_pDevice->BeginScene(
						surfs,
						nSurfNum,
						0,
						0, 1.0f, 0);
	}

	return ret;
}

// ポストエフェクト実行
IZ_BOOL CPostEffectSample::Apply()
{
	IZ_BOOL ret = IZ_TRUE;

	if (EnablePostEffect()) {
		IZ_UINT nEndSceneTargetFlag = izanagi::E_GRAPH_END_SCENE_FLAG_RT_0;

		m_pDevice->EndScene(nEndSceneTargetFlag);

		ret = m_pPostEffect->Apply(
				m_pSrcTex,
				m_nCurPostEffectTech);
	}

	return ret;
}
