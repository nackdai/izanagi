#include "graph/ogl/Surface.h"
#include "graph/ogl/Texture.h"
#include "std/allocator/MemoryAllocator.h"
#include "graph/ogl/OGLExtFuncProxy.h"

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

	m_nFBO = 0;
	m_nDepthBuffer = 0;

	FILL_ZERO(&m_Desc, sizeof(m_Desc));
}

// デストラクタ
CSurface::~CSurface()
{
	if (m_nFBO > 0) {
		UN_OGL_EXT_PROC(glDeleteFramebuffers)(1, &m_nFBO);
	}

	if (m_nDepthBuffer > 0) {
		UN_OGL_EXT_PROC(glDeleteRenderbuffers)(1, &m_nDepthBuffer);
	}
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

	if (m_nFBO == 0) {
		UN_OGL_EXT_PROC(glGenFramebuffers)(1, &m_nFBO);
		VRETURN(m_nFBO > 0);
	}

	if (ret && (m_nFBO > 0)) {
		UN_ASSERT(pTexture != UN_NULL);

		// 記述取得
		m_Desc.Width = pTexture->GetWidth();
		m_Desc.Height = pTexture->GetHeight();
		m_Desc.Format = pTexture->GetPixelFormat();

		UN_OGL_EXT_PROC(glGenRenderbuffers)(1, &m_nDepthBuffer);
		UN_ASSERT(m_nDepthBuffer > 0);
	}

	return ret;
}

// うーん・・・
UN_BOOL CSurface::SetDesc()
{
	// Nothing is done.
	UN_ASSERT(UN_FALSE);
	return UN_FALSE;
}
