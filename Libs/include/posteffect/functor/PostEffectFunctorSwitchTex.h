#if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_SWITCH_TEX_H__)
#define __IZANAGI_POSTEFFECT_FUNCTOR_SWITCH_TEX_H__

#include "./PostEffectFunctorDefault.h"

namespace izanagi {
	/**
	* テクスチャ切り替えファンクタ
	*/
	class CPostEffectFunctorSwitchTex : public CPostEffectFunctorDefault {
		friend class CPostEffectFunctor;

	protected:
		inline CPostEffectFunctorSwitchTex();
		~CPostEffectFunctorSwitchTex() {}

		NO_COPIABLE(CPostEffectFunctorSwitchTex);

	protected:
		IZ_BOOL ApplyCore(
			graph::CGraphicsDevice* pDevice,
			CPostEffectShader* pShader);

		// 処理終了後処理
		inline IZ_BOOL EndApply(
			graph::CGraphicsDevice* pDevice,
			CPostEffectShader* pShader);

		// リセット
		inline void Reset();

		// ファンクタ引数セット
		inline void SetFunctorArgs(const IZ_FLOAT* pArgs);

	protected:
		IZ_INT m_nSwapTexIdx[2];
		IZ_BOOL m_bSwitch;
	};

	// inline ***********************************

	// コンストラクタ
	CPostEffectFunctorSwitchTex::CPostEffectFunctorSwitchTex()
	{
		m_nSwapTexIdx[0] = -1;
		m_nSwapTexIdx[1] = -1;
		
		Reset();
	}

	// 処理終了後処理
	IZ_BOOL CPostEffectFunctorSwitchTex::EndApply(
		graph::CGraphicsDevice* pDevice,
		CPostEffectShader* pShader)
	{
		IZ_UINT tmp = m_nSwapTexIdx[0];
		m_nSwapTexIdx[0] = m_nSwapTexIdx[1];
		m_nSwapTexIdx[1] = tmp;

		m_bSwitch = IZ_TRUE;
		
		return IZ_TRUE;
	}

	// リセット
	void CPostEffectFunctorSwitchTex::Reset()
	{
		m_bSwitch = IZ_FALSE;
	}

	// ファンクタ引数セット
	void CPostEffectFunctorSwitchTex::SetFunctorArgs(const IZ_FLOAT* pArgs)
	{
		IZ_ASSERT(pArgs != IZ_NULL);
		m_nSwapTexIdx[0] = (IZ_INT)pArgs[0];
		m_nSwapTexIdx[1] = (IZ_INT)pArgs[1];
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_SWITCH_TEX_H__)
