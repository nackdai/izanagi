#include "graph/dx9/Surface.h"
#include "graph/dx9/Texture.h"
#include "std/MemoryAllocator.h"

using namespace uranus;

CSurface* CSurface::CreateSurface(IMemoryAllocator* pAllocator)
{
	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CSurface* pInstance = UN_NULL;

	// メモリ確保
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CSurface));
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf)CSurface;
	{
		pInstance->m_pAllocator = pAllocator;
		pInstance->AddRef();
	}

__EXIT__:
	if (!result) {
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != UN_NULL) {
			pAllocator->Free(pBuf);
		}
	}

	return pInstance;
}

// コンストラクタ
CSurface::CSurface()
{
	m_pAllocator = UN_NULL;

	m_pSurface = UN_NULL;

	FILL_ZERO(&m_Desc, sizeof(m_Desc));
}

// デストラクタ
CSurface::~CSurface()
{
	SAFE_RELEASE(m_pSurface);
}

// 解放
void CSurface::InternalRelease()
{
	delete this;

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}
}

// リセット
UN_BOOL CSurface::Reset(
	CTexture* pTexture,
	UN_UINT nLevel)
{
	UN_BOOL ret = UN_TRUE;

	SAFE_RELEASE(m_pSurface);

	if (pTexture != UN_NULL) {
		UN_ASSERT(nLevel < pTexture->GetMipMapNum());

		D3D_TEXTURE* pD3DTex = pTexture->GetRawInterface();

		// TODO
		// 最上位レベルのみ
		HRESULT hr = pD3DTex->GetSurfaceLevel(nLevel, &m_pSurface);

		UN_BOOL ret = SUCCEEDED(hr);
		UN_ASSERT(ret);
	}

	if (ret && (m_pSurface != UN_NULL)) {
		// 記述取得
		HRESULT hr = m_pSurface->GetDesc(&m_Desc);
		ret = SUCCEEDED(hr);
		UN_ASSERT(ret);
	}

	return ret;
}
