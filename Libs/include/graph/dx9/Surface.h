#if !defined(__IZANAGI_GRAPH_SURFACE_H__)
#define __IZANAGI_GRAPH_SURFACE_H__

#include "izStd.h"
#include "izD3DDefs.h"
#include "../GraphDefs.h"

namespace izanagi {
	class IMemoryAllocator;
	class CTexture;

	class CSurface : public CObject {
		friend class CTexture;
		friend class CGraphicsDevice;

	private:
		// サーフェース作成.
		static CSurface* CreateDepthStencilSurface(
			IMemoryAllocator* allocator,
			CGraphicsDevice* device,
			IZ_UINT width, 
			IZ_UINT height,
			E_GRAPH_PIXEL_FMT fmt);

		// サーフェス作成
		static CSurface* CreateSurface(IMemoryAllocator* pAllocator);

	private:
		CSurface();
		~CSurface();

		NO_COPIABLE(CSurface);

	private:
		// 解放
		void InternalRelease();

		// リセット
		IZ_BOOL Reset(
			CTexture* pTexture,
			IZ_UINT nLevel);

		// うーん・・・
		IZ_BOOL SetDesc();

	private:
		inline void ReleaseResource();

	public:
		inline IZ_UINT GetWidth() const;
		inline IZ_UINT GetHeight() const;
		inline E_GRAPH_PIXEL_FMT GetPixelFormat() const;

	public:
		D3D_SURFACE* GetRawInterface() { return m_pSurface; }

	private:
		IMemoryAllocator* m_Allocator;

		// 本体
		D3D_SURFACE* m_pSurface;

		// 記述
		D3DSURFACE_DESC m_Desc;
		E_GRAPH_PIXEL_FMT m_Fmt;
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

	E_GRAPH_PIXEL_FMT CSurface::GetPixelFormat() const
	{
		return m_Fmt;
	}

	void CSurface::ReleaseResource()
	{
		SAFE_RELEASE(m_pSurface);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_SURFACE_H__)
