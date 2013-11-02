#if !defined(__POSTEFFECT_SAMPLE_H__)
#define __POSTEFFECT_SAMPLE_H__

#include "izPostEffect.h"

class CPostEffectSample {
private:
    static CPostEffectSample s_Instance;

public:
    static inline CPostEffectSample* GetInstance() { return &s_Instance; }

private:
    CPostEffectSample();
    ~CPostEffectSample();

    NO_COPIABLE(CPostEffectSample);

public:
    // 初期化
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    // 開放
    void Release();

    // ポストエフェクト読み込み
    IZ_BOOL Read(IZ_PCSTR filapath);

    // シーン描画開始
    IZ_BOOL BeginScene(izanagi::graph::CGraphicsDevice* device);

    // ポストエフェクト実行
    IZ_BOOL Apply(izanagi::graph::CGraphicsDevice* device);

    // 深度テクスチャセット
    void SetDepthTexture(izanagi::graph::CTexture* pTex);

    // ポストエフェクト実行可能かどうか
    IZ_BOOL EnablePostEffect();

    // ポストエフェクト実行フラグ切り替え
    void ToggleEnablePostEffect();

    // ポストエフェクトテクニック切り替え
    void ChangePostEffectTechnique(IZ_UINT techIdx);

    // テクニック数取得
    IZ_UINT GetTechniqueNum() const;

    // 現在のポストエフェクトテクニック取得
    IZ_UINT GetCurrentPostEffectTechnique() const;

    izanagi::CPostEffect* GetPostEffect() { return m_PostEffect; }

private:
    izanagi::graph::CGraphicsDevice* m_Device;
    
    izanagi::CPostEffectSystem* m_System;
    izanagi::CPostEffect* m_PostEffect;
    izanagi::CPostEffectTextureCreator* m_TexCreator;

    izanagi::graph::CTexture* m_SrcTex;

    IZ_BOOL m_Enable;

    IZ_UINT m_CurTechIdx;
};

#endif    // #if !defined(__POSTEFFECT_SAMPLE_H__)
