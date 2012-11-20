#include "posteffect/functor/PostEffectFunctorRenderStar.h"
#include "posteffect/PostEffectShader.h"
#include "posteffect/PostEffectTextureCreator.h"

using namespace izanagi;

// デストラクタ
CPostEffectFunctorRenderStar::~CPostEffectFunctorRenderStar()
{
	for (IZ_UINT i = 0; i < TEX_NUM; ++i) {
		SAFE_RELEASE(m_pWorkTex[i]);
	}
}

// スター出力テクスチャ取得
graph::CTexture* CPostEffectFunctorRenderStar::GetStarDstTex(CPostEffectShader* pShader)
{
	IZ_INT nTexIdx = pShader->GetDstTexIdx(m_nTechIdx, m_nPassIdx);
	IZ_ASSERT(nTexIdx > 0);

	graph::CTexture* ret = pShader->GetTexture(nTexIdx);

	return ret;
}

// 使用テクスチャ数取得
IZ_UINT CPostEffectFunctorRenderStar::GetUseTexNum()
{
	return WORK_TEX_NUM;
}

// アルファブレンドステート設定
void CPostEffectFunctorRenderStar::SetAlphaBlendState(
	CPostEffectShader* pShader,
	IZ_BOOL enable, IZ_UINT method)
{
	pShader->SetAlphaBlendState(enable, method);
}

#define STAR_INCLINATION			IZ_DEG2RAD(25.0f)
#define CHROMATIC_ABERRATION_RATIO	0.5f

const CPostEffectFunctorRenderStar::SStarData CPostEffectFunctorRenderStar::STAR_DATA[CPostEffectFunctorRenderStar::STAR_TYPE_NUM] = {
	{0,	0, 0.0f,  0.0f,  IZ_DEG2RAD(0.0f), IZ_FALSE,},	// STAR_TYPE_DISABLE
	//{4,	3, 1.0f, 0.85f,	IZ_DEG2RAD(0.0f), IZ_TRUE,},	// STAR_TYPE_CROSS
	{4,	3, 1.0f, 0.95f,	IZ_DEG2RAD(0.0f), IZ_TRUE,},	// STAR_TYPE_CROSS
	{4,	3, 1.0f, 0.95f,	IZ_DEG2RAD(0.0f), IZ_TRUE,},	// STAR_TYPE_CROSSFILTER
	{4,	3, 1.0f, 0.96f,	IZ_DEG2RAD(2.0f), IZ_TRUE,},	// STAR_TYPE_SNOWCROSS
	{2,	3, 1.0f, 0.96f,	IZ_DEG2RAD(0.0f), IZ_FALSE,},	// STAR_TYPE_VERTICAL
};

IZ_BOOL CPostEffectFunctorRenderStar::Apply(
	graph::CGraphicsDevice* pDevice,
	CPostEffectShader* pShader)
{
	IZ_ASSERT(pShader != IZ_NULL);
	UNUSED_ALWAYS(pDevice);

	// 光芒の色の計算用
	static const SVector vColWhite = {0.63f, 0.63f, 0.63f, 0.0f};

	static const SVector vChromaticAberrationTable[SAMPLE_NUM] = {
		{0.5f, 0.5f, 0.5f, 0.0f},	// 白
		{0.8f, 0.3f, 0.3f, 0.0f},	// 赤
		{1.0f, 0.2f, 0.2f, 0.0f},	// 赤
		{0.5f, 0.2f, 0.6f, 0.0f},	// 紫
		{0.2f, 0.2f, 1.0f, 0.0f},	// 青
		{0.2f, 0.3f, 0.7f, 0.0f},	// 青
		{0.2f, 0.6f, 0.2f, 0.0f},	// 緑
		{0.3f, 0.5f, 0.3f, 0.0f},	// 緑
	};

	if (m_hStarWeight == IZ_NULL) {
		m_hStarWeight = pShader->GetParamHandleByName("g_vStarWeight");
		IZ_ASSERT(m_hStarWeight != IZ_NULL);
	}
	if (m_hStarMergeWeight == IZ_NULL) {
		m_hStarMergeWeight = pShader->GetParamHandleByName("g_fStarMergeWeight");
		IZ_ASSERT(m_hStarMergeWeight != IZ_NULL);
	}

	// TODO
	// STAR_TYPE_CROSS固定
	const SStarData& sStarData = STAR_DATA[STAR_TYPE_CROSS];
	IZ_ASSERT(MAX_PASS >= sStarData.nPasses);

	// 光芒の色
	SVector vColor[MAX_PASS][SAMPLE_NUM];

	// 中心からの距離に応じて光芒の色をつける
	for (IZ_INT nPass = 0; nPass < MAX_PASS; ++nPass) {
		IZ_FLOAT fRatio = (IZ_FLOAT)(nPass + 1) / MAX_PASS;

		// そろぞれのサンプリングで適当に色をつける
		for (IZ_INT i = 0; i < SAMPLE_NUM; ++i) {
			SVector::Lerp(
				vColor[nPass][i],
				vChromaticAberrationTable[i],
				vColWhite,
				fRatio);
			
			// 全体的な色の変化を調整する
			SVector::Lerp(
				vColor[nPass][i],
				vColWhite,
				vColor[nPass][i],
				CHROMATIC_ABERRATION_RATIO);
		}
	}

	IZ_BOOL result = IZ_TRUE;

	// テクスチャインデックス
	IZ_INT nTexIdx = pShader->GetSrcTexIdx(
						0,
						m_nTechIdx,
						m_nPassIdx);
	IZ_ASSERT(nTexIdx > 0);

	// Get parameter handle for texture.
	IZ_POSTEFFECT_HANDLE hTexHandle = pShader->GetTexHandleByIdx(nTexIdx);

	graph::CTexture* pSrcTex = pShader->GetTexture(nTexIdx);
	IZ_ASSERT(pSrcTex != IZ_NULL);

	// 元の画像のサイズ
	IZ_INT nSrcWidth = pSrcTex->GetWidth();
	IZ_INT nSrcHeight = pSrcTex->GetHeight();

	// 回転オフセット = グレアの回転 + スターの回転
	IZ_FLOAT fRadOffset = STAR_INCLINATION + sStarData.fInclination;

	// スターの線の数
	IZ_INT nStarLines = sStarData.nStarLines;

	// スター描画用ウエイト値
	IZ_FLOAT fStarWeight = 1.0f / (IZ_FLOAT)nStarLines;

	// TODO
	IZ_FLOAT fTanFov = atanf(IZ_MATH_PI / 8);
	IZ_FLOAT fScreenWidthQuarter = 1280 / 4;
	IZ_FLOAT fScreenHeightQuarter = 720 / 4;

	SVector vOffsets[SAMPLE_NUM];
	SVector vWeights[SAMPLE_NUM];

	static char tmp[64];

	for (IZ_INT nLine = 0; nLine < nStarLines; ++nLine) {
		graph::CTexture* pTexSource = pSrcTex;

		// 各スターの線ごとの傾き
		IZ_FLOAT fRad = fRadOffset + nLine * IZ_MATH_PI2 / nStarLines;

		IZ_FLOAT fSin, fCos;
		CMath::GetSinCosF(fRad, &fSin, &fCos);

		SVector vStepUV;
		vStepUV.x = 0.3f * fSin / nSrcWidth * sStarData.fSampleLength;
		vStepUV.y = 0.3f * fCos / nSrcHeight * sStarData.fSampleLength;

		// よく分からんが、こういうもんらしい・・・
		IZ_FLOAT fAttnPowScale = (fTanFov + 0.1f) * (fScreenWidthQuarter + fScreenHeightQuarter) / (nSrcWidth + nSrcHeight) * 1.2f;

		// ワークテクスチャインデックス初期化
		m_nWorkTexIdx = 0;

		for (IZ_INT nPass = 0; nPass < sStarData.nPasses; ++nPass) {
			sprintf_s(tmp, sizeof(tmp), "passStar_%d_%d\0", nLine, nPass);
			CGraphPerf::BeginEvent(0, tmp);

			// 描画先の決定
			graph::CTexture* pDst = IZ_NULL;
			{
				if (sStarData.nPasses - 1 == nPass) {
					// 最後だけは、別のバッファを使う
					// ここに結果(スターの線)が書き込まれる
					pDst = GetStarDstTex(pShader);
				}
				else {
					pDst = GetWorkTex();
				}

				IZ_ASSERT(pDst != IZ_NULL);
				//IZ_ASSERT(pDst != pTexSource);
			}

			for (IZ_INT nSample = 0; nSample < SAMPLE_NUM; ++nSample) {
				// それぞれの重み
				IZ_FLOAT fLum = powf(sStarData.fAttenuation, fAttnPowScale * nSample);
	            
				// 色を変化させるための重み
				SVector::Scale(
					vWeights[nSample],
					vColor[sStarData.nPasses - 1 - nPass][nSample],
					fLum * (nPass + 1.0f) * 0.5f);

				// テクスチャ座標をずらす量
				IZ_UINT pos = nSample & 0x01;
				vOffsets[nSample >> 1].v[pos * 2 + 0] = vStepUV.x * nSample;
				vOffsets[nSample >> 1].v[pos * 2 + 1] = vStepUV.y * nSample;
			
				if ((fabs(vOffsets[nSample >> 1].v[pos]) >= 0.9f)
					|| (fabs(vOffsets[nSample >> 1].v[pos + 1]) >= 0.9f))
				{
					vOffsets[nSample].v[pos] = 0.0f;
					vOffsets[nSample].v[pos + 1] = 0.0f;

					SVector::SetZero(vWeights[nSample]);
				}
			}	// end of for

			// 描画開始
			{
				IZ_BOOL bIsClear = IZ_TRUE;

				if (sStarData.nPasses - 1 == nPass) {
					// 最初の線のときだけカラーバッファをクリアすればよい
					bIsClear = (nLine == 0 ? IZ_TRUE : IZ_FALSE);
				}

				// 加算ブレンドを強制セットする
				SetAlphaBlendState(
					pShader,
					IZ_TRUE,
					graph::E_GRAPH_ALPHA_BLEND_Cs_Cd);

				result = pShader->BeginRender(
							this,
							m_nTechIdx,
							m_nPassIdx,
							pDst,
							bIsClear);
				VRETURN(result);
			}

			// パラメータセット
			{
				// 頂点シェーダ
				// サンプリングオフセット
				result = pShader->SetTextureOffsetParameter(
							vOffsets,
							SAMPLE_NUM >> 1);
				VRETURN(result);

				// ピクセルシェーダ

				// ブラーウエイト
				result = pShader->SetParameter(
							m_hStarWeight,
							vWeights,
							sizeof(vWeights));
				VRETURN(result);

				// マージウエイト
				IZ_FLOAT fMergeWeight = (sStarData.nPasses - 1 == nPass
											? fStarWeight
											: 1.0f);
				result = pShader->SetParameter(
							m_hStarMergeWeight,
							&fMergeWeight,
							sizeof(fMergeWeight));
				VRETURN(result);
			}

			// テクスチャセット
			result = pShader->SetTexture(
						hTexHandle, 
						pTexSource);
			VRETURN(result);
			
			// コミットチェンジ
			result = pShader->CommitChanges();
			VRETURN(result);

			// 描画
			result = pShader->Render(IZ_NULL, IZ_NULL);
			VRETURN(result);

			// 描画終了
			result = pShader->EndRender();
			VRETURN(result);

			// 次への準備
			{
				SVector::Scale(
					vStepUV,
					vStepUV,
					(IZ_FLOAT)SAMPLE_NUM);

				fAttnPowScale *= SAMPLE_NUM;

				// レンダリングした出力を次のテクスチャにする
				pTexSource = pDst;

				// ワークテクスチャ切り替え
				ToggleWorkTex();
			}

			CGraphPerf::EndEvent();
		}	// end of for
	}

	return IZ_TRUE;
}

// ファンクタ独自のテクスチャ作成
IZ_BOOL CPostEffectFunctorRenderStar::CreateTexture(
	CPostEffect* pPostEffect,
	CPostEffectShader* pShader,
	CPostEffectTextureCreator* pTexCreator,
	graph::CGraphicsDevice* pDevice)
{
	UNUSED_ALWAYS(pPostEffect);
	UNUSED_ALWAYS(pDevice);

	// メインのソーステクスチャ
	IZ_INT nTexIdx = pShader->GetSrcTexIdx(
						0,
						m_nTechIdx,
						m_nPassIdx);
	IZ_ASSERT(nTexIdx > 0);

	graph::CTexture* pSrcTex = pShader->GetTexture(nTexIdx);
	IZ_ASSERT(pSrcTex != IZ_NULL);

	IZ_UINT num = GetUseTexNum();

	for (IZ_UINT i = 0; i < num; ++i) {
		// 作成
		graph::CTexture* pTex = pTexCreator->Create(
							pSrcTex->GetWidth(),
							pSrcTex->GetHeight(),
							pSrcTex->GetPixelFormat(),
							IZ_TRUE,
							graph::E_GRAPH_RSC_TYPE_STATIC,	// RenderTargetなので何でもいい
							E_POSTEFFECT_TEXTURE_TYPE_PRIVATE);

		VRETURN(pTex != IZ_NULL);

		// セット
		SAFE_REPLACE(m_pWorkTex[i], pTex);
	}

	return IZ_TRUE;
}
