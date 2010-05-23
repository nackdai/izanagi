#if !defined(__URANUS_GRAPH_SURFACE_H__)
#define __URANUS_GRAPH_SURFACE_H__

#include "unStd.h"
#include "unD3DDefs.h"

namespace uranus {
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
		UN_BOOL Reset(
			CTexture* pTexture,
			UN_UINT nLevel);

	private:
		inline void ReleaseResource();

	public:
		inline UN_UINT GetWidth() const;
		inline UN_UINT GetHeight() const;
		inline D3DFORMAT GetPixelFormat() const;

	public:
		D3D_SURFACE* GetRawInterface() { return m_pSurface; }

		// うーん・・・
		inline UN_BOOL SetDesc();

	private:
		IMemoryAllocator* m_pAllocator;

		// 本体
		D3D_SURFACE* m_pSurface;

		// 記述
		D3DSURFACE_DESC m_Desc;
	};

	// inline ********************************

	UN_UINT CSurface::GetWidth() const
	{
		return m_Desc.Width;
	}

	UN_UINT CSurface::GetHeight() const
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
	UN_BOOL CSurface::SetDesc()
	{
		UN_BOOL ret = UN_FALSE;
		if (m_pSurface != UN_NULL) {
			HRESULT hr = m_pSurface->GetDesc(&m_Desc);
			ret = SUCCEEDED(hr);
		}
		return ret;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_GRAPH_SURFACE_H__)
