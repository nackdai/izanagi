#if !defined(__IZANAGI_TOOL_KIT_GRAPH_TEXTURE_LITE_H__)
#define __IZANAGI_TOOL_KIT_GRAPH_TEXTURE_LITE_H__

#include <d3d9.h>
#include <d3dx9.h>

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"
#include "graph/ToolkitGraphDefs.h"

namespace izanagi {
namespace tool {
    class CGraphicsDeviceLite;

    /**
    */
    class CTextureLite : public CObject {
        friend class CGraphicsDeviceLite;

    private:
        // ファイルからテクスチャ作成
        static CTextureLite* CreateTextureFromFile(
            CGraphicsDeviceLite* pDevice,
            IMemoryAllocator* pAllocator,
            IZ_PCSTR lpszPath,
            IZ_BOOL needNonPow2 = IZ_FALSE);

        // テクスチャ作成
        static CTextureLite* CreateTexture(
            CGraphicsDeviceLite* pDevice,
            IMemoryAllocator* pAllocator,
            IZ_UINT nWidth,
            IZ_UINT nHeight,
            D3DFORMAT fmt);

        // テクスチャ作成
        static CTextureLite* CreateTexture(
            CGraphicsDeviceLite* pDevice,
            IMemoryAllocator* pAllocator,
            IZ_UINT nWidth,
            IZ_UINT nHeight,
            izanagi::graph::E_GRAPH_PIXEL_FMT fmt);

        // テクスチャ保存
        static IZ_BOOL SaveTexture(
            IZ_PCSTR path,
            CTextureLite* texture,
            TextureExportType type);

    private:
        CTextureLite();
        ~CTextureLite();

        NO_COPIABLE(CTextureLite);

        IZ_DEFINE_INTERNAL_RELEASE();
        
    public:
        // ロック
        IZ_UINT Lock(
            UINT nLevel,
            void** data);

        // アンロック
        IZ_BOOL Unlock(IZ_UINT nLevel);

    public:
        // 幅取得
        IZ_UINT GetWidth(IZ_UINT level = 0) const;

        // 高さ取得
        IZ_UINT GetHeight(IZ_UINT level = 0) const;

        // フォーマット取得
        graph::E_GRAPH_PIXEL_FMT GetPixelFormat() const;

        // MIPMAP数取得
        IZ_UINT GetMipMapNum() const;

        // オリジナルフォーマット取得
        D3DFORMAT GetOriginalFormat() const
        {
            return m_Desc.Format;
        }

    public:
        LPDIRECT3DTEXTURE9 GetRawInterface() { return m_pTexture; }

    private:
        IMemoryAllocator* m_Allocator;

        // 本体
        LPDIRECT3DTEXTURE9 m_pTexture;

        D3DSURFACE_DESC m_Desc;
        IZ_UINT m_nMipLevels;
        graph::E_GRAPH_PIXEL_FMT m_nFmt;
    };
}   // namespace tool
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TOOL_KIT_GRAPH_TEXTURE_LITE_H__)
