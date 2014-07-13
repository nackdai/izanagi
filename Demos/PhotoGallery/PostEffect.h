#if !defined(__POSTEFFECT_H__)
#define __POSTEFFECT_H__

#include "izPostEffect.h"

class PostEffect {
private:
    static PostEffect instance;

public:
    static PostEffect& Instance();

private:
    PostEffect();
    ~PostEffect();

    NO_COPIABLE(PostEffect);

public:
    // 初期化
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    // 開放
    void Terminate();

    // ポストエフェクト読み込み
    IZ_BOOL Read(
        IZ_PCSTR filapath,
        izanagi::graph::CGraphicsDevice* device);

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

    izanagi::CPostEffect* GetPostEffect();

private:    
    izanagi::CPostEffectSystem* m_System;
    izanagi::CPostEffect* m_PostEffect;
    izanagi::CPostEffectTextureCreator* m_TexCreator;

    izanagi::graph::CTexture* m_SrcTex;

    IZ_BOOL m_Enable;

    IZ_UINT m_CurTechIdx;
};

#endif    // #if !defined(__POSTEFFECT_H__)
