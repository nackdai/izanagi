#include "graph/dx9/CubeTexture.h"
#include "graph/dx9/GraphicsDevice.h"
#include "std/MemoryAllocator.h"
#include "graph/internal/ParamValueConverter.h"

using namespace izanagi;

// ファイルからテクスチャ作成
CCubeTexture* CCubeTexture::CreateCubeTextureFromFile(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	IZ_PCSTR lpszPathName,
	E_GRAPH_PIXEL_FMT fmt)
{
	CCubeTexture* pInstance = CreateInternal(
								pDevice,
								pAllocator,
								lpszPathName,
								0, 0, 0,		// 使用しないので適当
								fmt,
								&CCubeTexture::CreateTextureFromFileImpl);

	return pInstance;
}

// テクスチャ作成
CCubeTexture* CCubeTexture::CreateCubeTexture(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT fmt)
{
	CCubeTexture* pInstance = CreateInternal(
								pDevice,
								pAllocator,
								IZ_NULL,
								nWidth,
								nHeight,
								nMipLevel,
								fmt,
								&CCubeTexture::CreateTextureImpl);

	return pInstance;
}

CCubeTexture* CCubeTexture::CreateInternal(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	IZ_PCSTR lpszPathName,
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT fmt,
	CreateTexFunc pCreateTexFunc)
{
	IZ_ASSERT(pDevice != IZ_NULL);
	IZ_ASSERT(pCreateTexFunc != IZ_NULL);

	D3D_DEVICE* pD3DDev = pDevice->GetRawInterface();

	IZ_BOOL result = IZ_TRUE;
	IZ_UINT8* pBuf = IZ_NULL;
	CCubeTexture* pInstance = IZ_NULL;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CCubeTexture));
	if (!(result = (pBuf != IZ_NULL))) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf)CCubeTexture;
	{
		pInstance->m_pAllocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->AddRef();
	}

	// 初期化
	result = (pInstance->*pCreateTexFunc)(
				lpszPathName,
				nWidth,
				nHeight,
				nMipLevel,
				fmt);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
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

// 解放
void CCubeTexture::InternalRelease()
{
	delete this;

	if (m_pAllocator != IZ_NULL) {
		m_pAllocator->Free(this);
	}
}

// ファイルからテクスチャ作成
IZ_BOOL CCubeTexture::CreateTextureFromFileImpl(
	IZ_PCSTR lpszPathName,
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT nFmt)
{
	IZ_ASSERT(m_pDevice != IZ_NULL);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

	D3DFORMAT fmt = (nFmt >= E_GRAPH_PIXEL_FMT_NUM
						? D3DFMT_FROM_FILE
						: IZ_GET_TARGET_PIXEL_FMT(nFmt));

	HRESULT hr;
	hr = D3DXCreateCubeTextureFromFileEx(
			pD3DDev,
			lpszPathName,
			D3DX_DEFAULT,		// size
			D3DFMT_FROM_FILE,	// mip levels
			0,					// usage
			fmt,				// format
			D3DPOOL_MANAGED,	// pool
			D3DX_FILTER_LINEAR,	// filter
			D3DX_FILTER_LINEAR,	// mip filter
			0,					// color key
			NULL,
			NULL,
			&m_pTexture);

	VRETURN(SUCCEEDED(hr));

	// テクスチャ情報取得
	GetTextureInfo();

	return IZ_TRUE;
}

// テクスチャ作成
IZ_BOOL CCubeTexture::CreateTextureImpl(
	IZ_PCSTR lpszPathName,
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	IZ_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT nFmt)
{
	IZ_ASSERT(m_pDevice != IZ_NULL);
	IZ_ASSERT(nWidth == nHeight);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

	D3DFORMAT fmt = IZ_GET_TARGET_PIXEL_FMT(nFmt);

	HRESULT hr;
	hr = D3DXCreateCubeTexture(
			pD3DDev,
			nWidth,				// size
			nMipLevel,			// mip levels
			0,					// usage
			fmt,				// format
			D3DPOOL_MANAGED,	// pool
			&m_pTexture);

	VRETURN(SUCCEEDED(hr));

	// テクスチャ情報取得
	GetTextureInfo();

	return IZ_TRUE;
}

// テクスチャ情報取得
void CCubeTexture::GetTextureInfo()
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
}

/**
* ロック
*/
IZ_UINT CCubeTexture::Lock(
	E_GRAPH_CUBE_TEX_FACE nFace,
	UINT nLevel,
	void** data,
	IZ_BOOL bIsReadOnly,
	IZ_BOOL bIsDiscard/*= IZ_FALSE*/)
{
	IZ_ASSERT(m_pTexture != IZ_NULL);

	if (!IsDynamic() && !IsOnSysMem()) {
		// 動的テクスチャ以外ではロック不可
		// ただし、システムメモリ上にあるときはＯＫ
		IZ_ASSERT(IZ_FALSE);
		return 0;
	}

	if (nLevel >= m_TexInfo.level) {
		// レベル指定オーバー
		IZ_ASSERT(IZ_FALSE);
		return 0;
	}

	DWORD flag = 0;
	if (bIsReadOnly) {
		flag = D3DLOCK_READONLY;
	}
	else if (IsDynamic()) {
		if (bIsDiscard) {
			flag = D3DLOCK_DISCARD;
		}
		else {
			flag = D3DLOCK_NOOVERWRITE;
		}
	}

	D3DLOCKED_RECT rect;
	HRESULT hr = m_pTexture->LockRect(
					IZ_GET_TARGET_CUBE_FACE(nFace),
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
IZ_BOOL CCubeTexture::Unlock(
	E_GRAPH_CUBE_TEX_FACE nFace,
	IZ_UINT nLevel)
{
	IZ_ASSERT(m_pTexture != IZ_NULL);

	HRESULT hr = m_pTexture->UnlockRect(
					IZ_GET_TARGET_CUBE_FACE(nFace),
					nLevel);
	IZ_BOOL ret = SUCCEEDED(hr);
	IZ_ASSERT(ret);

	return ret;
}
