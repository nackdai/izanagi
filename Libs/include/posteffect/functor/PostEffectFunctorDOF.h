#if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_DOF_H__)
#define __IZANAGI_POSTEFFECT_FUNCTOR_DOF_H__

#if 0
#include "PostEffectFunctorDefault.h"

namespace izanagi {
	/**
	* DOF マージ処理
	* (DOF = Depth Of Field)
	*/
	class CPostEffectFunctorDOFMerge : public CPostEffectFunctorDefault {
		friend class CPostEffectFunctor;

	protected:
		inline CPostEffectFunctorDOFMerge();
		inline ~CPostEffectFunctorDOFMerge();

		CPostEffectFunctorDOFMerge(const CPostEffectFunctorDOFMerge& rhs);
		const CPostEffectFunctorDOFMerge& operator=(const CPostEffectFunctorDOFMerge& rhs);

	protected:
		// ファンクタ独自のテクスチャ作成
		IZ_BOOL CreateTexture(
			CPostEffect* pPostEffect,
			CPostEffectShader* pShader,
			IPostEffectTextureCreator* pTexCreator,
			CGraphicsDevice* pDevice);

		IZ_BOOL Apply(
			CPostEffectParamContainer* pParamContainer,
			CGraphicsDevice* pDevice,
			CPostEffectShader* pShader);

	protected:
		// マージ処理
		IZ_BOOL ApplyMerge(
			CPostEffectParamContainer* pParamContainer,
			CGraphicsDevice* pDevice,
			CPostEffectShader* pShader);

		// マージ処理開始前処理
		IZ_BOOL BeginApplyMerge(
			CPostEffectParamContainer* pParamContainer,
			CGraphicsDevice* pDevice,
			CPostEffectShader* pShader);

		// マージ処理終了後処理
		inline IZ_BOOL EndApplyMerge(
			CPostEffectParamContainer* pParamContainer,
			CGraphicsDevice* pDevice,
			CPostEffectShader* pShader);

	protected:
		IZ_INT m_nWeightTexPos;

		// ウエイトテクスチャ（ダブルバッファ）
		CTexture* m_pWeightTex[2];

		// 0 -> RGB
		// 1 -> A
		IZ_BOOL m_bColorWriteEnable[2];
	};

	// inline ***********************************

	// コンストラクタ
	CPostEffectFunctorDOFMerge::CPostEffectFunctorDOFMerge()
	{
		m_nWeightTexPos = 0;
		m_pWeightTex[0] = m_pWeightTex[1] = IZ_NULL;
	}

	// デストラクタ
	CPostEffectFunctorDOFMerge::~CPostEffectFunctorDOFMerge()
	{
		SAFE_RELEASE(m_pWeightTex[0]);
		SAFE_RELEASE(m_pWeightTex[1]);
	}

	// マージ処理終了後処理
	IZ_BOOL CPostEffectFunctorDOFMerge::EndApplyMerge(
		CPostEffectParamContainer* pParamContainer,
		CGraphicsDevice* pDevice,
		CPostEffectShader* pShader)
	{
		UNUSED_ALWAYS(pParamContainer);
		UNUSED_ALWAYS(pShader);

		// 使用するテクスチャを変更
		m_nWeightTexPos = 1 - m_nWeightTexPos;

		// カラーバッファ描き込み設定を戻す
		CGraphicsDeviceUtil::EnableRenderColorRGB(pDevice, m_bColorWriteEnable[0]);
		CGraphicsDeviceUtil::EnableRenderColorA(pDevice, m_bColorWriteEnable[1]);

		return IZ_TRUE;
	}
}	// namespace izanagi
#endif

#endif	// #if !defined(__IZANAGI_POSTEFFECT_FUNCTOR_DOF_H__)
