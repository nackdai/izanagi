#if !defined(__URANUS_GRAPH_TEXTURE_H__)
#define __URANUS_GRAPH_TEXTURE_H__

#include "BaseTexture.h"
#include "GraphicsDevice.h"

namespace uranus {
	class CSurface;

	class CTexture : public CBaseTexture {
		friend class CGraphicsDevice;

	private:
		// ファイルからテクスチャ作成
		static CTexture* CreateTextureFromFile(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			UN_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt);

		// データからテクスチャ作成
		static CTexture* CreateTextureFromMemory(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			const void* pData,
			UN_UINT nDataSize,
			E_GRAPH_PIXEL_FMT fmt);

		// テクスチャ作成
		static CTexture* CreateTexture(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			UN_UINT nWidth,
			UN_UINT nHeight,
			UN_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType,
			UN_BOOL bIsOnSysMem = UN_FALSE);

		// レンダーターゲット作成
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

		// 本体作成（テクスチャ）
		UN_BOOL CreateBody_Texture(
			UN_UINT nWidth,
			UN_UINT nHeight,
			UN_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT nFmt,
			E_GRAPH_RSC_CREATE_TYPE nCreateType,
			UN_BOOL bIsOnSysMem);

		// 本体作成（レンダーターゲット）
		UN_BOOL CreateBody_RenderTarget(
			UN_UINT nWidth,
			UN_UINT nHeight,
			E_GRAPH_PIXEL_FMT nFmt,
			UN_UINT nMipLevel);

		// テクスチャ情報取得
		void GetTextureInfo();

		// サーフェス作成
		UN_BOOL CreateSurface();

	public:
		// ロック
		UN_UINT Lock(
			UINT nLevel,
			void** data,
			UN_BOOL bIsReadOnly,
			UN_BOOL bIsDiscard = UN_FALSE);

		// アンロック
		UN_BOOL Unlock(UN_UINT nLevel);

		// サーフェス取得
		CSurface* GetSurface(UN_UINT idx);

		// リセット
		UN_BOOL Reset();

	private:
		// 解放
		inline void InternalRelease();

		// 本体解放
		inline void ReleaseResource();

	public:
		inline TEX_HANDLE GetTexHandle();

	public:
		D3D_TEXTURE* GetRawInterface() { return m_pTexture; }

	private:
		// 本体
		D3D_TEXTURE* m_pTexture;

		// サーフェス
		CSurface** m_pSurface;

		CTexture* m_pNext;
	};

	// inline ***********************************

	// コンストラクタ
	CTexture::CTexture()
	{
		m_pTexture = UN_NULL;
		m_pSurface = UN_NULL;

		m_pNext = UN_NULL;
	}

	// デストラクタ
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

	// 解放
	void CTexture::InternalRelease()
	{
		delete this;

		if (m_pAllocator != UN_NULL) {
			m_pAllocator->Free(this);
		}
	}

	// 本体解放
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
