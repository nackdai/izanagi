#include "posteffect/PostEffectTextureCreator.h"

using namespace izanagi;

//////////////////////////////////////////////////////

struct CPostEffectTextureCreator::STexHolder {
	graph::CTexture* tex;
	IZ_BOOL lock;

	E_POSTEFFECT_TEXTURE_TYPE type;

	CTexHashItem item;

	void Init(
		graph::CTexture* pTex,
		IZ_BOOL bIsLock,
		E_POSTEFFECT_TEXTURE_TYPE nType)
	{
		tex = pTex;
		lock = bIsLock;

		type = nType;

		IZ_UINT nKey = pTex->GetWidth();

		item.Init(nKey, this);
	}

	CTexHashItem* GetHashItem() { return &item; }
};

//////////////////////////////////////////////////////

CPostEffectTextureCreator* CPostEffectTextureCreator::CreatePostEffectTextureCreator(
	IMemoryAllocator* pAllocator,
	graph::CGraphicsDevice* pDevice,
	IZ_UINT nMaxTexNum)
{
	size_t nSize = sizeof(CPostEffectTextureCreator); 
	nSize += sizeof(STexHolder) * nMaxTexNum;

	IZ_UINT8* pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, nSize);
	VRETURN_NULL(pBuf != IZ_NULL);

	CPostEffectTextureCreator* pInstance = new(pBuf) CPostEffectTextureCreator;
	{
		pBuf += sizeof(CPostEffectTextureCreator);

		pInstance->AddRef();
		pInstance->m_Allocator = pAllocator;

		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->m_pTexList = reinterpret_cast<STexHolder*>(pBuf);
		pInstance->m_nTexNum = nMaxTexNum;
	}

	return pInstance;
}

// コンストラクタ
CPostEffectTextureCreator::CPostEffectTextureCreator()
{
	m_Allocator = IZ_NULL;
	m_pDevice = IZ_NULL;

	m_pTexList = IZ_NULL;

	m_nTexNum = 0;
	m_nTexPos = 0;

	m_bEnableFind = IZ_TRUE;
}

// デストラクタ
CPostEffectTextureCreator::~CPostEffectTextureCreator()
{
	SAFE_RELEASE(m_pDevice);

	if (m_pTexList != IZ_NULL) {
		for (IZ_UINT i = 0; i < m_nTexNum; ++i) {
			SAFE_RELEASE(m_pTexList[i].tex);
		}
	}

	FREE(m_Allocator, m_pTexList);
}

// テクスチャ作成開始
void CPostEffectTextureCreator::BeginCreate()
{
	for (IZ_UINT i = 0; i < m_nTexNum; ++i) {
		if (m_pTexList[i].tex != IZ_NULL) {
			m_pTexList[i].lock = IZ_FALSE;
		}
	}
}

/**
* テクスチャ作成
*/
graph::CTexture* CPostEffectTextureCreator::Create(
	IZ_UINT16 nWidth,
	IZ_UINT16 nHeight,
	graph::E_GRAPH_PIXEL_FMT fmt,
	IZ_BOOL bIsRenderTarget,
	graph::E_GRAPH_RSC_TYPE nRscType,
	E_POSTEFFECT_TEXTURE_TYPE type)
{
	graph::CTexture* ret = IZ_NULL;

	if (type != E_POSTEFFECT_TEXTURE_TYPE_PRIVATE) {
		// まずは探す
		ret = Find(
				nWidth, nHeight,
				fmt,
				bIsRenderTarget,
				nRscType);
		if (ret != IZ_NULL) {
			return ret;
		}
	}

	IZ_ASSERT(m_nTexPos < m_nTexNum);

	// 無いので作る
	if (bIsRenderTarget) {
		ret = m_pDevice->CreateRenderTarget(
				nWidth,
				nHeight,
				fmt);
	}
	else {
		ret = m_pDevice->CreateTexture(
				nWidth,
				nHeight,
				1,
				fmt,
				nRscType);
	}

	STexHolder& sTexHolder = m_pTexList[m_nTexPos++];
	sTexHolder.Init(
		ret,
		IZ_TRUE,
		type);

	m_TexHash.Add(sTexHolder.GetHashItem());

	IZ_ASSERT(ret != IZ_NULL);

	return ret;
}

/**
* テクスチャを探す
*/
graph::CTexture* CPostEffectTextureCreator::Find(
	IZ_UINT16 nWidth,
	IZ_UINT16 nHeight,
	graph::E_GRAPH_PIXEL_FMT fmt,
	IZ_BOOL bIsRenderTarget,
	graph::E_GRAPH_RSC_TYPE nRscType)
{
	if (!m_bEnableFind) {
		return IZ_NULL;
	}

	graph::CTexture* ret = IZ_NULL;

	const IZ_UINT nKey = nWidth;

	CStdList<CTexHashItem>::Item* pItem = m_TexHash.GetHashTop(nKey);

	while (pItem != IZ_NULL) {
		STexHolder* pTexHolder = pItem->GetData()->GetData();

		if (pTexHolder->type != E_POSTEFFECT_TEXTURE_TYPE_PRIVATE) {
			if (!pTexHolder->lock) {
				if ((pTexHolder->tex->GetWidth() == nWidth)
					&& (pTexHolder->tex->GetHeight() == nHeight)
					&& (pTexHolder->tex->GetPixelFormat() == fmt)
					&& (pTexHolder->tex->IsRenderTarget() == bIsRenderTarget)
					&& (pTexHolder->tex->GetRscType() == nRscType))
				{
					ret = pTexHolder->tex;
					break;
				}
			}
		}

		pItem = pItem->GetNext();
	}

	return ret;
}

void CPostEffectTextureCreator::ClearAll()
{
	for (IZ_UINT i = 0; i < m_nTexPos; ++i) {
		SAFE_RELEASE(m_pTexList[i].tex);
	}

	m_TexHash.Clear();
	m_nTexPos = 0;
}
