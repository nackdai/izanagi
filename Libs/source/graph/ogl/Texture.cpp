#include "graph/ogl/Texture.h"
#include "graph/ogl/Surface.h"
#include "graph/ogl/OGLParamValueConverter.h"
#include "graph/ogl/GraphicsDevice.h"
#include "std/allocator/MemoryAllocator.h"
#include "graph/ogl/OGLExtFuncProxy.h"

using namespace uranus;

// �t�@�C������e�N�X�`���쐬
CTexture* CTexture::CreateTextureFromFile(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	UN_PCSTR lpszPathName,
	E_GRAPH_PIXEL_FMT fmt)
{
	// Nothing is done.
	UN_ASSERT(UN_FALSE);
	return UN_NULL;
}

// �f�[�^����e�N�X�`���쐬
CTexture* CTexture::CreateTextureFromMemory(
	CGraphicsDevice* pDevice,
	IMemoryAllocator* pAllocator,
	const void* pData,
	UN_UINT nDataSize,
	E_GRAPH_PIXEL_FMT fmt)
{
	// Nothing is done.
	UN_ASSERT(UN_FALSE);
	return UN_NULL;
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

// �R���X�g���N�^
CTexture::CTexture()
{
	m_nTexture = 0;

	m_LockInfo.Clear();

	m_pSurface = UN_NULL;

	m_pNext = UN_NULL;
}

// �f�X�g���N�^
CTexture::~CTexture()
{
	m_pDevice->RemoveTexture(this);

	FREE(m_pAllocator, m_LockInfo.data);
	
	if (m_nTexture > 0) {
		glDeleteTextures(1, &m_nTexture);
		m_nTexture = 0;
	}

	if (m_pSurface != NULL) {
		for (UN_UINT i = 0; i < GetMipMapNum(); i++) {
			SAFE_RELEASE(m_pSurface[i]);
		}
	}

	SAFE_RELEASE(m_pDevice);
}

namespace {
	inline UN_BOOL _IsCompressedFormat(E_GRAPH_PIXEL_FMT fmt)
	{
		return ((fmt == E_GRAPH_PIXEL_FMT_DXT1)
				|| (fmt == E_GRAPH_PIXEL_FMT_DXT3)
				|| (fmt == E_GRAPH_PIXEL_FMT_DXT5));
	}
}	// namespace

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

	glGenTextures(1, &m_nTexture);
	VRETURN(m_nTexture > 0);

	glBindTexture(GL_TEXTURE_2D, m_nTexture);
	
	UN_UINT nPixelFmt = UN_GET_TARGET_PIXEL_FMT(nFmt);
	GLenum fmt = UN_OGL_GET_PIXEL_FMT(nPixelFmt);
	GLenum type = UN_OGL_GET_PIXEL_TYPE(nPixelFmt);

	// TODO
	// �{�̍쐬
	if (_IsCompressedFormat(nFmt)) {
	}
	else {
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	// �e�N�X�`�����ݒ�
	{
		m_TexInfo.width = nWidth;
		m_TexInfo.height = nHeight;
		
		m_TexInfo.level = nMipLevel;
		m_TexInfo.fmt = nFmt;

		m_TexInfo.is_rendertarget = UN_FALSE;
		m_TexInfo.is_dynamic = UN_TRUE;
		m_TexInfo.is_on_sysmem = UN_FALSE;
		m_TexInfo.is_on_vram = UN_TRUE;
	}

	return UN_TRUE;
}

// �{�̍쐬�i�����_�[�^�[�Q�b�g�j
UN_BOOL CTexture::CreateBody_RenderTarget(
	UN_UINT nWidth, 
	UN_UINT nHeight,
	E_GRAPH_PIXEL_FMT nFmt,
	UN_UINT nMipLevel)
{
	UN_ASSERT(!_IsCompressedFormat(nFmt));

	// �e�N�X�`���쐬
	UN_BOOL ret = CreateBody_Texture(
					nWidth,
					nHeight,
					nMipLevel,
					nFmt,
					E_GRAPH_RSC_CREATE_TYPE_STATIC,	// TODO
					UN_FALSE);

	m_TexInfo.is_rendertarget = UN_TRUE;

	return ret;
}

// �T�[�t�F�X�쐬
UN_BOOL CTexture::CreateSurface()
{
	// �T�[�t�F�X�쐬
	if (m_pSurface != UN_NULL) {
		UN_ASSERT(m_TexInfo.level == 1);

		for (UN_UINT i = 0; i < m_TexInfo.level; i++) {
			m_pSurface[i] = CSurface::CreateSurface(m_pAllocator);
			UN_BOOL result = (m_pSurface != UN_NULL);

			if (result) {
				result = m_pSurface[i]->Reset(this, i);
				VRETURN(result);

				UN_OGL_EXT_PROC(glBindFramebuffer)(
					GL_FRAMEBUFFER_EXT,
					m_pSurface[i]->GetRawInterface());

				// FBO �ƃe�N�X�`�����o�C���h
				UN_OGL_EXT_PROC(glFramebufferTexture2D)(
					GL_FRAMEBUFFER_EXT,
					GL_COLOR_ATTACHMENT0_EXT,
					GL_TEXTURE_2D,
					m_nTexture,
					0);

				// FBO �Ɛ[�x�o�b�t�@���o�C���h
				UN_OGL_EXT_PROC(glFramebufferTexture2D)(
					GL_FRAMEBUFFER_EXT,
					GL_DEPTH_ATTACHMENT_EXT,
					GL_TEXTURE_2D,
					m_pSurface[i]->GetDepthBuffer(),
					0);

				UN_OGL_EXT_PROC(glBindFramebuffer)(
					GL_FRAMEBUFFER_EXT,
					0);
			}
			else {
				UN_ASSERT(UN_FALSE);
				return UN_FALSE;
			}
		}
	}

	return UN_TRUE;
}

// �{�̉��
void CTexture::ReleaseResource()
{
	// Nothing is done.
	UN_ASSERT(UN_FALSE);
}

namespace {
	inline UN_UINT _ComputePitch(
		UN_UINT nWidth,
		E_GRAPH_PIXEL_FMT fmt)
	{
		UN_UINT nBPP = 0;

		switch (fmt) {
		case E_GRAPH_PIXEL_FMT_RGBA8:
		case E_GRAPH_PIXEL_FMT_BGRA8:
		case E_GRAPH_PIXEL_FMT_RGB10A2:
		case E_GRAPH_PIXEL_FMT_R32F:
			nBPP = 4;
			break;
		case E_GRAPH_PIXEL_FMT_RGBA4:
			nBPP = 2;
			break;
		case E_GRAPH_PIXEL_FMT_A8:
			nBPP = 1;
			break;
		case E_GRAPH_PIXEL_FMT_RGBA16F:
			nBPP = 8;
			break;
		case E_GRAPH_PIXEL_FMT_RGBA32F:
			nBPP = 16;
			break;
		default:
			UN_ASSERT(UN_FALSE);
			break;
		}

		UN_UINT ret = nBPP * nWidth;

		return ret;
	}

	inline UN_UINT _ComputeTexSize(
		UN_UINT nWidth,
		UN_UINT nHeight,
		E_GRAPH_PIXEL_FMT fmt)
	{
		UN_UINT nPitch = _ComputePitch(nWidth, fmt);
		UN_UINT ret = nPitch * nHeight;
		return ret;
	}
}	// namespace

/**
* ���b�N
*/
UN_UINT CTexture::Lock(
	UINT nLevel,
	void** data,
	UN_BOOL bIsReadOnly,
	UN_BOOL bIsDiscard/*= UN_FALSE*/)
{
	UN_ASSERT(m_nTexture > 0);
	UN_ASSERT(!m_LockInfo.IsLock());

	// NOTE
	// OpenGLES �̓o�b�t�@�̓ǂݍ��݂͂ł��Ȃ��̂�
	// OpenGL �ł��ł��Ȃ��悤�ɂ���
	UN_ASSERT(!bIsReadOnly);

	// NOTE
	// OpenGL �̎d�l�Ƃ��� Discard �Ƃ����T�O���Ȃ��i�͂��j�̂Ŗ�������

	if (nLevel >= m_TexInfo.level) {
		// ���x���w��I�[�o�[
		UN_ASSERT(UN_FALSE);
		return 0;
	}

	m_LockInfo.level = nLevel;
	m_LockInfo.width = GetWidth(nLevel);
	m_LockInfo.height = GetHeight(nLevel);

	UN_UINT nSize = _ComputeTexSize(
						m_LockInfo.width,
						m_LockInfo.height,
						m_TexInfo.fmt);

	m_LockInfo.data = ALLOC_ZERO(m_pAllocator, nSize);
	UN_ASSERT(m_LockInfo.data != UN_NULL);

	*data = m_LockInfo.data;
	UN_UINT ret = _ComputePitch(
					m_LockInfo.width, 
					m_TexInfo.fmt); 

	return ret;
}

/**
* �A�����b�N
*/
UN_BOOL CTexture::Unlock(UN_UINT nLevel)
{
	UN_ASSERT(m_nTexture > 0);
	UN_ASSERT(m_LockInfo.IsLock());

	glBindTexture(GL_TEXTURE_2D, m_nTexture);

	glTexSubImage2D(
		GL_TEXTURE_2D,
		m_LockInfo.level,
		0,
		0,
		m_LockInfo.width,
		m_LockInfo.height,
		m_TexInfo.glFmt,
		m_TexInfo.glType,
		m_LockInfo.data);

	glBindTexture(GL_TEXTURE_2D, 0);

	FREE(m_pAllocator, m_LockInfo.data);
	
	m_LockInfo.Clear();
	
	return UN_TRUE;
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
	// Nothing is done.
	UN_ASSERT(UN_FALSE);
	return UN_FALSE;
}
