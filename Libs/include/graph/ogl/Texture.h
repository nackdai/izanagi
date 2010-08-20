#if !defined(__URANUS_GRAPH_TEXTURE_H__)
#define __URANUS_GRAPH_TEXTURE_H__

#include "BaseTexture.h"
//#include "GraphicsDevice.h"

namespace uranus {
	class CSurface;
	class CGraphicsDevice;

	class CTexture : public CBaseTexture {
		friend class CGraphicsDevice;
		friend class CSurface;

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
		CTexture();
		~CTexture();

		NO_COPIABLE(CTexture);

	private:
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

		// �T�[�t�F�X�쐬
		UN_BOOL CreateSurface();

		// �{�̉��
		void ReleaseResource();

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

		UN_UINT GetRawInterface() { return m_nTexture; }

	public:
		inline TEX_HANDLE GetTexHandle();

	private:
		// �{��
		UN_UINT m_nTexture;

		struct {
			void* data;
			UN_UINT level;
			UN_UINT16 width;
			UN_UINT16 height;

			void Clear()
			{
				data = UN_NULL;
				level = 0;
				width = 0;
				height = 0;
			}

			UN_BOOL IsLock() const { return (data != UN_NULL); }
		} m_LockInfo;

		// �T�[�t�F�X
		CSurface** m_pSurface;

		CTexture* m_pNext;
	};

	// inline ***********************************

	// ���
	void CTexture::InternalRelease()
	{
		delete this;

		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	TEX_HANDLE CTexture::GetTexHandle()
	{
		return m_nTexture;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_TEXTURE_H__)
