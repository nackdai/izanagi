#if !defined(__IZANAGI_GRAPH_RENDER_TARGET_DX9_H__)
#define __IZANAGI_GRAPH_RENDER_TARGET_DX9_H__

#include "izD3DDefs.h"
#include "graph/RenderTarget.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceDX9;
    class CSurfaceDX9;

    class CRenderTargetDX9 : public CRenderTarget
    {
        friend class CGraphicsDeviceDX9;

    private:
        // レンダーターゲット作成
        static CRenderTargetDX9* CreateRenderTarget(
            CGraphicsDeviceDX9* device,
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt);

        // レンダーターゲット作成
        static CRenderTargetDX9* CreateRenderTarget(
            CGraphicsDeviceDX9* device,
            IMemoryAllocator* allocator,
            CSurfaceDX9* surface);

    private:
        inline CRenderTargetDX9();
        virtual inline ~CRenderTargetDX9();

    private:
        // 本体作成（レンダーターゲット）
        IZ_BOOL CreateBody_RenderTarget(
            IZ_UINT width,
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt);

        // テクスチャ情報取得
        void SetDesc();

        // サーフェス作成
        IZ_BOOL CreateSurface();

    public:
        virtual IZ_BOOL IsPrepared() const;

        virtual IZ_BOOL Disable();

        virtual IZ_BOOL Restore();

    private:
        CSurfaceDX9* GetSurface();

        virtual TEX_HANDLE GetTexHandle()
        {
            return m_Texture;
        }

    public:
        D3D_TEXTURE* GetRawInterface() { return m_Texture; }

    private:
        CGraphicsDeviceDX9* m_Device;

        // 本体
        D3D_TEXTURE* m_Texture;

        // サーフェス
        CSurfaceDX9* m_Surface;

        CRenderTargetDX9* m_Next;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_RENDER_TARGET_DX9_H__)
