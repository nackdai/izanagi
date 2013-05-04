#if !defined(__IZANAGI_GRAPH_TEXTURE_GLES2_H__)
#define __IZANAGI_GRAPH_TEXTURE_GLES2_H__

#include "izGLES2Defs.h"
#include "graph/Texture.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;

    class CTextureGLES2 : public CTexture
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // データからテクスチャ作成
        static CTextureGLES2* CreateTextureFromMemory(
            IMemoryAllocator* allocator,
            const void* data,
            IZ_UINT dataSize,
            E_GRAPH_PIXEL_FMT fmt);

        // テクスチャ作成
        static CTextureGLES2* CreateTexture(
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            E_GRAPH_RSC_USAGE rscType);

    private:
        inline CTextureGLES2();
        virtual inline ~CTextureGLES2();

    private:
        // 本体作成（テクスチャ）
        IZ_BOOL CreateBody_Texture(
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            E_GRAPH_RSC_USAGE usage);

        void SetTextureInfo(
            IZ_BOOL isRT,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            E_GRAPH_RSC_USAGE rscType);

        // サーフェス作成
        IZ_BOOL CreateSurface();

    public:
        // ロック
        virtual IZ_UINT Lock(
            UINT level,
            void** data,
            IZ_BOOL isReadOnly,
            IZ_BOOL isDiscard = IZ_FALSE);

        // アンロック
        virtual IZ_BOOL Unlock(IZ_UINT level);

    public:
        virtual IZ_BOOL IsPrepared() const;

        virtual IZ_BOOL Disable();

        virtual IZ_BOOL Restore();

    private:
        virtual TEX_HANDLE GetTexHandle()
        {
            return m_Texture;
        }

    public:
        GLuint GetRawInterface() { return m_Texture; }

    private:
        // 本体
        GLuint m_Texture;

        void* m_TemporaryData;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_TEXTURE_GLES2_H__)
