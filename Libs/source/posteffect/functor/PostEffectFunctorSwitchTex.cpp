#include "posteffect/functor/PostEffectFunctorSwitchTex.h"
#include "posteffect/PostEffectShader.h"
#include "posteffect/PostEffectTextureCreator.h"

using namespace izanagi;

// 実行メイン処理
IZ_BOOL CPostEffectFunctorSwitchTex::ApplyCore(
	graph::CGraphicsDevice* pDevice,
	CPostEffectShader* pShader)
{
	IZ_ASSERT(m_nSwapTexIdx[0] >= 0);
	IZ_ASSERT(m_nSwapTexIdx[1] >= 0);

	IZ_BOOL ret = IZ_TRUE;

	// 描画開始
	if (pShader->BeginRender(this, GetTechIdx(), GetPassIdx())) {
		{
			IZ_POSTEFFECT_HANDLE nTexHandle_0 = pShader->GetTexHandleByIdx(m_nSwapTexIdx[0]);
			IZ_POSTEFFECT_HANDLE nTexHandle_1 = pShader->GetTexHandleByIdx(m_nSwapTexIdx[1]);

			graph::CTexture* pTex_0 = pShader->GetTexture(m_nSwapTexIdx[0]);
			graph::CTexture* pTex_1 = pShader->GetTexture(m_nSwapTexIdx[1]);

			// Swap
			if (m_bSwitch) {
				pShader->SetTexture(
					nTexHandle_0,
					pTex_1);
				pShader->SetTexture(
					nTexHandle_1,
					pTex_0);
			}
			else {
				pShader->SetTexture(
					nTexHandle_0,
					pTex_0);
				pShader->SetTexture(
					nTexHandle_1,
					pTex_1);
			}
		}

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

