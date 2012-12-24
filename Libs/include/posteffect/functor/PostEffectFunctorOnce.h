#if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_ONCE_H__)
#define __IZANAGI_POSTEFFECT_FUNCTOR_ONCE_H__

#include "./PostEffectFunctorDefault.h"

namespace izanagi {
    /**
    * 一度しか呼ばれないファンクタ
    */
    class CPostEffectFunctorOnce : public CPostEffectFunctorDefault {
        friend class CPostEffectFunctor;

    protected:
        inline CPostEffectFunctorOnce();
        ~CPostEffectFunctorOnce() {}

        NO_COPIABLE(CPostEffectFunctorOnce);

    protected:
        // 処理開始前処理
        inline IZ_BOOL BeginApply(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader);

        // 処理終了後処理
        inline IZ_BOOL EndApply(
            graph::CGraphicsDevice* pDevice,
            CPostEffectShader* pShader);

        // リセット
        inline void Reset();

    protected:
        // 呼ばれたかどうかフラグ
        IZ_BOOL m_bIsCall;
    };

    // inline ***********************************

    // コンストラクタ
    CPostEffectFunctorOnce::CPostEffectFunctorOnce()
    {
        Reset();
    }

    // 処理開始前処理
    IZ_BOOL CPostEffectFunctorOnce::BeginApply(
        graph::CGraphicsDevice* pDevice,
        CPostEffectShader* pShader)
    {
        UNUSED_ALWAYS(pDevice);
        UNUSED_ALWAYS(pShader);

        // NOTE
        // １度呼ばれたら、何もしないようにする
        return !m_bIsCall;
    }

    // 処理終了後処理
    IZ_BOOL CPostEffectFunctorOnce::EndApply(
        graph::CGraphicsDevice* pDevice,
        CPostEffectShader* pShader)
    {
        UNUSED_ALWAYS(pDevice);
        UNUSED_ALWAYS(pShader);

        // 呼ばれたのでフラグを立てる
        m_bIsCall = IZ_TRUE;

        return IZ_TRUE;
    }

    // リセット
    void CPostEffectFunctorOnce::Reset()
    {
        m_bIsCall = IZ_FALSE;
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_ONCE_H__)
