#if !defined(__IZANAGI_GRAPH_SURFACE_H__)
#define __IZANAGI_GRAPH_SURFACE_H__

#include "izStd.h"
#include "izD3DDefs.h"

namespace izanagi {
	class IMemoryAllocator;
	class CTexture;

	class CSurface : public CObject {
		friend class CTexture;
		friend class CGraphicsDevice;

	private:
		// サーフェス作成
		static CSurface* CreateSurface(IMemoryAllocator* pAllocator);

	private:
		CSurface();
		~CSurface();

		CSurface(const CSurface& rhs);
		const CSurface& operator=(const CSurface& rhs);

	private:
		// 解放
		void InternalRelease();

		// リセット
		IZ_BOOL Reset(
			CTexture* pTexture,
			IZ_UINT nLevel);

	private:
		inline void ReleaseResource();

	public:
		inline IZ_UINT GetWidth() const;
		inline IZ_UINT GetHeight() const;
		inline D3DFORMAT GetPixelFormat() const;

	public:
		D3D_SURFACE* GetRawInterface() { return m_pSurface; }

		// うーん・・・
		inline IZ_BOOL SetDesc();

	private:
		IMemoryAllocator* m_pAllocator;

		// 本体
		D3D_SURFACE* m_pSurface;

		// 記述
		D3DSURFACE_DESC m_Desc;
	};

	// inline ********************************

	IZ_UINT CSurface::GetWidth() const
	{
		return m_Desc.Width;
	}

	IZ_UINT CSurface::GetHeight() const
	{
		return m_Desc.Height;
	}

	D3DFORMAT CSurface::GetPixelFormat() const
	{
		return m_Desc.Format;
	}

	void CSurface::ReleaseResource()
	{
		SAFE_RELEASE(m_pSurface);
	}

	// うーん・・・
	IZ_BOOL CSurface::SetDesc()
	{
		IZ_BOOL ret = IZ_FALSE;
		if (m_pSurface != IZ_NULL) {
			HRESULT hr = m_pSurface->GetDesc(&m_Desc);
			ret = SUCCEEDED(hr);
		}
		return ret;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_SURFACE_H__)
