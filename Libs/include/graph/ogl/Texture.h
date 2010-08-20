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
		CTexture();
		~CTexture();

		NO_COPIABLE(CTexture);

	private:
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

		// サーフェス作成
		UN_BOOL CreateSurface();

		// 本体解放
		void ReleaseResource();

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

		UN_UINT GetRawInterface() { return m_nTexture; }

	public:
		inline TEX_HANDLE GetTexHandle();

	private:
		// 本体
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

		// サーフェス
		CSurface** m_pSurface;

		CTexture* m_pNext;
	};

	// inline ***********************************

	// 解放
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
