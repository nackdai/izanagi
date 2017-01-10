#if !defined(__IZANAGI_GRAPH_TEXTURE_OGL_H__)
#define __IZANAGI_GRAPH_TEXTURE_OGL_H__

#include "graph/gles2/Texture_GLES2.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceOGL;

    class CTextureOGL : public CTextureGLES2
    {
        friend class CGraphicsDeviceOGL;

    private:
        // データからテクスチャ作成
        static CTextureOGL* CreateTextureFromMemory(
            CGraphicsDeviceOGL* device,
            IMemoryAllocator* allocator,
            const void* data,
            IZ_UINT dataSize,
            E_GRAPH_PIXEL_FMT fmt);

        // テクスチャ作成
        static CTextureOGL* CreateTexture(
            CGraphicsDeviceOGL* device,
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            E_GRAPH_RSC_USAGE rscType);

    private:
        CTextureOGL() {}
        virtual ~CTextureOGL() {}

    public:
        // アンロック
        virtual IZ_BOOL Unlock(IZ_UINT level) override;

        virtual IZ_BOOL Write(
            IZ_UINT level,
            void* data,
            IZ_UINT x, IZ_UINT y,
            IZ_UINT width, IZ_UINT height) override;

        virtual void overrideNativeResource(void* rsc, E_GRAPH_OVERRIDE_NATIVE_RSC_BEHAVIOUR behaviour) override;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_TEXTURE_OGL_H__)
