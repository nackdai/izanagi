#include "graph/dx9/Surface.h"
#include "graph/dx9/Texture.h"
#include "std/allocator/MemoryAllocator.h"

using namespace izanagi;

// サーフェース作成.
CSurface* CSurface::CreateDepthStencilSurface(
	IMemoryAllocator* allocator,
	CGraphicsDevice* device,
	IZ_UINT width, 
	IZ_UINT height,
	E_GRAPH_PIXEL_FMT fmt)
{
	VRETURN_NULL(fmt == E_GRAPH_PIXEL_FMT_D24S8);

	IZ_BOOL result = IZ_FALSE;

	void* buf = ALLOC_ZERO(allocator, sizeof(CSurface));
	VRETURN_NULL(buf != IZ_NULL);

	CSurface* instance = new(buf) CSurface();
	{
		instance->m_pAllocator = allocator;
		instance->AddRef();

		D3D_DEVICE* d3dDevice = device->GetRawInterface();

		HRESULT hr = d3dDevice->CreateDepthStencilSurface(
						width, height,
						CD3D9ParamValueConverter::ConvAbstractToTarget_PixelFormat(fmt),
						D3DMULTISAMPLE_NONE, 0,
						TRUE,
						&instance->m_pSurface,
						NULL);
		result = SUCCEEDED(hr);
		if (result) {
			instance->SetDesc();
		}
	}

	if (!result) {
		SAFE_RELEASE(instance);
	}
	return instance;
}

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
		pInstance->m_pAllocator = pAllocator;
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
	m_pAllocator = IZ_NULL;

	m_pSurface = IZ_NULL;

	FILL_ZERO(&m_Desc, sizeof(m_Desc));
	m_Fmt = E_GRAPH_PIXEL_FMT_FORCE_INT32;
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

	if (m_pAllocator != IZ_NULL) {
		m_pAllocator->Free(this);
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

#if 0
	if (ret && (m_pSurface != IZ_NULL)) {
		// 記述取得
		HRESULT hr = m_pSurface->GetDesc(&m_Desc);
		ret = SUCCEEDED(hr);
		IZ_ASSERT(ret);
	}
#else
	if (ret && (m_pSurface != IZ_NULL)) {
		ret = SetDesc();
		IZ_ASSERT(ret);
	}
#endif

	return ret;
}

// うーん・・・
IZ_BOOL CSurface::SetDesc()
{
	IZ_BOOL ret = IZ_FALSE;
	if (m_pSurface != IZ_NULL) {
		HRESULT hr = m_pSurface->GetDesc(&m_Desc);
		ret = SUCCEEDED(hr);

		if (ret) {
			m_Fmt = IZ_GET_ABST_PIXEL_FMT(m_Desc.Format);
		}
	}

	return ret;
}
