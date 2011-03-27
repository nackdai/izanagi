#if !defined(__POSTEFFECT_SAMPLE_H__)
#define __POSTEFFECT_SAMPLE_H__

#include "izPostEffect.h"

class CPostEffectSample {
private:
	static CPostEffectSample s_cInstance;

public:
	static inline CPostEffectSample& GetInstance() { return s_cInstance; }

private:
	CPostEffectSample();
	~CPostEffectSample();

	CPostEffectSample(const CPostEffectSample& rhs);
	const CPostEffectSample& operator=(const CPostEffectSample& rhs);

public:
	// 初期化
	IZ_BOOL Init(
		izanagi::IMemoryAllocator* pAllocator,
		izanagi::CGraphicsDevice* pDevice);

	// 開放
	void Release();

	// ポストエフェクト読み込み
	IZ_BOOL Read(LPCSTR lpszName);

	// シーン描画開始
	IZ_BOOL BeginScene();

	// ポストエフェクト実行
	IZ_BOOL Apply();

public :
	// 深度テクスチャセット
	inline void SetDepthTexture(izanagi::CTexture* pTex);

	inline IZ_BOOL EnablePostEffect();
	inline void ToggleEnablePostEffect();

	inline void ChangePostEffectTechnique(IZ_BOOL bIsAdd);
	inline IZ_UINT GetCurrentPostEffectTechnique();

	inline IZ_BOOL Reset();
	inline void OnLostDevice();

	izanagi::CPostEffect* GetPostEffect() { return m_pPostEffect; }

private:
	izanagi::IMemoryAllocator* m_pAllocator;
	izanagi::CGraphicsDevice* m_pDevice;

	izanagi::CPostEffectSystem* m_pPostEffectSystem;
	izanagi::CPostEffect* m_pPostEffect;
	izanagi::CPostEffectTextureCreator* m_pPostEffectTexCreator;

	izanagi::CTexture* m_pSrcTex;

	IZ_BOOL m_bEnablePostEffect;

	IZ_UINT m_nCurPostEffectTech;

	char m_pszCurFile[256];
};

// inline ********************************

// ポストエフェクト実行可能かどうか
IZ_BOOL CPostEffectSample::EnablePostEffect()
{
#if 0
	return m_bEnablePostEffect;
#else
	return (m_bEnablePostEffect && (m_pPostEffect != IZ_NULL));
#endif
}

// ポストエフェクト実行フラグ切り替え
void CPostEffectSample::ToggleEnablePostEffect()
{
	m_bEnablePostEffect = !m_bEnablePostEffect;
}

// ポストエフェクトテクニック切り替え
void CPostEffectSample::ChangePostEffectTechnique(IZ_BOOL bIsAdd)
{
	if (m_pPostEffect != IZ_NULL) {
		IZ_UINT nTechNum = m_pPostEffect->GetTechniqueNum();

		if (bIsAdd) {
			m_nCurPostEffectTech++;
			m_nCurPostEffectTech = (m_nCurPostEffectTech >= nTechNum
									? 0
									: m_nCurPostEffectTech);
		}
		else {
			m_nCurPostEffectTech = (m_nCurPostEffectTech == 0
									? nTechNum - 1
									: m_nCurPostEffectTech - 1);
		}
	}
}

// 現在のポストエフェクトテクニック取得
IZ_UINT CPostEffectSample::GetCurrentPostEffectTechnique()
{
	return m_nCurPostEffectTech;
}

IZ_BOOL CPostEffectSample::Reset()
{
	// シェーダを内部で持っているため、再作成＆再読み込みの必要がある

	IZ_BOOL ret = IZ_TRUE;

	m_pPostEffectSystem = izanagi::CPostEffectSystem::CreatePostEffectSystem(
							m_pAllocator,
							m_pDevice);
	ret = (m_pPostEffectSystem != IZ_NULL);

	if (ret) {
		m_pPostEffectSystem->SetTextureCreator(m_pPostEffectTexCreator);

		if (strlen(m_pszCurFile) > 0) {
			ret = Read(m_pszCurFile);
		}
	}
	
	return ret;
}

void CPostEffectSample::OnLostDevice()
{
	SAFE_RELEASE(m_pPostEffectSystem);
	SAFE_RELEASE(m_pPostEffect);
}

#endif	// #if !defined(__POSTEFFECT_SAMPLE_H__)
