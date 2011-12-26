#if !defined(__IZANAGI_GRAPH_BASE_TEXTURE_H__)
#define __IZANAGI_GRAPH_BASE_TEXTURE_H__

#include "izStd.h"
#include "izD3DDefs.h"
#include "graph/GraphDefs.h"

namespace izanagi {
	class CGraphicsDevice;

	/**
	* ベーステクスチャ
	*/
	class CBaseTexture : public CObject {
		friend class CGraphicsDevice;

	protected:
		CBaseTexture();
		virtual ~CBaseTexture();

		CBaseTexture(const CBaseTexture& rhs);
		const CBaseTexture& operator=(const CBaseTexture& rhs);

	public:
		// 幅取得
		inline IZ_UINT GetWidth(IZ_UINT level = 0) const;

		// 高さ取得
		inline IZ_UINT GetHeight(IZ_UINT level = 0) const;

		// フォーマット取得
		inline E_GRAPH_PIXEL_FMT GetPixelFormat() const;

		// MIPMAP数取得
		inline IZ_UINT GetMipMapNum() const;

		// フィルタセット
		inline void SetFilter(
			E_GRAPH_TEX_FILTER minFilter,
			E_GRAPH_TEX_FILTER magFilter,
			E_GRAPH_TEX_FILTER mipFilter);

		// アドレッシングセット
		inline void SetAddress(
			E_GRAPH_TEX_ADDRESS addressU,
			E_GRAPH_TEX_ADDRESS addressV);

		// サンプラステート取得
		inline const S_SAMPLER_STATE& GetState() const;

		inline IZ_BOOL IsRenderTarget() const;
		inline IZ_BOOL IsDynamic() const;

	protected:
		inline IZ_BOOL IsOnSysMem() const;
		inline IZ_BOOL IsOnVram() const;

	public:
		virtual TEX_HANDLE GetTexHandle() = 0;

	protected:
		CGraphicsDevice* m_pDevice;
		IMemoryAllocator* m_pAllocator;

		// テクスチャ情報
		struct {
			IZ_UINT16 width;
			IZ_UINT16 height;
			
			IZ_UINT8  level;
			IZ_UINT8  depth;

			struct {
				IZ_UINT16 is_dynamic		: 1;	// 動的テクスチャかどうか
				IZ_UINT16 is_on_sysmem		: 1;	// システムメモリ上のみにあるかどうか
				IZ_UINT16 is_on_vram		: 1;	// VRAM上のみにあるかどうか
				IZ_UINT16 is_rendertarget	: 1;	// レンダーターゲットかどうか
			};

			E_GRAPH_PIXEL_FMT fmt;
		} m_TexInfo;		

		// ステート
		S_SAMPLER_STATE m_sState;
	};

	// inline ***********************************************

	// 幅取得
	IZ_UINT CBaseTexture::GetWidth(IZ_UINT level/*= 0*/) const
	{
		IZ_ASSERT(level < m_TexInfo.level);
		IZ_UINT ret = (m_TexInfo.width >> level);
		return ret;
	}

	// 高さ取得
	IZ_UINT CBaseTexture::GetHeight(IZ_UINT level/*= 0*/) const
	{
		IZ_ASSERT(level < m_TexInfo.level);
		IZ_UINT ret = (m_TexInfo.height >> level);
		return ret;
	}

	// フォーマット取得
	E_GRAPH_PIXEL_FMT CBaseTexture::GetPixelFormat() const
	{
		return m_TexInfo.fmt;
	}

	// MIPMAP数取得
	IZ_UINT CBaseTexture::GetMipMapNum() const
	{
		return m_TexInfo.level;
	}

	// フィルタセット
	void CBaseTexture::SetFilter(
		E_GRAPH_TEX_FILTER minFilter,
		E_GRAPH_TEX_FILTER magFilter,
		E_GRAPH_TEX_FILTER mipFilter)
	{
		m_sState.minFilter = minFilter;
		m_sState.magFilter = magFilter;
		m_sState.mipFilter = mipFilter;
	}

	// アドレッシングセット
	void CBaseTexture::SetAddress(
		E_GRAPH_TEX_ADDRESS addressU,
		E_GRAPH_TEX_ADDRESS addressV)
	{
		m_sState.addressU = addressU;
		m_sState.addressV = addressV;
	}

	// テクスチャステート取得
	const S_SAMPLER_STATE& CBaseTexture::GetState() const
	{
		return m_sState;
	}

	IZ_BOOL CBaseTexture::IsRenderTarget() const
	{
		return m_TexInfo.is_rendertarget;
	}

	IZ_BOOL CBaseTexture::IsDynamic() const
	{
		return m_TexInfo.is_dynamic;
	}

	IZ_BOOL CBaseTexture::IsOnSysMem() const
	{
		return m_TexInfo.is_on_sysmem;
	}

	IZ_BOOL CBaseTexture::IsOnVram() const
	{
		return m_TexInfo.is_on_vram;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_BASE_TEXTURE_H__)
