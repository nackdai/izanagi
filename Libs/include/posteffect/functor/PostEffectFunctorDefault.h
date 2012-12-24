#if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_DEFAULT_H__)
#define __IZANAGI_POSTEFFECT_FUNCTOR_DEFAULT_H__

#include "PostEffectFunctor.h"

namespace izanagi {
    /**
    * デフォルトファンクタ
    */
    class CPostEffectFunctorDefault : public CPostEffectFunctor {
        friend class CPostEffectFunctor;

    protected:
        CPostEffectFunctorDefault() {}
        ~CPostEffectFunctorDefault() {}

        NO_COPIABLE(CPostEffectFunctorDefault);

    protected:
        IZ_BOOL Apply(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader);

        virtual IZ_BOOL ApplyCore(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader);

        // 処理開始前処理
        virtual IZ_BOOL BeginApply(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader)
        {
            return IZ_TRUE;
        }

        // 処理終了後処理
        virtual IZ_BOOL EndApply(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader)
        {
            return IZ_TRUE;
        }

        // テクスチャオフセットをセット
        IZ_BOOL SetTextureOffset(CPostEffectShader* pShader);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_DEFAULT_H__)
