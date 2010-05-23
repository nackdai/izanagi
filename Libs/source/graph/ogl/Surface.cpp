#include "graph/dx9/Surface.h"
#include "graph/dx9/Texture.h"
#include "std/MemoryAllocator.h"

using namespace uranus;

CSurface* CSurface::CreateSurface(IMemoryAllocator* pAllocator)
{
	UN_BOOL result = UN_TRUE;
	UN_UINT8* pBuf = UN_NULL;
	CSurface* pInstance = UN_NULL;

	// �������m��
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CSurface));
	if (!(result = (pBuf != UN_NULL))) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// �C���X�^���X�쐬
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

// �R���X�g���N�^
CSurface::CSurface()
{
	m_pAllocator = UN_NULL;

	m_pSurface = UN_NULL;

	FILL_ZERO(&m_Desc, sizeof(m_Desc));
}

// �f�X�g���N�^
CSurface::~CSurface()
{
	SAFE_RELEASE(m_pSurface);
}

// ���
void CSurface::InternalRelease()
{
	delete this;

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}
}

// ���Z�b�g
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
		// �ŏ�ʃ��x���̂�
		HRESULT hr = pD3DTex->GetSurfaceLevel(nLevel, &m_pSurface);

		UN_BOOL ret = SUCCEEDED(hr);
		UN_ASSERT(ret);
	}

	if (ret && (m_pSurface != UN_NULL)) {
		// �L�q�擾
		HRESULT hr = m_pSurface->GetDesc(&m_Desc);
		ret = SUCCEEDED(hr);
		UN_ASSERT(ret);
	}

	return ret;
}
