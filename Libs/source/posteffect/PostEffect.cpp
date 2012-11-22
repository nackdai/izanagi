#include "posteffect/PostEffect.h"
#include "posteffect/PostEffectShader.h"
#include "posteffect/PostEffectSystem.h"
#include "posteffect/functor/PostEffectFunctor.h"

using namespace izanagi;

// インスタンス作成
CPostEffect* CPostEffect::CreatePostEffect(
	IMemoryAllocator* pAllocator,
	graph::CGraphicsDevice* pDevice,
	CPostEffectTextureCreator* pTexCreator,
	CPostEffectVSManager* pVSMgr,
	IInputStream* in)
{
	CPostEffectShader* pShader = CPostEffectShader::CreatePostEffectShader(
									pAllocator,
									pDevice,
									pVSMgr,
									in);
	VRETURN_NULL(pShader != IZ_NULL);

	CPostEffect* pInstance = IZ_NULL;

	size_t nSize = sizeof(CPostEffect);
	nSize += ComputeFunctorSize(pShader);

	IZ_UINT8* pBuffer = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nSize));
	IZ_BOOL result = (pBuffer != IZ_NULL);
	VGOTO(result, __EXIT__);

	IZ_UINT8* pBufTop = pBuffer;

	pInstance = new(pBuffer) CPostEffect;
	{
		pBuffer += sizeof(CPostEffect);

		pInstance->AddRef();
		pInstance->m_Allocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->m_pShader = pShader;
	}

	// ファンクタ作成
	pBuffer = pInstance->CreateFunctor(pBuffer);
	result = (pBuffer != IZ_NULL);
	VGOTO(result, __EXIT__);

	// チェック
	IZ_ASSERT(CStdUtil::GetPtrDistance(pBuffer, pBufTop) == nSize);

	// テクスチャ作成
	result = pInstance->CreateTexture(pTexCreator);
	VGOTO(result, __EXIT__);

__EXIT__:
	if (!result) {
		if (pInstance == IZ_NULL) {
			SAFE_RELEASE(pShader);
		}
		else {
			SAFE_RELEASE(pInstance);
		}
	}

	return pInstance;
}

// ファンクタサイズ計算
size_t CPostEffect::ComputeFunctorSize(CPostEffectShader* pShader)
{
	size_t ret = 0;
	IZ_UINT nPassNum = pShader->GetHeader().numPass;

	for (IZ_UINT i = 0; i < nPassNum; ++i) {
		const S_PES_PASS* pDesc = pShader->GetPassDesc(i);
		if (!pDesc->ann.isSubPass) {
			IZ_PCSTR pszFunctorName = pShader->GetString(pDesc->ann.posNameFunctor);

			ret += CPostEffectSystem::GetPostEffectSystem()->GetFunctorSize(pszFunctorName);
		}
	}

	return ret;
}

// コンストラクタ
CPostEffect::CPostEffect()
{
	m_Allocator = IZ_NULL;
	m_pDevice = IZ_NULL;

	m_pShader = IZ_NULL;

	m_nTechNum = 0;
	m_nEnableTechIdx = -1;

	m_ListItem.Init(this);
}

// デストラクタ
CPostEffect::~CPostEffect()
{
	SAFE_RELEASE(m_pDevice);

	SAFE_RELEASE(m_pShader);

	CStdList<CFunctorHashItem>::Item* pItem = m_FunctorHash.GetOrderTop();

	while (pItem != IZ_NULL) {
		CPostEffectFunctor* pFunctor = pItem->GetData()->GetData();

		pItem = pItem->GetNext();

		SAFE_RELEASE(pFunctor);
	}
}

// ファンクタ作成
IZ_UINT8* CPostEffect::CreateFunctor(IZ_UINT8* pBuffer)
{
	IZ_UINT nTechNum = m_pShader->GetHeader().numTech;

	IZ_INT nBaseFunctorPassIdx = -1;

	for (IZ_UINT nTechIdx = 0; nTechIdx < nTechNum; ++nTechIdx) {
		const S_PES_TECHNIQUE* pTechDesc = m_pShader->GetTechniqueDesc(nTechIdx);
		
		for (IZ_UINT nPassPos = 0; nPassPos < pTechDesc->numPass; ++nPassPos) {
			IZ_UINT nPassIdx = m_pShader->GetTechniqueTable().GetPassIdx(nTechIdx, nPassPos);

			const S_PES_PASS* pDesc = m_pShader->GetPassDesc(nPassIdx);

			if (pDesc->ann.isSubPass) {
				IZ_ASSERT(nBaseFunctorPassIdx >= 0);

				// サブパスとしてファンクタに登録する
				CPostEffectFunctor* pFunctor = m_FunctorHash.FindData(nBaseFunctorPassIdx);
				VRETURN_NULL(pFunctor != IZ_NULL);

				//pFunctor->SetConnectPassIdx(nPassIdx);
				pFunctor->SetConnectPassIdx(nPassPos);
			}
			else {
				// ファンクタ新規作成
				size_t nFunctorSize = 0;

				CPostEffectFunctor* pFunctor = CPostEffectSystem::GetPostEffectSystem()->CreateFunctor(
												pDesc->ann.keyFunctor,
												nPassIdx,
												pBuffer,
												&nFunctorSize);
				VRETURN_NULL(pFunctor != IZ_NULL);

				// NOTE
				// テクニック内でのパスインデックスを指定
				// nPassIdx は全パスを通してのインデックス
				pFunctor->Init(
					nTechIdx,
					nPassPos);

				// ファンクタ引数セット
				pFunctor->SetFunctorArgs(pDesc->ann.FunctorArgs);

				// 登録
				m_FunctorHash.Add(pFunctor->GetHashItem());

				pBuffer += nFunctorSize;

				nBaseFunctorPassIdx = nPassIdx;
			}
		}
	}

	return pBuffer;
}

// テクスチャ作成
IZ_BOOL CPostEffect::CreateTexture(CPostEffectTextureCreator* pTexCreator)
{
	IZ_ASSERT(m_pShader != IZ_NULL);

	static const graph::CTexture* tblCreatedTex[graph::TEX_STAGE_NUM];
	IZ_UINT nCreatedTexTblPos = 0;

	CStdList<CFunctorHashItem>::Item* pItem = m_FunctorHash.GetOrderTop();

	while (pItem != IZ_NULL) {
		// テクスチャ作成開始
		pTexCreator->BeginCreate();

		FILL_ZERO(tblCreatedTex, sizeof(tblCreatedTex));
		nCreatedTexTblPos = 0;

		CFunctorHashItem* pFunctorHashItem = pItem->GetData();
		CPostEffectFunctor* pFunctor = pFunctorHashItem->GetData();

		IZ_UINT nPassIdx = m_pShader->ConvertPassIdx(
							pFunctor->GetTechIdx(),
							pFunctor->GetPassIdx());

		// パス情報取得
		const S_PES_PASS* pPassDesc = m_pShader->GetPassDesc(nPassIdx);

#ifdef __IZ_DEBUG__
		IZ_PCSTR pszPassName = m_pShader->GetString(pPassDesc->posName);
#endif	// #ifdef __IZ_DEBUG__

		VRETURN(
			CreateTexture(
				pTexCreator,
				nPassIdx,
				nCreatedTexTblPos,
				tblCreatedTex));

		// For sub passes.
		for (IZ_UINT nSubPassListPos = 0; nSubPassListPos < PES_CONNECT_PASS_NUM; ++nSubPassListPos) {
			IZ_INT8 nSubPassIdx = pFunctor->GetConnectPassIdx(nSubPassListPos);
			if (nSubPassIdx < 0) {
				break;
			}

			VRETURN(
				CreateTexture(
					pTexCreator,
					nSubPassIdx,
					nCreatedTexTblPos,
					tblCreatedTex));
		}

		// ファンクタ独自テクスチャ作成
		VRETURN(
			pFunctor->CreateTexture(
				this,
				GetShader(),
				pTexCreator,
				m_pDevice));

		// テクスチャ作成終了
		pTexCreator->EndCreate();

		pItem = pItem->GetNext();
	}

	return IZ_TRUE;
}

IZ_BOOL CPostEffect::CreateTexture(
	CPostEffectTextureCreator* pTexCreator,
	IZ_UINT nPassIdx,
	IZ_UINT& nCreatedTexTblPos,
	const graph::CTexture* tblCreatedTex[])
{
	// パス情報取得
	const S_PES_PASS* pPassDesc = m_pShader->GetPassDesc(nPassIdx);

#ifdef __IZ_DEBUG__
	IZ_PCSTR pszPassName = m_pShader->GetString(pPassDesc->posName);
#endif	// #ifdef __IZ_DEBUG__

	IZ_INT nDstTexIdx = pPassDesc->ann.RenderTargetIdx;
	graph::CTexture* pDstTex = m_pShader->GetTexture(nDstTexIdx);

	// サンプラ数
	IZ_UINT nSmplNum = pPassDesc->numSampler;

	for (IZ_UINT nSmplPos = 0; nSmplPos < nSmplNum; ++nSmplPos) {
		IZ_UINT nSmplIdx = pPassDesc->GetSmplIdx(nSmplPos);

		// サンプラ情報取得
		const S_PES_SAMPLER* pSmplDesc = m_pShader->GetSamplerDesc(nSmplIdx);

		const IZ_UINT nTexIdx = pSmplDesc->state.BindTexIdx;

		// テクスチャ情報取得
		const S_PES_TEXTURE* pTexDesc = m_pShader->GetTexDesc(nTexIdx);

		if (pTexDesc->type == E_POSTEFFECT_TEXTURE_TYPE_INPUT) {
			// E_POSTEFFECT_TEXTURE_TYPE_INPUTは外部で作成されるので何もしない
			continue;
		}

#ifdef __IZ_DEBUG__
		IZ_PCSTR pszTexName = m_pShader->GetString(pTexDesc->posName);
#endif	// #ifdef __IZ_DEBUG__

		// テクスチャ有無確認
		graph::CTexture* pTex = m_pShader->GetTextureTable().GetTexture(nTexIdx);

		if (pTex == IZ_NULL) {
			// テクスチャ作成
			pTexCreator->SetEnableFind(IZ_TRUE);
			pTex = CreateTexture(
					pTexCreator,
					pTexDesc);
			VRETURN(pTex != IZ_NULL);

			if (pTex == pDstTex) {
				// 入力と出力が一致するので強制的に作り直し・・・
				pTexCreator->SetEnableFind(IZ_FALSE);
				pTex = CreateTexture(
						pTexCreator,
						pTexDesc);
				VRETURN(pTex != IZ_NULL);
			}

			// テクスチャ登録
			m_pShader->GetTextureTable().SetTexture(nTexIdx, pTex);
		}

		IZ_ASSERT(nCreatedTexTblPos < graph::TEX_STAGE_NUM);
		tblCreatedTex[nCreatedTexTblPos++] = pTex;
	}

	if ((nDstTexIdx >= 0)
		&& (pDstTex == IZ_NULL))
	{
		// 出力先がまだ存在しないので作成

		const S_PES_TEXTURE* pTexDesc = m_pShader->GetTexDesc(nDstTexIdx);

#ifdef __IZ_DEBUG__
		IZ_PCSTR pszTexName = m_pShader->GetString(pTexDesc->posName);
#endif	// #ifdef __IZ_DEBUG__

		if (pTexDesc->type != E_POSTEFFECT_TEXTURE_TYPE_INPUT) {
			// E_POSTEFFECT_TEXTURE_TYPE_INPUTは外部で作成されるので何もしない

			// テクスチャ作成
			pTexCreator->SetEnableFind(IZ_TRUE);
			graph::CTexture* pTex = CreateTexture(
								pTexCreator,
								pTexDesc);
			VRETURN(pTex != IZ_NULL);

			// 入力テクスチャと一致するか調べる
			for (IZ_UINT i = 0; i < nCreatedTexTblPos; ++i) {
				if (tblCreatedTex[i] == pTex) {
					// 一致するので、強制的に作る
					pTexCreator->SetEnableFind(IZ_FALSE);
					pTex = CreateTexture(
							pTexCreator,
							pTexDesc);
					VRETURN(pTex != IZ_NULL);

					break;
				}
			}

			// テクスチャ登録
			m_pShader->GetTextureTable().SetTexture(nDstTexIdx, pTex);
		}
	}

	return IZ_TRUE;
}

namespace {
	// テクスチャサイズを計算
	void _ComputeTexSize(
		const S_PES_TEXTURE_ANN& sTexAnn,
		IZ_INT& nRefTexWidth,
		IZ_INT& nRefTexHeight)
	{
		// 幅
		if (sTexAnn.ViewportRatio[0] > 0.0f) {
			nRefTexWidth = (IZ_INT)(nRefTexWidth * sTexAnn.ViewportRatio[0] + sTexAnn.Dimensions[0]);
		}
		else if (sTexAnn.Dimensions[0] > 0) {
			nRefTexWidth = sTexAnn.Dimensions[0];
		}

		// 高さ
		if (sTexAnn.ViewportRatio[1] > 0.0f) {
			nRefTexHeight = (IZ_INT)(nRefTexHeight * sTexAnn.ViewportRatio[1] + sTexAnn.Dimensions[1]);
		}
		else if (sTexAnn.Dimensions[1] > 0) {
			nRefTexHeight = sTexAnn.Dimensions[1];
		}
	}
}	// namespace

// テクスチャ作成
graph::CTexture* CPostEffect::CreateTexture(
	CPostEffectTextureCreator* pTexCreator,
	const S_PES_TEXTURE* pTexDesc)
{
	// テクスチャサイズ計算
	IZ_INT nWidth = m_pDevice->GetBackBufferWidth();
	IZ_INT nHeight = m_pDevice->GetBackBufferHeight();

	_ComputeTexSize(
		pTexDesc->ann,
		nWidth,
		nHeight);

	graph::E_GRAPH_RSC_TYPE nCreateType = (pTexDesc->ann.isDynamic
									? graph::E_GRAPH_RSC_TYPE_DYNAMIC 
									: graph::E_GRAPH_RSC_TYPE_STATIC);

	// テクスチャ作成
	graph::CTexture* pTex = pTexCreator->Create(
						nWidth,
						nHeight,
						pTexDesc->ann.fmt,
						pTexDesc->ann.isRenderTarget,
						pTexDesc->ann.typeRsc,
						pTexDesc->type);

	return pTex;
}

/**
* ポストエフェクト実行
*/
IZ_BOOL CPostEffect::Apply(
	graph::CTexture* pInputSceneTex,
	IZ_UINT nEnableTechIdx)
{
	IZ_ASSERT(m_pDevice != IZ_NULL);
	IZ_ASSERT(nEnableTechIdx < m_pShader->GetHeader().numTech);

	IZ_BOOL ret = IZ_FALSE;

	if (pInputSceneTex != IZ_NULL) {
		// 入力テクスチャ登録
		if (!m_pShader->GetTextureTable().HasTextureBySemantic("INPUT_SCENE")) {
			m_pShader->GetTextureTable().SetTextureBySemantic(
				"INPUT_SCENE", 
				pInputSceneTex);
		}
	}
	else {
		// 入力テクスチャの有無を確認
		VRETURN(m_pShader->GetTextureTable().HasTextureBySemantic("INPUT_SCENE"));
	}

	// レンダーステートの保持
	graph::S_RENDER_STATE sRS;
	memcpy(&sRS, &m_pDevice->GetRenderState(), sizeof(sRS));

	// Z
    m_pDevice->SetRenderState(
        graph::E_GRAPH_RS_ZWRITEENABLE,
        IZ_FALSE);
    m_pDevice->SetRenderState(
        graph::E_GRAPH_RS_ZENABLE,
        IZ_FALSE);

	const S_PES_TECHNIQUE* pTechDesc = m_pShader->GetTechniqueDesc(nEnableTechIdx);

	// パス数取得
	IZ_UINT nPassNum = pTechDesc->numPass;

	for (IZ_UINT i = 0; i < nPassNum; ++i) {
		IZ_UINT nPassIdx = m_pShader->GetTechniqueTable().GetPassIdx(
							nEnableTechIdx,
							i);

		const S_PES_PASS* pPassDesc = m_pShader->GetPassDesc(nPassIdx);
		if (pPassDesc->ann.isSubPass) {
			continue;
		}

#ifdef __IZ_DEBUG__
		IZ_PCSTR pszPassName = m_pShader->GetString(pPassDesc->posName);
		CGraphPerf::BeginEvent(0, pszPassName);
#endif	// #ifdef __IZ_DEBUG__

		// ファンクタ取得
		CPostEffectFunctor* pFunctor = m_FunctorHash.FindData(nPassIdx);
		IZ_ASSERT(pFunctor != IZ_NULL);

		IZ_BOOL result = pFunctor->Apply(
							m_pDevice,
							m_pShader);
		VGOTO(result, __EXIT__);

#ifdef __IZ_DEBUG__
		CGraphPerf::EndEvent();
#endif	// #ifdef __IZ_DEBUG__
	}

__EXIT__:
	// レンダーステートを元に戻す
	m_pDevice->SetRenderState(sRS);

	return ret;
}
