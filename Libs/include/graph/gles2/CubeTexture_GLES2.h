#if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_GLES2_H__)
#define __IZANAGI_GRAPH_CUBE_TEXTURE_GLES2_H__

#include "graph/CubeTexture.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDevice;

    // キューブテクスチャ
    class CCubeTextureGLES2 : public CCubeTexture
    {
        friend class CGraphicsDeviceGLES2;

    private:
        // テクスチャ作成
        static CCubeTexture* CreateCubeTexture(
            CGraphicsDevice* device,
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt);

    private:
        typedef IZ_BOOL (CCubeTextureGLES2::*CreateTexFunc)(
            CGraphicsDevice* device,
            IZ_PCSTR, IZ_UINT, IZ_UINT, IZ_UINT, E_GRAPH_PIXEL_FMT);

        static CCubeTexture* CreateInternal(
            CGraphicsDevice* device,
            IMemoryAllocator* allocator,
            IZ_PCSTR path,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            CreateTexFunc pCreateTexFunc);

    private:
        inline CCubeTextureGLES2();
        virtual inline ~CCubeTextureGLES2();

    private:
        // テクスチャ作成
        IZ_BOOL CreateTextureImpl(
            CGraphicsDevice* device,
            IZ_PCSTR path,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt);

        // テクスチャ情報取得
        void SetTextureInfo(
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            E_GRAPH_RSC_USAGE usage);

        void Initialize();

    public:
        // ロック
        virtual IZ_UINT Lock(
            E_GRAPH_CUBE_TEX_FACE face,
            IZ_UINT level,
            void** data,
            IZ_BOOL isReadOnly,
            IZ_BOOL isDiscard = IZ_FALSE);

        // アンロック
        virtual IZ_BOOL Unlock(
            E_GRAPH_CUBE_TEX_FACE face,
            IZ_UINT level);

        virtual TEX_HANDLE GetTexHandle();

    private:
        CGraphicsDeviceGLES2* m_Device;

        // 本体
        GLuint m_Texture;

        IZ_BOOL m_IsInitialized;

        IZ_UINT m_Size;

        GLenum m_GLFormat;
        GLenum m_GLType;

        IZ_UINT m_LockedSize;
        IZ_INT m_LockedLevel;
        E_GRAPH_CUBE_TEX_FACE m_LockedFace;

        void* m_TemporaryData;
    };
}   // namesace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_GLES2_H__)
