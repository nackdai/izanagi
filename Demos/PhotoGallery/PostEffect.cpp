#include "PostEffect.h"

PostEffect PostEffect::instance;

PostEffect& PostEffect::Instance()
{
    return instance;
}

// コンストラクタ
PostEffect::PostEffect()
{
    m_System = IZ_NULL;
    m_PostEffect = IZ_NULL;
    m_TexCreator = IZ_NULL;

    m_SrcTex = IZ_NULL;

    m_Enable = IZ_TRUE;

    m_CurTechIdx = 0;
}

// デストラクタ
PostEffect::~PostEffect()
{
}

// 初期化
IZ_BOOL PostEffect::Init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    static const IZ_UINT DefaultTexMgrCreateMax = 32;

    IZ_BOOL ret = IZ_TRUE;

    // ポストエフェクトシステム作成
    // 全てのポストエフェクトの元になる
    // 必ず最初に作成すること
    m_System = izanagi::CPostEffectSystem::CreatePostEffectSystem(
                            allocator,
                            device);
    VGOTO(
        ret = (m_System != IZ_NULL),
        __EXIT__);

    if (ret) {
        // テクスチャクリエータ作成
        // ポストエフェクトで使用されるテクスチャは、テクスチャクリエータを通じて作成、管理される
        m_TexCreator = izanagi::CPostEffectTextureCreator::CreatePostEffectTextureCreator(
            allocator,
            device,
            DefaultTexMgrCreateMax);    // 管理可能テクスチャ最大数
        VGOTO(
            ret = (m_TexCreator != IZ_NULL),
            __EXIT__);

        if (ret) {
            // システムにセット
            m_System->SetTextureCreator(m_TexCreator);
        }
    }

__EXIT__:
    if (!ret)
    {
        Terminate();
    }
    
    return ret;
}

// 開放
void PostEffect::Terminate()
{
    SAFE_RELEASE(m_PostEffect);
    SAFE_RELEASE(m_System);
    SAFE_RELEASE(m_TexCreator);

    SAFE_RELEASE(m_SrcTex);
}

// ポストエフェクト読み込み
IZ_BOOL PostEffect::Read(
    IZ_PCSTR filapath,
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    IZ_BOOL ret = IZ_FALSE;

    izanagi::CFileInputStream in;
    if (!in.Open(filapath)) {
        IZ_ASSERT(IZ_FALSE);
        return IZ_FALSE;
    }

    SAFE_RELEASE(m_PostEffect);

    if (m_System != IZ_NULL) {
        m_System->GetTextureCreator()->ClearAll();

        // ポストエフェクト作成
        m_PostEffect = m_System->CreatePostEffect(device, &in);
        ret = (m_PostEffect != IZ_NULL);

        if (ret) {
            // 入力シーンテクスチャ取得
            SAFE_REPLACE(
                m_SrcTex,
                m_PostEffect->GetInputSceneTex());

            ret = (m_SrcTex != IZ_NULL);
        }
    }

    in.Close();

    IZ_ASSERT(ret);
    return ret;
}

// シーン描画開始
IZ_BOOL PostEffect::BeginScene(izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL ret = IZ_TRUE;

    if (EnablePostEffect()) {
        izanagi::graph::CRenderTarget* rt[2];
        IZ_UINT rtNum = 0;

        rt[rtNum] = m_SrcTex->AsRenderTarget();
        IZ_ASSERT(rt[rtNum] != IZ_NULL);

        rtNum++;

        IZ_BOOL ret = device->BeginScene(
            rt,
            rtNum,
            0,
            0, 1.0f, 0);
    }

    return ret;
}

// ポストエフェクト実行
IZ_BOOL PostEffect::Apply(izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL ret = IZ_TRUE;

    if (EnablePostEffect()) {
        IZ_UINT endSceneTargetFlag = izanagi::graph::E_GRAPH_END_SCENE_FLAG_RT_0;

        device->EndScene(endSceneTargetFlag);

        ret = m_PostEffect->Apply(
            device,
            m_SrcTex,
            m_CurTechIdx);
    }

    return ret;
}

// ポストエフェクト実行可能かどうか
IZ_BOOL PostEffect::EnablePostEffect()
{
    return (m_Enable && (m_PostEffect != IZ_NULL));
}

// ポストエフェクト実行フラグ切り替え
void PostEffect::ToggleEnablePostEffect()
{
    m_Enable = !m_Enable;
}

// ポストエフェクトテクニック切り替え
void PostEffect::ChangePostEffectTechnique(IZ_UINT techIdx)
{
    if (m_PostEffect != IZ_NULL) {
        IZ_UINT techNum = m_PostEffect->GetTechniqueNum();

        m_CurTechIdx = (techIdx > techNum ? techNum - 1 : techIdx);
    }
}

// テクニック数取得
IZ_UINT PostEffect::GetTechniqueNum() const
{
    IZ_ASSERT(m_PostEffect != IZ_NULL);
    return m_PostEffect->GetTechniqueNum();
}

// 現在のポストエフェクトテクニック取得
IZ_UINT PostEffect::GetCurrentPostEffectTechnique() const
{
    return m_CurTechIdx;
}

izanagi::CPostEffect* PostEffect::GetPostEffect()
{
    return m_PostEffect;
}
