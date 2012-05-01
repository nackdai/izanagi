#if !defined(__IZANAGI_GRAPH_TEXTURE_H__)
#define __IZANAGI_GRAPH_TEXTURE_H__

#include "BaseTexture.h"
#include "GraphicsDevice.h"

namespace izanagi {
	class CSurface;

	class CTexture : public CBaseTexture {
		friend class CGraphicsDevice;

	private:
		// ファイルからテクスチャ作成
		static CTexture* CreateTextureFromFile(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			IZ_PCSTR lpszPathName,
			E_GRAPH_PIXEL_FMT fmt);

		// データからテクスチャ作成
		static CTexture* CreateTextureFromMemory(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			const void* pData,
			IZ_UINT nDataSize,
			E_GRAPH_PIXEL_FMT fmt);

		// テクスチャ作成
		static CTexture* CreateTexture(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT fmt,
			E_GRAPH_RSC_TYPE nRscType,
			IZ_BOOL bIsOnSysMem = IZ_FALSE);

		// レンダーターゲット作成
		static CTexture* CreateRenderTarget(
			CGraphicsDevice* pDevice,
			IMemoryAllocator* pAllocator,
			IZ_UINT nWidth,
			IZ_UINT nHeight,
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
			IZ_UINT nInputSize,
			_Func& func,
			E_GRAPH_PIXEL_FMT fmt);

		// 本体作成（テクスチャ）
		IZ_BOOL CreateBody_Texture(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			IZ_UINT nMipLevel,
			E_GRAPH_PIXEL_FMT nFmt,
			E_GRAPH_RSC_TYPE nCreateType,
			IZ_BOOL bIsOnSysMem);

		// 本体作成（レンダーターゲット）
		IZ_BOOL CreateBody_RenderTarget(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			E_GRAPH_PIXEL_FMT nFmt,
			IZ_UINT nMipLevel);

		// テクスチャ情報取得
		void GetTextureInfo();

		// サーフェス作成
		IZ_BOOL CreateSurface();

	public:
		// ロック
		IZ_UINT Lock(
			UINT nLevel,
			void** data,
			IZ_BOOL bIsReadOnly,
			IZ_BOOL bIsDiscard = IZ_FALSE);

		// アンロック
		IZ_BOOL Unlock(IZ_UINT nLevel);

		// サーフェス取得
		CSurface* GetSurface(IZ_UINT idx);

		// リセット
		IZ_BOOL Reset();

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
		m_pTexture = IZ_NULL;
		m_pSurface = IZ_NULL;

		m_pNext = IZ_NULL;
	}

	// デストラクタ
	CTexture::~CTexture()
	{
		m_pDevice->RemoveTexture(this);
		
		SAFE_RELEASE(m_pTexture);

		if (m_pSurface != NULL) {
			for (IZ_UINT i = 0; i < GetMipMapNum(); ++i) {
				SAFE_RELEASE(m_pSurface[i]);
			}
		}

		SAFE_RELEASE(m_pDevice);
	}

	// 解放
	void CTexture::InternalRelease()
	{
		delete this;

		if (m_Allocator != IZ_NULL) {
			m_Allocator->Free(this);
		}
	}

	// 本体解放
	void CTexture::ReleaseResource()
	{
		if (m_pSurface != IZ_NULL) {
			for (IZ_UINT32 i = 0; i < GetMipMapNum(); ++i) {
				m_pSurface[i]->ReleaseResource();
			}
		}

		SAFE_RELEASE(m_pTexture);
	}

	TEX_HANDLE CTexture::GetTexHandle()
	{
		return m_pTexture;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_TEXTURE_H__)
