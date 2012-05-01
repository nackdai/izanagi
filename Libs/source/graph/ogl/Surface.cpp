#include "graph/dx9/Surface.h"
#include "graph/dx9/Texture.h"
#include "std/MemoryAllocator.h"

using namespace izanagi;

CSurface* CSurface::CreateSurface(IMemoryAllocator* pAllocator)
{
	IZ_BOOL result = IZ_TRUE;
	IZ_UINT8* pBuf = IZ_NULL;
	CSurface* pInstance = IZ_NULL;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CSurface));
	if (!(result = (pBuf != IZ_NULL))) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf)CSurface;
	{
		pInstance->m_Allocator = pAllocator;
		pInstance->AddRef();
	}

__EXIT__:
	if (!result) {
		if (pInstance != IZ_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != IZ_NULL) {
			pAllocator->Free(pBuf);
		}
	}

	return pInstance;
}

// コンストラクタ
CSurface::CSurface()
{
	m_Allocator = IZ_NULL;

	m_pSurface = IZ_NULL;

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

	if (m_Allocator != IZ_NULL) {
		m_Allocator->Free(this);
	}
}

// リセット
IZ_BOOL CSurface::Reset(
	CTexture* pTexture,
	IZ_UINT nLevel)
{
	IZ_BOOL ret = IZ_TRUE;

	SAFE_RELEASE(m_pSurface);

	if (pTexture != IZ_NULL) {
		IZ_ASSERT(nLevel < pTexture->GetMipMapNum());

		D3D_TEXTURE* pD3DTex = pTexture->GetRawInterface();

		// TODO
		// 最上位レベルのみ
		HRESULT hr = pD3DTex->GetSurfaceLevel(nLevel, &m_pSurface);

		IZ_BOOL ret = SUCCEEDED(hr);
		IZ_ASSERT(ret);
	}

	if (ret && (m_pSurface != IZ_NULL)) {
		// 記述取得
		HRESULT hr = m_pSurface->GetDesc(&m_Desc);
		ret = SUCCEEDED(hr);
		IZ_ASSERT(ret);
	}

	return ret;
}
