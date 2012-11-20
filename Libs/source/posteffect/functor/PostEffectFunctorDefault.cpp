#include "posteffect/functor/PostEffectFunctorDefault.h"
#include "posteffect/PostEffectShader.h"

using namespace izanagi;

// 実行
IZ_BOOL CPostEffectFunctorDefault::Apply(
	graph::CGraphicsDevice* pDevice,
	CPostEffectShader* pShader)
{
	IZ_ASSERT(pShader != IZ_NULL);
	
	IZ_BOOL ret = IZ_TRUE;

	if (BeginApply(
			pDevice,
			pShader))
	{
		ret =  ApplyCore(
				pDevice,
				pShader);
		VRETURN(ret);

		ret = EndApply(
				pDevice,
				pShader);
		VRETURN(ret);
	}

	return ret;
}

// 実行メイン処理
IZ_BOOL CPostEffectFunctorDefault::ApplyCore(
	graph::CGraphicsDevice* pDevice,
	CPostEffectShader* pShader)
{
	IZ_BOOL ret = IZ_TRUE;

	// 描画開始
	if (pShader->BeginRender(this, GetTechIdx(), GetPassIdx())) {
		// テクスチャオフセットをセット
		ret = SetTextureOffset(pShader);
		VGOTO(ret, __EXIT__);
		
		// コミットチェンジ
		ret = pShader->CommitChanges();
		VGOTO(ret, __EXIT__);

		// 描画
		ret = pShader->Render(IZ_NULL, IZ_NULL);
		VGOTO(ret, __EXIT__);

		// 描画終了
		ret = pShader->EndRender();
		VGOTO(ret, __EXIT__);
	}

__EXIT__:
	return ret;
}

// テクスチャオフセットをセット
IZ_BOOL CPostEffectFunctorDefault::SetTextureOffset(CPostEffectShader* pShader)
{
	IZ_INT nTexIdx = pShader->GetSrcTexIdx(
						0,
						m_nTechIdx,
						m_nPassIdx);

	graph::CTexture* pTex = pShader->GetTexture(nTexIdx);
	
	IZ_BOOL ret = pShader->SetTextureOffsetParameter(pTex);
	return ret;
}
