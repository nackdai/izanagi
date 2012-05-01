#include "graph/dx9/Texture.h"
#include "graph/dx9/Surface.h"
#include "std/MemoryAllocator.h"
#include "graph/dx9/D3D9ParamValueConverter.h"

using namespace izanagi;

namespace {
	struct SFuncCreateTextureFromFile {
		IZ_BOOL operator()(
			D3D_TEXTURE** pTex,
			CGraphicsDevice* pDevice,
			IZ_PCSTR lpszPathName,
			IZ_UINT nDummy,
			E_GRAPH_PIXEL_FMT nFmt)
		{
			D3D_DEVICE* pD3DDev = pDevice->GetRawInterface();

			D3DFORMAT fmt = (nFmt >= E_GRAPH_PIXEL_FMT_NUM
								? D3DFMT_FROM_FILE
								: IZ_GET_TARGET_PIXEL_FMT(nFmt));

			HRESULT hr;
			hr = D3DXCreateTextureFromFileEx(
					pD3DDev,
					lpszPathName,
					D3DX_DEFAULT,		// width
					D3DX_DEFAULT,		// height
					D3DFMT_FROM_FILE,	// mip levels
					0,					// usage
					fmt,				// format
					D3DPOOL_MANAGED,	// pool
					D3DX_FILTER_LINEAR,	// filter
					D3DX_FILTER_LINEAR,	// mip filter
					0,					// color key
					NULL,
					NULL,
					pTex);

			IZ_BOOL ret = SUCCEEDED(hr);
			IZ_ASSERT(ret);

			return ret;
		}
	};

	struct SFuncCreateTextureFromMemory {
		IZ_BOOL operator()(
			D3D_TEXTURE** pTex,
			CGraphicsDevice* pDevice,
			const void* pData,
			IZ_UINT nDataSize,
			E_GRAPH_PIXEL_FMT nFmt)
		{
			D3D_DEVICE* pD3DDev = pDevice->GetRawInterface();

			D3DFORMAT fmt = (nFmt >= E_GRAPH_PIXEL_FMT_NUM
								? D3DFMT_FROM_FILE
								: IZ_GET_TARGET_PIXEL_FMT(nFmt));

			HRESULT hr;
			hr = D3DXCreateTextureFromFileInMemoryEx (
					pD3DDev,
					pData,
					nDataSize,
					D3DX_DEFAULT,		// width
					D3DX_DEFAULT,		// height
					D3DFMT_FROM_FILE,	// mip levels
					0,					// usage
					fmt,				// format
					D3DPOOL_MANAGED,	// pool
					D3DX_FILTER_LINEAR,	// filter
					D3DX_FILTER_LINEAR,	// mip filter
					0,					// color key
					NULL,
					NULL,
					pTex);

			IZ_BOOL ret = SUCCEEDED(hr);
			IZ_ASSERT(ret);

			return ret;
		}
	};
}	// namespace

// ファイルからテクスチャ作成
CTexture* CTexture::CreateTextureFromFile(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	IZ_PCSTR lpszPathName,
	E_GRAPH_PIXEL_FMT fmt)
{
	CTexture* ret = CreateBody_From(
						pDevice,
						pAllocator,
						lpszPathName,
						0,
						SFuncCreateTextureFromFile(),
						fmt);
	return ret;
}

// データからテクスチャ作成
CTexture* CTexture::CreateTextureFromMemory(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const void* pData,
	IZ_UINT nDataSize,
	E_GRAPH_PIXEL_FMT fmt)
{
	CTexture* ret = CreateBody_From(
						pDevice, pAllocator,
						pData, nDataSize,
						SFuncCreateTextureFromMemory(),
						fmt);
	return ret;
}

// 本体作成
// 入力から作成
template <typename _T, typename _Func>
CTexture* CTexture::CreateBody_From(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	_T tInput,
	IZ_UINT nInputSize,
	_Func& func,
	E_GRAPH_PIXEL_FMT fmt)
{
	IZ_ASSERT(pDevice != IZ_NULL);

	IZ_BOOL result = IZ_TRUE;
	IZ_UINT8* pBuf = IZ_NULL;
	CTexture* pInstance = IZ_NULL;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CTexture));
	if (!(result = (pBuf != IZ_NULL))) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf)CTexture;
	{
		pInstance->m_Allocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->AddRef();
	}
	
	// 本体作成
	{
		// 本体作成
		result = func(
					&pInstance->m_pTexture,
					pDevice,
					tInput,
					nInputSize,
					fmt);
		IZ_ASSERT(result);

		pInstance->GetTextureInfo();
	}

	if (!result) {
		goto __EXIT__;
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

// テクスチャ作成
CTexture* CTexture::CreateTexture(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT fmt,
	E_GRAPH_RSC_TYPE nCreateType,
	IZ_BOOL bIsOnSysMem/*= IZ_FALSE*/)
{
	IZ_ASSERT(pDevice != IZ_NULL);

	IZ_BOOL result = IZ_TRUE;
	IZ_UINT8* pBuf = IZ_NULL;
	CTexture* pInstance = IZ_NULL;

	IZ_UINT nMaxMipLevel = 1;
	{
		IZ_UINT nW = nWidth;
		IZ_UINT nH = nHeight;
		while ((nW > 1) && (nH > 1)) {
			nW >>= 1;
			nH >>= 1;
			++nMaxMipLevel;
		}
	}

	size_t nSize = sizeof(CTexture);

	IZ_BOOL bIsCreateSurface = (nMipLevel <= nMaxMipLevel);
	if (bIsCreateSurface) {
		nMipLevel = (nMipLevel == 0 ? nMaxMipLevel : nMipLevel);
		nSize += sizeof(CSurface) * nMipLevel;
	}
	else {
		nMipLevel = 1;
	}

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, nSize);
	if (!(result = (pBuf != IZ_NULL))) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf)CTexture;
	{
		pInstance->m_Allocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		if (bIsCreateSurface) {
			pInstance->m_pSurface = reinterpret_cast<CSurface**>(pBuf + sizeof(CTexture));
		}

		pInstance->AddRef();
	}

	// 本体作成
	result = pInstance->CreateBody_Texture(
				nWidth,
				nHeight,
				nMipLevel,
				fmt,
				nCreateType,
				bIsOnSysMem);
	if (!result) {
		goto __EXIT__;
	}

	if (bIsCreateSurface) {
		// サーフェス作成
		result = pInstance->CreateSurface();
		if (!result) {
			goto __EXIT__;
		}
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

// レンダーターゲット作成
CTexture* CTexture::CreateRenderTarget(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	IZ_UINT nWidth, 
	IZ_UINT nHeight,
	E_GRAPH_PIXEL_FMT fmt)
{
	IZ_ASSERT(pDevice != IZ_NULL);

	IZ_UINT nMipLevel = 1;

	IZ_BOOL result = IZ_TRUE;
	IZ_UINT8* pBuf = IZ_NULL;
	CTexture* pInstance = IZ_NULL;

	size_t nSize = sizeof(CTexture) + sizeof(CSurface*) * nMipLevel;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, nSize);
	if (!(result = (pBuf != IZ_NULL))) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf)CTexture;
	{
		pInstance->m_Allocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->m_pSurface = reinterpret_cast<CSurface**>(pBuf + sizeof(CTexture));

		pInstance->AddRef();
	}

	// 本体作成
	result = pInstance->CreateBody_RenderTarget(
				nWidth, nHeight,
				fmt,
				nMipLevel);
	if (!result) {
		goto __EXIT__;
	}

	// サーフェス作成
	result = pInstance->CreateSurface();
	if (!result) {
		goto __EXIT__;
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

// 本体作成（テクスチャ）
IZ_BOOL CTexture::CreateBody_Texture(
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT nFmt,
	E_GRAPH_RSC_TYPE nCreateType,
	IZ_BOOL bIsOnSysMem)
{
	IZ_ASSERT(m_pDevice != IZ_NULL);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();
	
	IZ_DWORD nUsage = 0;
	if (!bIsOnSysMem) {
		nUsage = (nCreateType & E_GRAPH_RSC_TYPE_STATIC
					? 0
					: D3DUSAGE_DYNAMIC);
	}

	D3DPOOL nPool = D3DPOOL_SYSTEMMEM;
	if (!bIsOnSysMem) {
		nPool = (nCreateType & E_GRAPH_RSC_TYPE_DYNAMIC
					? D3DPOOL_DEFAULT
					: D3DPOOL_MANAGED);
	}

	D3DFORMAT fmt = IZ_GET_TARGET_PIXEL_FMT(nFmt);

	// 本体作成
	HRESULT hr;
	hr = pD3DDev->CreateTexture(
			nWidth,			// width
			nHeight,		// height
			nMipLevel,		// mip levels
			nUsage,			// usage
			fmt,			// format
			nPool,			// pool
			&m_pTexture,
			NULL);

	VRETURN(SUCCEEDED(hr));

	// テクスチャ情報取得
	GetTextureInfo();

	return IZ_TRUE;
}

// 本体作成（レンダーターゲット）
IZ_BOOL CTexture::CreateBody_RenderTarget(
	IZ_UINT nWidth, 
	IZ_UINT nHeight,
	E_GRAPH_PIXEL_FMT nFmt,
	IZ_UINT nMipLevel)
{
	IZ_ASSERT(m_pDevice != IZ_NULL);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

	D3DFORMAT fmt = IZ_GET_TARGET_PIXEL_FMT(nFmt);

	// NOTE
	// poolはD3DPOOL_DEFAULTしか許されていない

	// 本体作成
	HRESULT hr;
	hr = pD3DDev->CreateTexture(
			nWidth,					// width
			nHeight,				// height
			nMipLevel,				// mip levels
			D3DUSAGE_RENDERTARGET,	// usage
			fmt,					// format
			D3DPOOL_DEFAULT,		// pool
			&m_pTexture,
			NULL);

	VRETURN(SUCCEEDED(hr));

	// テクスチャ情報取得
	GetTextureInfo();

	return IZ_TRUE;
}

// テクスチャ情報取得
void CTexture::GetTextureInfo()
{
	IZ_ASSERT(m_pTexture != IZ_NULL);

	D3DSURFACE_DESC sDesc;

	m_pTexture->GetLevelDesc(0, &sDesc);

	m_TexInfo.width = sDesc.Width;
	m_TexInfo.height = sDesc.Height;
	
	m_TexInfo.level = static_cast<IZ_UINT8>(m_pTexture->GetLevelCount());
	m_TexInfo.fmt = IZ_GET_ABST_PIXEL_FMT(sDesc.Format);

	m_TexInfo.is_rendertarget = (sDesc.Usage == D3DUSAGE_RENDERTARGET);
	m_TexInfo.is_dynamic = ((sDesc.Usage & D3DUSAGE_DYNAMIC) > 0);
	m_TexInfo.is_on_sysmem = (sDesc.Pool == D3DPOOL_SYSTEMMEM);
	m_TexInfo.is_on_vram = (sDesc.Pool == D3DPOOL_DEFAULT);
}

// サーフェス作成
IZ_BOOL CTexture::CreateSurface()
{
	// サーフェス作成
	if (m_pSurface != IZ_NULL) {
		for (IZ_UINT i = 0; i < m_TexInfo.level; ++i) {
			m_pSurface[i] = CSurface::CreateSurface(m_Allocator);
			IZ_BOOL result = (m_pSurface != IZ_NULL);

			if (result) {
				result = m_pSurface[i]->Reset(this, i);
				VRETURN(result);
			}
			else {
				IZ_ASSERT(IZ_FALSE);
				return IZ_FALSE;
			}
		}
	}

	return IZ_TRUE;
}

/**
* ロック
*/
IZ_UINT CTexture::Lock(
	UINT nLevel,
	void** data,
	IZ_BOOL bIsReadOnly,
	IZ_BOOL bIsDiscard/*= IZ_FALSE*/)
{
	// NOTE
	// 動的テクスチャにおいて
	// ミップマップの場合、最上位レベルでのみ LOCK_DISCARD を使える
	// 最上位レベルをロックするだけですべてのレベルが破棄される

	IZ_ASSERT(m_pTexture != IZ_NULL);

#if 0
	if (!IsDynamic() && !IsOnSysMem()) {
		// 動的テクスチャ以外ではロック不可
		// ただし、システムメモリ上にあるときはＯＫ
		IZ_ASSERT(IZ_FALSE);
		return 0;
	}
#endif

	if (nLevel >= m_TexInfo.level) {
		// レベル指定オーバー
		IZ_ASSERT(IZ_FALSE);
		return 0;
	}

	DWORD flag = 0;
	if (IsDynamic()) {
		// READONLY不可
		VRETURN(!bIsReadOnly);

		if (bIsDiscard) {
			flag = D3DLOCK_DISCARD;
		}
		else {
			flag = D3DLOCK_NOOVERWRITE;
		}
	}
	else if (bIsReadOnly) {
		flag = D3DLOCK_READONLY;
	}

	D3DLOCKED_RECT rect;
	HRESULT hr = m_pTexture->LockRect(
					nLevel,
					&rect,
					NULL,
					flag);
	IZ_ASSERT(SUCCEEDED(hr));

	UINT ret = 0;

	if (SUCCEEDED(hr)) {
		*data = rect.pBits;
		ret = rect.Pitch;
	}

	return ret;
}

/**
* アンロック
*/
IZ_BOOL CTexture::Unlock(IZ_UINT nLevel)
{
	IZ_ASSERT(m_pTexture != IZ_NULL);

	HRESULT hr = m_pTexture->UnlockRect(nLevel);
	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}

// サーフェス取得
CSurface* CTexture::GetSurface(IZ_UINT idx)
{
	IZ_ASSERT(idx < GetMipMapNum());
	IZ_ASSERT(m_pSurface != IZ_NULL);

	return m_pSurface[idx];
}

// リセット
IZ_BOOL CTexture::Reset()
{
	IZ_BOOL ret = IZ_TRUE;

	if (IsRenderTarget()) {
		// RenderTargetは強制的にリセット

		SAFE_RELEASE(m_pTexture);

		ret = CreateBody_RenderTarget(
				GetWidth(),
				GetHeight(),
				GetPixelFormat(),
				GetMipMapNum());
		IZ_ASSERT(ret);

		if (ret) {
			// サーフェスもリセット
			for (IZ_UINT32 i = 0; i < GetMipMapNum(); ++i) {
				ret = m_pSurface[i]->Reset(this, i);
				IZ_ASSERT(ret);
			}
		}
	}
	else if (IsOnVram()) {
		SAFE_RELEASE(m_pTexture);

		ret = CreateBody_Texture(
				GetWidth(),
				GetHeight(),
				GetMipMapNum(),
				GetPixelFormat(),
				E_GRAPH_RSC_TYPE_DYNAMIC,
				IsOnSysMem());
		IZ_ASSERT(ret);
	}

	return ret;
}
