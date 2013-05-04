#ifdef __IZ_DX9__
#if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_DX9_H__)
#define __IZANAGI_GRAPH_CUBE_TEXTURE_DX9_H__

#include "izD3DDefs.h"
#include "graph/CubeTexture.h"

namespace izanagi
{
namespace graph
{
    class CGraphicsDeviceDX9;

    // キューブテクスチャ
    class CCubeTextureDX9 : public CCubeTexture
    {
        friend class CGraphicsDeviceDX9;

    private:
        // ファイルからテクスチャ作成
        static CCubeTexture* CreateCubeTextureFromFile(
            CGraphicsDeviceDX9* device,
            IMemoryAllocator* allocator,
            IZ_PCSTR path,
            E_GRAPH_PIXEL_FMT fmt);

        // テクスチャ作成
        static CCubeTexture* CreateCubeTexture(
            CGraphicsDeviceDX9* device,
            IMemoryAllocator* allocator,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt);

    private:
        typedef IZ_BOOL (CCubeTextureDX9::*CreateTexFunc)(
            CGraphicsDeviceDX9*,
            IZ_PCSTR, IZ_UINT, IZ_UINT, IZ_UINT, E_GRAPH_PIXEL_FMT);

        static CCubeTexture* CreateInternal(
            CGraphicsDeviceDX9* device,
            IMemoryAllocator* allocator,
            IZ_PCSTR path,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt,
            CreateTexFunc pCreateTexFunc);

    private:
        inline CCubeTextureDX9();
        virtual inline ~CCubeTextureDX9();

    private:
        // ファイルからテクスチャ作成
        IZ_BOOL CreateTextureFromFileImpl(
            CGraphicsDeviceDX9* device,
            IZ_PCSTR path,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt);

        // テクスチャ作成
        IZ_BOOL CreateTextureImpl(
            CGraphicsDeviceDX9* device,
            IZ_PCSTR path,
            IZ_UINT width,
            IZ_UINT height,
            IZ_UINT mipLevel,
            E_GRAPH_PIXEL_FMT fmt);

        // テクスチャ情報取得
        void GetTextureInfo();

    public:
        // ロック
        virtual IZ_UINT Lock(
            E_GRAPH_CUBE_TEX_FACE face,
            UINT level,
            void** data,
            IZ_BOOL isReadOnly,
            IZ_BOOL isDiscard = IZ_FALSE);

        // アンロック
        virtual IZ_BOOL Unlock(
            E_GRAPH_CUBE_TEX_FACE face,
            IZ_UINT level);

        virtual TEX_HANDLE GetTexHandle();

    public:
        D3D_CUBETEXTURE* GetRawInterface() { return m_Texture; }

    private:
        // 本体
        D3D_CUBETEXTURE* m_Texture;
    };
}   // namesace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_CUBE_TEXTURE_DX9_H__)
#endif  // #ifdef __IZ_DX9__
