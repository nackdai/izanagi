#if !defined(__URANUS_GRAPH_TEXTURE_H__)
#define __URANUS_GRAPH_TEXTURE_H__

#include "BaseTexture.h"
#include "GraphicsDevice.h"

namespace uranus {
	class CSurface;

	class CTexture : public CBaseTexture {
		friend class CGraphicsDevice;

	private:
		// �t�@�C������e�N�X�`���쐬
		static CTexture* CreateTextureFromFile(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			UN_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt);

		// �f�[�^����e�N�X�`���쐬
		static CTexture* CreateTextureFromMemory(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			const void* pData,
			UN_UINT nDataSize,
			E_GRAPH_PIXEL_FMT fmt);

		// �e�N�X�`���쐬
		static CTexture* CreateTexture(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			UN_UINT nWidth,
			UN_UINT nHeight,
			UN_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType,
			UN_BOOL bIsOnSysMem = UN_FALSE);

		// �����_�[�^�[�Q�b�g�쐬
		static CTexture* CreateRenderTarget(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			UN_UINT nWidth,
			UN_UINT nHeight,
			E_GRAPH_PIXEL_FMT fmt);

	private:
		inline CTexture();
		inline ~CTexture();

		CTexture(const CTexture& rhs);
		const CTexture& operator=(const CTexture& rhs);

	private:
		template <typename _T, typename _Func>
		static CTexture* CreateBody_From(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			_T tInput,
			UN_UINT nInputSize,
			_Func& func,
			E_GRAPH_PIXEL_FMT fmt);

		// �{�̍쐬�i�e�N�X�`���j
		UN_BOOL CreateBody_Texture(
			UN_UINT nWidth,
			UN_UINT nHeight,
			UN_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT nFmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType,
			UN_BOOL bIsOnSysMem);

		// �{�̍쐬�i�����_�[�^�[�Q�b�g�j
		UN_BOOL CreateBody_RenderTarget(
			UN_UINT nWidth,
			UN_UINT nHeight,
			E_GRAPH_PIXEL_FMT nFmt,
			UN_UINT nMipLevel);

		// �e�N�X�`�����擾
		void GetTextureInfo();

		// �T�[�t�F�X�쐬
		UN_BOOL CreateSurface();

	public:
		// ���b�N
		UN_UINT Lock(
			UINT nLevel,
			void** data,
			UN_BOOL bIsReadOnly,
			UN_BOOL bIsDiscard = UN_FALSE);

		// �A�����b�N
		UN_BOOL Unlock(UN_UINT nLevel);

		// �T�[�t�F�X�擾
		CSurface* GetSurface(UN_UINT idx);

		// ���Z�b�g
		UN_BOOL Reset();

	private:
		// ���
		inline void InternalRelease();

		// �{�̉��
		inline void ReleaseResource();

	public:
		inline TEX_HANDLE GetTexHandle();

	public:
		D3D_TEXTURE* GetRawInterface() { return m_pTexture; }

	private:
		// �{��
		D3D_TEXTURE* m_pTexture;

		// �T�[�t�F�X
		CSurface** m_pSurface;

		CTexture* m_pNext;
	};

	// inline ***********************************

	// �R���X�g���N�^
	CTexture::CTexture()
	{
		m_pTexture = UN_NULL;
		m_pSurface = UN_NULL;

		m_pNext = UN_NULL;
	}

	// �f�X�g���N�^
	CTexture::~CTexture()
	{
		m_pDevice->RemoveTexture(this);
		
		SAFE_RELEASE(m_pTexture);

		if (m_pSurface != NULL) {
			for (UN_UINT i = 0; i < GetMipMapNum(); i++) {
				SAFE_RELEASE(m_pSurface[i]);
			}
		}

		SAFE_RELEASE(m_pDevice);
	}

	// ���
	void CTexture::InternalRelease()
	{
		delete this;

		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	// �{�̉��
	void CTexture::ReleaseResource()
	{
		if (m_pSurface != UN_NULL) {
			for (UN_UINT32 i = 0; i < GetMipMapNum(); i++) {
				m_pSurface[i]->ReleaseResource();
			}
		}

		SAFE_RELEASE(m_pTexture);
	}

	TEX_HANDLE CTexture::GetTexHandle()
	{
		return m_pTexture;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_TEXTURE_H__)
