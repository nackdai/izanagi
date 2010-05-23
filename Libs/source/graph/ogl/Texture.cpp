#include "graph/dx9/Texture.h"
#include "graph/dx9/Surface.h"
#include "std/MemoryAllocator.h"
#include "graph/dx9/D3D9ParamValueConverter.h"

using namespace uranus;

namespace {
	struct SFuncCreateTextureFromFile {
		UN_BOOL operator()(
			D3D_TEXTURE** pTex,
			CGraphicsDevice* pDevice,
			UN_PCSTR lpszPathName,
			UN_UINT nDummy,
			E_GRAPH_PIXEL_FMT nFmt)
		{
			D3D_DEVICE* pD3DDev = pDevice->GetRawInterface();

			D3DFORMAT fmt = (nFmt >= E_GRAPH_PIXEL_FMT_NUM
								? D3DFMT_FROM_FILE
								: UN_GET_TARGET_PIXEL_FMT(nFmt));

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

			UN_BOOL ret = SUCCEEDED(hr);
			UN_ASSERT(ret);

			return ret;
		}
	};

	struct SFuncCreateTextureFromMemory {
		UN_BOOL operator()(
			D3D_TEXTURE** pTex,
			CGraphicsDevice* pDevice,
			const void* pData,
			UN_UINT nDataSize,
			E_GRAPH_PIXEL_FMT nFmt)
		{
			D3D_DEVICE* pD3DDev = pDevice->GetRawInterface();

			D3DFORMAT fmt = (nFmt >= E_GRAPH_PIXEL_FMT_NUM
								? D3DFMT_FROM_FILE
								: UN_GET_TARGET_PIXEL_FMT(nFmt));

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

			UN_BOOL ret = SUCCEEDED(hr);
			UN_ASSERT(ret);

			return ret;
		}
	};
}	// namespace

// �t�@�C������e�N�X�`���쐬
CTexture* CTexture::CreateTextureFromFile(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	UN_PCSTR lpszPathName,
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

// �f�[�^����e�N�X�`���쐬
CTexture* CTexture::CreateTextureFromMemory(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const void* pData,
	UN_UINT nDataSize,
	E_GRAPH_PIXEL_FMT fmt)
{
	CTexture* ret = CreateBody_From(
						pDevice, pAllocator,
						pData, nDataSize,
						SFuncCreateTextureFromMemory(),
						fmt);
	return ret;
}

// �{�̍쐬
// ���͂���쐬
template <typename _T, typename _Func>
CTexture* CTexture::CreateBody_From(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	_T tInput,
	UN_UINT nInputSize,
	_Func& func,
	E_GRAPH_PIXEL_FMT fmt)
{
	UN_ASSERT(pDevice != UN_NULL);

	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CTexture* pInstance = UN_NULL;

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CTexture));
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new (pBuf)CTexture;
	{
		pInstance->m_pAllocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->AddRef();
	}
	
	// �{�̍쐬
	{
		// �{�̍쐬
		result = func(
					&pInstance->m_pTexture,
					pDevice,
					tInput,
					nInputSize,
					fmt);
		UN_ASSERT(result);

		pInstance->GetTextureInfo();
	}

	if (!result) {
		goto __EXIT__;
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

// �e�N�X�`���쐬
CTexture* CTexture::CreateTexture(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	UN_UINT nWidth,
	UN_UINT nHeight,
	UN_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT fmt,
	E_GRAPH_RSC_CREATE_TYPE nCreateType,
	UN_BOOL bIsOnSysMem/*= UN_FALSE*/)
{
	UN_ASSERT(pDevice != UN_NULL);

	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CTexture* pInstance = UN_NULL;

	UN_UINT nMaxMipLevel = 1;
	{
		UN_UINT nW = nWidth;
		UN_UINT nH = nHeight;
		while ((nW > 1) && (nH > 1)) {
			nW >>= 1;
			nH >>= 1;
			nMaxMipLevel++;
		}
	}

	size_t nSize = sizeof(CTexture);

	UN_BOOL bIsCreateSurface = (nMipLevel <= nMaxMipLevel);
	if (bIsCreateSurface) {
		nMipLevel = (nMipLevel == 0 ? nMaxMipLevel : nMipLevel);
		nSize += sizeof(CSurface) * nMipLevel;
	}
	else {
		nMipLevel = 1;
	}

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, nSize);
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new (pBuf)CTexture;
	{
		pInstance->m_pAllocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		if (bIsCreateSurface) {
			pInstance->m_pSurface = reinterpret_cast<CSurface**>(pBuf + sizeof(CTexture));
		}

		pInstance->AddRef();
	}

	// �{�̍쐬
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
		// �T�[�t�F�X�쐬
		result = pInstance->CreateSurface();
		if (!result) {
			goto __EXIT__;
		}
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

// �����_�[�^�[�Q�b�g�쐬
CTexture* CTexture::CreateRenderTarget(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	UN_UINT nWidth, 
	UN_UINT nHeight,
	E_GRAPH_PIXEL_FMT fmt)
{
	UN_ASSERT(pDevice != UN_NULL);

	UN_UINT nMipLevel = 1;

	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CTexture* pInstance = UN_NULL;

	size_t nSize = sizeof(CTexture) + sizeof(CSurface*) * nMipLevel;

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, nSize);
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
	pInstance = new (pBuf)CTexture;
	{
		pInstance->m_pAllocator = pAllocator;
		SAFE_REPLACE(pInstance->m_pDevice, pDevice);

		pInstance->m_pSurface = reinterpret_cast<CSurface**>(pBuf + sizeof(CTexture));

		pInstance->AddRef();
	}

	// �{�̍쐬
	result = pInstance->CreateBody_RenderTarget(
				nWidth, nHeight,
				fmt,
				nMipLevel);
	if (!result) {
		goto __EXIT__;
	}

	// �T�[�t�F�X�쐬
	result = pInstance->CreateSurface();
	if (!result) {
		goto __EXIT__;
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

// �{�̍쐬�i�e�N�X�`���j
UN_BOOL CTexture::CreateBody_Texture(
	UN_UINT nWidth,
	UN_UINT nHeight,
	UN_UINT nMipLevel,
	E_GRAPH_PIXEL_FMT nFmt,
	E_GRAPH_RSC_CREATE_TYPE nCreateType,
	UN_BOOL bIsOnSysMem)
{
	UN_ASSERT(m_pDevice != UN_NULL);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();
	
	UN_DWORD nUsage = 0;
	if (!bIsOnSysMem) {
		nUsage = (nCreateType & E_GRAPH_RSC_CREATE_TYPE_STATIC
					? 0
					: D3DUSAGE_DYNAMIC);
	}

	D3DPOOL nPool = D3DPOOL_SYSTEMMEM;
	if (!bIsOnSysMem) {
		nPool = (nCreateType & E_GRAPH_RSC_CREATE_TYPE_DYNAMIC
					? D3DPOOL_DEFAULT
					: D3DPOOL_MANAGED);
	}

	D3DFORMAT fmt = UN_GET_TARGET_PIXEL_FMT(nFmt);

	// �{�̍쐬
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

	// �e�N�X�`�����擾
	GetTextureInfo();

	return UN_TRUE;
}

// �{�̍쐬�i�����_�[�^�[�Q�b�g�j
UN_BOOL CTexture::CreateBody_RenderTarget(
	UN_UINT nWidth, 
	UN_UINT nHeight,
	E_GRAPH_PIXEL_FMT nFmt,
	UN_UINT nMipLevel)
{
	UN_ASSERT(m_pDevice != UN_NULL);

	D3D_DEVICE* pD3DDev = m_pDevice->GetRawInterface();

	D3DFORMAT fmt = UN_GET_TARGET_PIXEL_FMT(nFmt);

	// NOTE
	// pool��D3DPOOL_DEFAULT����������Ă��Ȃ�

	// �{�̍쐬
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

	// �e�N�X�`�����擾
	GetTextureInfo();

	return UN_TRUE;
}

// �e�N�X�`�����擾
void CTexture::GetTextureInfo()
{
	UN_ASSERT(m_pTexture != UN_NULL);

	D3DSURFACE_DESC sDesc;

	m_pTexture->GetLevelDesc(0, &sDesc);

	m_TexInfo.width = sDesc.Width;
	m_TexInfo.height = sDesc.Height;
	
	m_TexInfo.level = static_cast<UN_UINT8>(m_pTexture->GetLevelCount());
	m_TexInfo.fmt = UN_GET_ABST_PIXEL_FMT(sDesc.Format);

	m_TexInfo.is_rendertarget = (sDesc.Usage == D3DUSAGE_RENDERTARGET);
	m_TexInfo.is_dynamic = ((sDesc.Usage & D3DUSAGE_DYNAMIC) > 0);
	m_TexInfo.is_on_sysmem = (sDesc.Pool == D3DPOOL_SYSTEMMEM);
	m_TexInfo.is_on_vram = (sDesc.Pool == D3DPOOL_DEFAULT);
}

// �T�[�t�F�X�쐬
UN_BOOL CTexture::CreateSurface()
{
	// �T�[�t�F�X�쐬
	if (m_pSurface != UN_NULL) {
		for (UN_UINT i = 0; i < m_TexInfo.level; i++) {
			m_pSurface[i] = CSurface::CreateSurface(m_pAllocator);
			UN_BOOL result = (m_pSurface != UN_NULL);

			if (result) {
				result = m_pSurface[i]->Reset(this, i);
				VRETURN(result);
			}
			else {
				UN_ASSERT(UN_FALSE);
				return UN_FALSE;
			}
		}
	}

	return UN_TRUE;
}

/**
* ���b�N
*/
UN_UINT CTexture::Lock(
	UINT nLevel,
	void** data,
	UN_BOOL bIsReadOnly,
	UN_BOOL bIsDiscard/*= UN_FALSE*/)
{
	// NOTE
	// ���I�e�N�X�`���ɂ�����
	// �~�b�v�}�b�v�̏ꍇ�A�ŏ�ʃ��x���ł̂� LOCK_DISCARD ���g����
	// �ŏ�ʃ��x�������b�N���邾���ł��ׂẴ��x�����j�������

	UN_ASSERT(m_pTexture != UN_NULL);

#if 0
	if (!IsDynamic() && !IsOnSysMem()) {
		// ���I�e�N�X�`���ȊO�ł̓��b�N�s��
		// �������A�V�X�e����������ɂ���Ƃ��͂n�j
		UN_ASSERT(UN_FALSE);
		return 0;
	}
#endif

	if (nLevel >= m_TexInfo.level) {
		// ���x���w��I�[�o�[
		UN_ASSERT(UN_FALSE);
		return 0;
	}

	DWORD flag = 0;
	if (IsDynamic()) {
		// READONLY�s��
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
	UN_ASSERT(SUCCEEDED(hr));

	UINT ret = 0;

	if (SUCCEEDED(hr)) {
		*data = rect.pBits;
		ret = rect.Pitch;
	}

	return ret;
}

/**
* �A�����b�N
*/
UN_BOOL CTexture::Unlock(UN_UINT nLevel)
{
	UN_ASSERT(m_pTexture != UN_NULL);

	HRESULT hr = m_pTexture->UnlockRect(nLevel);
	UN_BOOL ret = SUCCEEDED(hr);
	UN_ASSERT(ret);

	return ret;
}

// �T�[�t�F�X�擾
CSurface* CTexture::GetSurface(UN_UINT idx)
{
	UN_ASSERT(idx < GetMipMapNum());
	UN_ASSERT(m_pSurface != UN_NULL);

	return m_pSurface[idx];
}

// ���Z�b�g
UN_BOOL CTexture::Reset()
{
	UN_BOOL ret = UN_TRUE;

	if (IsRenderTarget()) {
		// RenderTarget�͋����I�Ƀ��Z�b�g

		SAFE_RELEASE(m_pTexture);

		ret = CreateBody_RenderTarget(
				GetWidth(),
				GetHeight(),
				GetPixelFormat(),
				GetMipMapNum());
		UN_ASSERT(ret);

		if (ret) {
			// �T�[�t�F�X�����Z�b�g
			for (UN_UINT32 i = 0; i < GetMipMapNum(); i++) {
				ret = m_pSurface[i]->Reset(this, i);
				UN_ASSERT(ret);
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
				E_GRAPH_RSC_CREATE_TYPE_DYNAMIC,
				IsOnSysMem());
		UN_ASSERT(ret);
	}

	return ret;
}
