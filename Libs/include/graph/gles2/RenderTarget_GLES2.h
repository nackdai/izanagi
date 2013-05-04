#if !defined(__IZANAGI_GRAPH_RENDER_TARGET_GLES2_H__)
#define __IZANAGI_GRAPH_RENDER_TARGET_GLES2_H__

#include "izGLES2Defs.h"
#include "graph/RenderTarget.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;

    class CRenderTargetGLES2 : public CRenderTarget
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // レンダーターゲット作成
        static CRenderTargetGLES2* CreateRenderTarget(
            CGraphicsDeviceGLES2* device,
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt);

    private:
        inline CRenderTargetGLES2();
        virtual inline ~CRenderTargetGLES2();

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
        virtual TEX_HANDLE GetTexHandle()
        {
            return m_Texture;
        }

    private:
        CGraphicsDeviceGLES2* m_Device;

        // 本体
        GLuint m_Texture;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_RENDER_TARGET_GLES2_H__)
