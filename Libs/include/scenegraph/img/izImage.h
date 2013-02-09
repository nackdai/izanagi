#if !defined(__IZANAGI_SCENEGRAPH_IZ_IMAGE_H__)
#define __IZANAGI_SCENEGRAPH_IZ_IMAGE_H__

#include "izStd.h"
#include "izGraph.h"
#include "IMGFormat.h"

namespace izanagi {
    class IInputStream;

    /**
    */
    class CImage : public CObject {
    public:
        // インスタンス作成
        static CImage* CreateImage(
            IMemoryAllocator* pAllocator,
            graph::CGraphicsDevice* pDevice,
            IInputStream* pInputStream);

    private:
        // データサイズを計算
        static IZ_UINT ComputeSize(
            IZ_UINT pitch,
            IZ_UINT height,
            graph::E_GRAPH_PIXEL_FMT fmt);

        // データ読み込み
        static void ReadData(
            IZ_UINT8* dst,
            IZ_UINT nPitch,
            IZ_UINT nHeight,
            graph::E_GRAPH_PIXEL_FMT nFmt,
            IInputStream* input);

        // テクスチャ作成
        static graph::CBaseTexture* CreatePlaneTexture(
            graph::CGraphicsDevice* pDevice,
            IZ_UINT nWidth,
            IZ_UINT nHeight,
            IZ_UINT nMipLevel,
            graph::E_GRAPH_PIXEL_FMT nFmt,
            IInputStream* input);

        // キューブテクスチャ作成
        static graph::CBaseTexture* CreateCubeTexture(
            graph::CGraphicsDevice* pDevice,
            IZ_UINT nWidth,
            IZ_UINT nHeight,
            IZ_UINT nMipLevel,
            graph::E_GRAPH_PIXEL_FMT nFmt,
            IInputStream* input);

        // テクスチャ作成
        static inline graph::CBaseTexture* CreateTexture(
            graph::CGraphicsDevice* pDevice,
            const S_IMG_TEX_HEADER& sTexHeader,
            IInputStream* input);

    protected:
        CImage()
        {
            m_Allocator = IZ_NULL;
            m_pTexture = IZ_NULL;
        }
        ~CImage()
        {
            for (IZ_UINT i = 0; i < m_Header.numTextures; ++i) {
                SAFE_RELEASE(m_pTexture[i]);
            }
        }

        NO_COPIABLE(CImage);

         IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        // テクスチャ読み込み
        IZ_BOOL ReadTexture(
            graph::CGraphicsDevice* pDevice,
            IInputStream* pInputStream);

    public:
        // ヘッダ取得
        const S_IMG_HEADER& GetHeader() const
        {
            return m_Header;
        }

        // テクスチャ数取得
        IZ_UINT GetTexNum() const
        {
            return m_Header.numTextures;
        }

        // テクスチャ取得
        graph::CBaseTexture* GetTexture(IZ_UINT nIdx)
        {
            IZ_ASSERT(nIdx < m_Header.numTextures);
            IZ_ASSERT(m_pTexture != IZ_NULL);
            return m_pTexture[nIdx];
        }

    protected:
        IMemoryAllocator* m_Allocator;

        S_IMG_HEADER m_Header;
        graph::CBaseTexture** m_pTexture;
    };  
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_IZ_IMAGE_H__)
