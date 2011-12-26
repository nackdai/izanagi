#include "posteffect/PostEffectSystem.h"

#include "posteffect/PostEffect.h"
#include "posteffect/functor/PostEffectFunctor.h"
#include "posteffect/functor/PostEffectFunctorCreator.h"
#include "posteffect/PostEffectTextureCreator.h"

#include "posteffect/util/PostEffectShaderUtil.h"

#include "posteffect/functor/PostEffectFunctorDefault.h"
#include "posteffect/functor/PostEffectFunctorRenderStar.h"
#include "posteffect/functor/PostEffectFunctorOnce.h"
#include "posteffect/functor/PostEffectFunctorSwitchTex.h"
#include "posteffect/functor/PostEffectFunctorMGF.h"

using namespace izanagi;

//**********************************************************
// NOTE
// ファンクタを追加したら、必ずここに追加すること！！！
namespace {
	// ファンクタクリエータ
	CPostEffectFunctorCreator<CPostEffectFunctorDefault>       _cFunctorDefualtCreator("PostEffectFunctorDefault");
	CPostEffectFunctorCreator<CPostEffectFunctorRenderStar>    _cFunctorRenderStarCreator("PostEffectFunctorRenderStar");
	CPostEffectFunctorCreator<CPostEffectFunctorOnce>          _cFunctorOnceCreator("PostEffectFunctorOnce");
	CPostEffectFunctorCreator<CPostEffectFunctorSwitchTex>     _cFunctorSwitchTexCreator("PostEffectFunctorSwitchTex");
	CPostEffectFunctorCreator<CPostEffectFunctorMGF>           _cFunctorMGFCreator("PostEffectFunctorMGF");
#if 0
	CPostEffectFunctorCreator<CPostEffectFunctorDOFMerge>      _cFunctorDOFMergeCreator("PostEffectFunctorDOFMerge");
#endif

	// ファンクタクリエータテーブル
	IPostEffectFunctorCreator* _tblFunctorCreator[] = {
		&_cFunctorDefualtCreator,
		&_cFunctorRenderStarCreator,
		&_cFunctorOnceCreator,
		&_cFunctorSwitchTexCreator,
		&_cFunctorMGFCreator,
#if 0
		&_cFunctorDOFMergeCreator,
#endif
	};
}	// namespace
//**********************************************************

// CPostEffectSystemのインスタンス
CPostEffectSystem* CPostEffectSystem::s_pInstance = IZ_NULL;

/**
* インスタンス作成
*/
CPostEffectSystem* CPostEffectSystem::CreatePostEffectSystem(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice)
{
	if (s_pInstance != IZ_NULL) {
		return s_pInstance;
	}

	IZ_ASSERT(pDevice != IZ_NULL);

	CPostEffectSystem* pInstance = IZ_NULL;
	IZ_BOOL result = IZ_TRUE;

	// メモリ確保
	IZ_UINT8* pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CPostEffectSystem));
	VRETURN_NULL(pBuf != IZ_NULL);

	// インスタンス作成
	pInstance = new(pBuf) CPostEffectSystem;
	{
		pInstance->m_pAllocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		// 初期化
		result = pInstance->InitInternal();
		VGOTO(result, __EXIT__);
	}

__EXIT__:
	if (!result) {
		SAFE_RELEASE(pInstance);
	}
	else {
		if (pInstance != IZ_NULL) {
			SAFE_REPLACE(s_pInstance, pInstance);
		}
	}

	return s_pInstance;
}

// コンストラクタ
CPostEffectSystem::CPostEffectSystem()
{
	m_pAllocator = IZ_NULL;
	m_pDevice = IZ_NULL;
	m_pTexCreator = IZ_NULL;
}

// デストラクタ
CPostEffectSystem::~CPostEffectSystem()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pTexCreator);
}

/**
* ポストエフェクト作成
*/
CPostEffect* CPostEffectSystem::CreatePostEffect(IInputStream* in)
{
	// ポストエフェクト作成
	CPostEffect* pPostEffect = CPostEffect::CreatePostEffect(
								m_pAllocator,
								m_pDevice,
								m_pTexCreator,
								&m_cVSMgr,
								in);
	IZ_ASSERT(pPostEffect != IZ_NULL);

	return pPostEffect;
}

namespace {
	IZ_BOOL _CheckColorSurface(
		CSurface** pColorBuffer,
		IZ_UINT nColorBufferNum)
	{
		if (nColorBufferNum < 1) {
			IZ_ASSERT(IZ_FALSE);
			return IZ_FALSE;
		}
		if (pColorBuffer[0] == IZ_NULL) {
			IZ_ASSERT(IZ_FALSE);
			return IZ_FALSE;
		}

		E_GRAPH_PIXEL_FMT fmtBase = pColorBuffer[0]->GetPixelFormat();

		for (IZ_UINT i = 1; i <  nColorBufferNum; ++i) {
			if (pColorBuffer[i] == IZ_NULL) {
				IZ_ASSERT(IZ_FALSE);
				return IZ_FALSE;
			}

			E_GRAPH_PIXEL_FMT fmt = pColorBuffer[i]->GetPixelFormat();

			// ピクセルフォーマットは一致してないといけない・・・
			IZ_ASSERT(fmtBase == fmt);
		}

		return IZ_TRUE;
	}
}	// namespace

// 解放
void CPostEffectSystem::InternalRelease()
{
	delete this;

	FREE(m_pAllocator, this);

	if (s_pInstance == this) {
		s_pInstance = IZ_NULL;
	}
}

// 初期化
IZ_BOOL CPostEffectSystem::InitInternal()
{
	IZ_BOOL result = IZ_TRUE;

	// 頂点シェーダマネージャ初期化
	result = m_cVSMgr.InitRenderer(m_pDevice);

	// ファンクタクリエータを登録
	for (IZ_UINT i = 0; i < COUNTOF(_tblFunctorCreator); ++i) {
		AddFunctorCreator(_tblFunctorCreator[i]);
	}

	return result;
}

// ファンクタ作成
CPostEffectFunctor* CPostEffectSystem::CreateFunctor(
	IZ_UINT nKey,
	IZ_UINT nPassIdx,
	IZ_UINT8* pBuffer,
	size_t* pFunctorSize)
{
	// ハッシュキー計算
	CKey cHashKey(nKey);

	IPostEffectFunctorCreator* pFunctorCreator = m_FunctorCreatorHash.FindData(cHashKey);
	VRETURN_NULL(pFunctorCreator != IZ_NULL);

	CPostEffectFunctor* ret = pFunctorCreator->CreateFunctor(nPassIdx, pBuffer);
	IZ_ASSERT(ret != IZ_NULL);

	if (ret != IZ_NULL) {
		ret->AddRef();

		if (pFunctorSize != IZ_NULL) {
			*pFunctorSize = pFunctorCreator->GetFunctorSize();
		}
	}
	
	return ret;
}

// ファンクタサイズ取得
size_t CPostEffectSystem::GetFunctorSize(IZ_PCSTR pszName)
{
	// ハッシュキー計算
	CKey cHashKey(pszName);

	IPostEffectFunctorCreator* pFunctorCreator = m_FunctorCreatorHash.FindData(cHashKey);
	VRETURN_NULL(pFunctorCreator != IZ_NULL);

	size_t ret = pFunctorCreator->GetFunctorSize();
	IZ_ASSERT(ret > 0);

	return ret;
}

/**
* ファンクタクリエイタをセット
*/
IZ_BOOL CPostEffectSystem::AddFunctorCreator(IPostEffectFunctorCreator* pFunctorCreator)
{
	IZ_ASSERT(pFunctorCreator != IZ_NULL);

	IZ_BOOL ret = m_FunctorCreatorHash.Add(pFunctorCreator->GetHashItem());
	IZ_ASSERT(ret);

	return ret;
}
