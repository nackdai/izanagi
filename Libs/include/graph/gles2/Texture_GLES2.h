#if !defined(__IZANAGI_GRAPH_TEXTURE_GLES2_H__)
#define __IZANAGI_GRAPH_TEXTURE_GLES2_H__

#include "graph/Texture.h"
#include "izGLES2Defs.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceGLES2;
    class CSurfaceGLES2;

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
            E_GRAPH_RSC_TYPE rscType);

        // レンダーターゲット作成
        static CTextureGLES2* CreateRenderTarget(
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt);

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
            E_GRAPH_RSC_TYPE createType);

        // 本体作成（レンダーターゲット）
        IZ_BOOL CreateBody_RenderTarget(
            IZ_UINT width,
            IZ_UINT height,
            E_GRAPH_PIXEL_FMT fmt,
            IZ_UINT mipLevel);

        void SetTextureInfo(
            IZ_BOOL isRT,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            E_GRAPH_RSC_TYPE rscType);

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
        // サーフェス取得
        virtual CSurface* GetSurface(IZ_UINT idx);

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

        // サーフェス
        CSurfaceGLES2** m_Surface;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_TEXTURE_GLES2_H__)
