#if !defined(__IZANAGI_TOOL_KIT_GRAPH_TEXTURE_LIGHT_H__)
#define __IZANAGI_TOOL_KIT_GRAPH_TEXTURE_LIGHT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"

namespace izanagi {
namespace izanagi_tk {
	class CGraphicsDeviceLite;

	/**
	*/
	class CTextureLite : public CObject {
		friend class CGraphicsDeviceLite;

	private:
		// ファイルからテクスチャ作成
		static CTextureLite* CreateTextureFromFile(
			CGraphicsDeviceLite* pDevice,
			IMemoryAllocator* pAllocator,
			IZ_PCSTR lpszPath);

		// テクスチャ作成
		static CTextureLite* CreateTexture(
			CGraphicsDeviceLite* pDevice,
			IMemoryAllocator* pAllocator,
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			D3DFORMAT fmt);

		// テクスチャ保存
		static IZ_BOOL SaveTexture(
			IZ_PCSTR path,
			CTextureLite* texture);

	private:
		inline CTextureLite();
		inline ~CTextureLite();

		NO_COPIABLE(CTextureLite);

	private:
		// 解放
		inline void InternalRelease();
		
	public:
		// ロック
		IZ_UINT Lock(
			UINT nLevel,
			void** data);

		// アンロック
		IZ_BOOL Unlock(IZ_UINT nLevel);

	public:
		// 幅取得
		inline IZ_UINT GetWidth(IZ_UINT level = 0) const;

		// 高さ取得
		inline IZ_UINT GetHeight(IZ_UINT level = 0) const;

		// フォーマット取得
		inline E_GRAPH_PIXEL_FMT GetPixelFormat() const;

		// MIPMAP数取得
		inline IZ_UINT GetMipMapNum() const;

		// オリジナルフォーマット取得
		D3DFORMAT GetOriginalFormat() const
		{
			return m_Desc.Format;
		}

	public:
		D3D_TEXTURE* GetRawInterface() { return m_pTexture; }

	private:
		IMemoryAllocator* m_pAllocator;

		// 本体
		D3D_TEXTURE* m_pTexture;

		D3DSURFACE_DESC m_Desc;
		IZ_UINT m_nMipLevels;
		E_GRAPH_PIXEL_FMT m_nFmt;
	};

	// inline ***********************************

	// コンストラクタ
	CTextureLite::CTextureLite()
	{
		m_pAllocator = IZ_NULL;
		m_pTexture = IZ_NULL;

		memset(&m_Desc, 0, sizeof(m_Desc));
		m_nMipLevels = 0;
		m_nFmt = E_GRAPH_PIXEL_FMT_RGBA8;
	}

	// デストラクタ
	CTextureLite::~CTextureLite()
	{	
		SAFE_RELEASE(m_pTexture);
	}

	// 解放
	void CTextureLite::InternalRelease()
	{
		delete this;

		if (m_pAllocator != IZ_NULL) {
			m_pAllocator->Free(this);
		}
	}

	// 幅取得
	IZ_UINT CTextureLite::GetWidth(IZ_UINT level/*= 0*/) const
	{
		IZ_ASSERT(level < m_nMipLevels);
		IZ_UINT ret = (m_Desc.Width >> level);
		return ret;
	}

	// 高さ取得
	IZ_UINT CTextureLite::GetHeight(IZ_UINT level/*= 0*/) const
	{
		IZ_ASSERT(level < m_nMipLevels);
		IZ_UINT ret = (m_Desc.Height >> level);
		return ret;
	}

	// フォーマット取得
	E_GRAPH_PIXEL_FMT CTextureLite::GetPixelFormat() const
	{
		return m_nFmt;
	}

	// MIPMAP数取得
	IZ_UINT CTextureLite::GetMipMapNum() const
	{
		return m_nMipLevels;
	}
}	// namespace izanagi_tk
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_TOOL_KIT_GRAPH_TEXTURE_LIGHT_H__)
