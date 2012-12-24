#if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_RENDER_STAR_H__)
#define __IZANAGI_POSTEFFECT_FUNCTOR_RENDER_STAR_H__

#include "PostEffectFunctor.h"

namespace izanagi {
    /**
    * スター作成ファンクタ
    */
    class CPostEffectFunctorRenderStar : public CPostEffectFunctor {
        friend class CPostEffectFunctor;

    protected:
        inline CPostEffectFunctorRenderStar();
        ~CPostEffectFunctorRenderStar();

        NO_COPIABLE(CPostEffectFunctorRenderStar);

    protected:
        enum {
            MAX_PASS = 3,
            SAMPLE_NUM = 8,
        };

        // スターの種類
        enum STAR_TYPE {
            STAR_TYPE_DISABLE = 0,
            STAR_TYPE_CROSS,
            STAR_TYPE_CROSSFILTER,
            STAR_TYPE_SNOWCROSS,
            STAR_TYPE_VERTICAL,

            STAR_TYPE_NUM,
        };

        // スターデータ
        struct SStarData {
            IZ_INT nStarLines;
            IZ_INT nPasses;
            IZ_FLOAT fSampleLength;
            IZ_FLOAT fAttenuation;
            IZ_FLOAT fInclination;
            IZ_UINT bRotation;
        };

        static const SStarData STAR_DATA[STAR_TYPE_NUM];

    protected:
        IZ_BOOL Apply(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader);

        // ファンクタ独自のテクスチャ作成
        IZ_BOOL CreateTexture(
            CPostEffect* pPostEffect,
            CPostEffectShader* pShader,
            CPostEffectTextureCreator* pTexCreator,
            graph::CGraphicsDevice* pDevice);

    protected:
        // スター出力テクスチャ取得
        graph::CTexture* GetStarDstTex(CPostEffectShader* pShader);

        // 使用テクスチャ数取得
        IZ_UINT GetUseTexNum();

        // アルファブレンドステート設定
        void SetAlphaBlendState(
            CPostEffectShader* pShader,
            IZ_BOOL enable, IZ_UINT method);

    protected:
        // ワークテクスチャ取得
        inline graph::CTexture* GetWorkTex();
        
        // ワークテクスチャ切り替え
        inline void ToggleWorkTex();

    protected:
        enum {
            WORK_TEX_NUM = 2,   // スター作成ワークテクスチャの数
            LINE_TEX_NUM = 4,   // スターの線テクスチャの数

            // スター用テクスチャの数 -> 2 + 4 = 6
            TEX_NUM = WORK_TEX_NUM + LINE_TEX_NUM,

            // スターの線テクスチャの開始位置
            LINE_TEX_POS = 2,
        };


    protected:
        graph::CTexture* m_pWorkTex[TEX_NUM];

        // ワークテクスチャインデックス
        IZ_INT m_nWorkTexIdx;

        IZ_POSTEFFECT_HANDLE m_hStarWeight;
        IZ_POSTEFFECT_HANDLE m_hStarMergeWeight;
    };

    // inline ******************************

    // コンストラクタ
    CPostEffectFunctorRenderStar::CPostEffectFunctorRenderStar()
    {
        FILL_ZERO(m_pWorkTex, sizeof(m_pWorkTex));
        m_nWorkTexIdx = 0;

        m_hStarWeight = IZ_NULL;
        m_hStarMergeWeight = IZ_NULL;
    }

    // ワークテクスチャ取得
    graph::CTexture* CPostEffectFunctorRenderStar::GetWorkTex()
    {
        return m_pWorkTex[m_nWorkTexIdx];
    }
        
    // ワークテクスチャ切り替え
    void CPostEffectFunctorRenderStar::ToggleWorkTex()
    {
        // NOTE
        // 0,1を切り替える
        m_nWorkTexIdx = 1 - m_nWorkTexIdx;
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_RENDER_STAR_H__)
