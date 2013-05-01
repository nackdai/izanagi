#if !defined(__IZANAGI_GRAPH_SURFACE_GLES2_H__)
#define __IZANAGI_GRAPH_SURFACE_GLES2_H__

#include "graph/Surface.h"
#include "izGLES2Defs.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;
    class CTexture;

    // サーフェース
    class CSurfaceGLES2 : public CSurface
    {
        friend class CTextureGLES2;
        friend class CGraphicsDeviceGLES2;

    private:
        // サーフェース作成.
        static CSurface* CreateDepthStencilSurface(
            IMemoryAllocator* allocator,
            CGraphicsDeviceGLES2* device,
            IZ_UINT width, 
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt);

        // サーフェス作成
        static CSurfaceGLES2* CreateSurface(IMemoryAllocator* allocator);

    protected:
        inline CSurfaceGLES2();
        virtual inline ~CSurfaceGLES2();

    public:
        virtual IZ_UINT GetWidth() const;

        virtual IZ_UINT GetHeight() const;

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
        GLuint* GetRawInterface() { return &m_Surface; }

    private:
        // 本体
        GLuint m_Surface;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_SURFACE_GLES2_H__)
