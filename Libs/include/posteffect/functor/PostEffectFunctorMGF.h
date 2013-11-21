#if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_MGF_H__)
#define __IZANAGI_POSTEFFECT_FUNCTOR_MGF_H__

#include "PostEffectFunctorDefault.h"

namespace izanagi {
    /**
    * MGFファンクタ
    * （MGF = Multi Gauss Filter）
    *
    * ガウスフィルターを縮小しながら行っていき、
    * 最後にそれらをマージして広がりのあるブルームを作成する。
    */
    class CPostEffectFunctorMGF : public CPostEffectFunctorDefault {
        friend class CPostEffectFunctor;

    protected:
        inline CPostEffectFunctorMGF();
        inline ~CPostEffectFunctorMGF();

        NO_COPIABLE(CPostEffectFunctorMGF);

    protected:
        enum {
            // 処理回数
            MGF_LOOP_NUM = 5,
        };

        // ブルームステート
        enum MGF_BLOOM_STATE {
            MGF_BLOOM_STATE_INIT = 0,
            MGF_BLOOM_STATE_ITER,
            MGF_BLOOM_STATE_FIN,

            MGF_BLOOM_STATE_NUM,
        };

        // ステートリスト要素
        struct SMGFStateListItem {
            graph::CTexture* pSrc;
            graph::CTexture* pDst;

            IZ_INT nSrcTexIdx;
            IZ_INT nDstTexIdx;

            SMGFStateListItem()
            {
                pSrc = IZ_NULL;
                pDst = IZ_NULL;

                nSrcTexIdx = -1;
                nDstTexIdx = -1;
            }

            void Release()
            {
                SAFE_RELEASE(pSrc);
                SAFE_RELEASE(pDst);
            }
        };

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
        // 縮小
        IZ_BOOL ApplyMGFDownScale(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader);

        // ブルーム作成
        IZ_BOOL ApplyMGFRenderBloom(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader);

        // マージ
        IZ_BOOL ApplyMGFMerge(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader);

    protected:
        // ステートリスト
        SMGFStateListItem m_DownScaleState[MGF_LOOP_NUM];
        SMGFStateListItem m_BloomState[MGF_LOOP_NUM][MGF_BLOOM_STATE_NUM];
    };

    // inline *****************************************

    // コンストラクタ
    CPostEffectFunctorMGF::CPostEffectFunctorMGF()
    {
    }

    // デストラクタ
    CPostEffectFunctorMGF::~CPostEffectFunctorMGF()
    {
        for (IZ_UINT i = 0; i < MGF_LOOP_NUM; ++i) {
            m_DownScaleState[i].Release();
        }

        for (IZ_UINT i = 0; i < MGF_LOOP_NUM; ++i) {
            for (IZ_UINT n = 0; n < MGF_BLOOM_STATE_NUM; ++n) {
                m_BloomState[i][n].Release();
            }
        }
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_MGF_H__)
