#if !defined(__IZANAGI_GRAPH_SURFACE_DX9_H__)
#define __IZANAGI_GRAPH_SURFACE_DX9_H__

#include "graph/Surface.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceDX9;
	class CTexture;

    // サーフェース
	class CSurfaceDX9 : public CSurface
    {
        friend class CTextureDX9;
        friend class CGraphicsDeviceDX9;

	private:
		// サーフェース作成.
		static CSurface* CreateDepthStencilSurface(
			IMemoryAllocator* allocator,
			CGraphicsDeviceDX9* device,
			IZ_UINT width, 
			IZ_UINT height,
			E_GRAPH_PIXEL_FMT fmt);

		// サーフェス作成
		static CSurfaceDX9* CreateSurface(IMemoryAllocator* allocator);

	protected:
		inline CSurfaceDX9();
		virtual inline ~CSurfaceDX9();

    public:
		virtual IZ_UINT GetWidth() const
        {
            return m_Desc.Width;
        }

		virtual IZ_UINT GetHeight() const
        {
            return m_Desc.Height;
        }

        virtual void* GetPlatformInterface()
        {
            return GetRawInterface();
        }

	public:
		// リセット
		virtual IZ_BOOL Reset(
			CTexture* texture,
			IZ_UINT level);

		// うーん・・・
		IZ_BOOL SetDesc();

		void ReleaseResource();

	public:
		D3D_SURFACE* GetRawInterface() { return m_Surface; }

	private:
		// 本体
		D3D_SURFACE* m_Surface;

		// 記述
		D3DSURFACE_DESC m_Desc;
	};
}   // namespace graph
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_SURFACE_DX9_H__)
