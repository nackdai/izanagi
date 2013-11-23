#if !defined(__IZANAGI_POSTEFFECT_RECT_UTILITY_H__)
#define __IZANAGI_POSTEFFECT_RECT_UTILITY_H__

#include "izStd.h"
#include "izGraph.h"

namespace izanagi {
    /**
    * ポストエフェクトの矩形処理に関するユーティリティ
    */
    class CPostEffectRectUtil{
    private:
        CPostEffectRectUtil();
        ~CPostEffectRectUtil();

        CPostEffectRectUtil(const CPostEffectRectUtil& rhs);
        const CPostEffectRectUtil& operator=(const CPostEffectRectUtil& rhs);

    public:
        // テクスチャ座標を計算する
        static void GetTextureCoord(
            SFloatRect* pTexCoord,
            const graph::CTexture* pSrcTex,
            const SIntRect* pSrcRect,
            const graph::CTexture* pDstTex,
            const SIntRect* pDstRect);

    public:
        // テクスチャ矩形を取得
        static inline void GetTextureRect(
            const graph::CTexture* pTex,
            SIntRect* pRect);
        static inline void GetTextureRect(
            graph::CTexture* pTex,
            graph::CGraphicsDevice* pDevice,
            SIntRect* pRect);

        // 矩形を膨らませる
        static inline void InflateRect(
            SIntRect* pRect,
            IZ_INT x, IZ_INT y);
        static inline void InflateRect(
            SIntRect* pDst,
            const SIntRect& rcSrc,
            IZ_INT x, IZ_INT y);

        // 矩形を比較する
        static inline IZ_BOOL CmpRect(
            const SIntRect& rc0,
            const SIntRect& rc1);
    };

    /**
    * テクスチャ矩形を取得
    */
    void CPostEffectRectUtil::GetTextureRect(
        const graph::CTexture* pTex,
        SIntRect* pRect)
    {
        IZ_ASSERT(pTex != IZ_NULL);
        IZ_ASSERT(pRect != IZ_NULL);

        pRect->left = 0;
        pRect->top = 0;
        pRect->width = pTex->GetWidth();
        pRect->height = pTex->GetHeight();
    }

    /**
    * テクスチャ矩形を取得
    */
    void CPostEffectRectUtil::GetTextureRect(
        graph::CTexture* pTex,
        graph::CGraphicsDevice* pDevice,
        SIntRect* pRect)
    {
        IZ_ASSERT(pRect != IZ_NULL);

        pRect->left = 0;
        pRect->top = 0;
        pRect->width = pTex->GetWidth();
        pRect->height = pTex->GetHeight();
    }

    /**
    * 矩形を膨らませる
    */
    void CPostEffectRectUtil::InflateRect(
        SIntRect* pRect,
        IZ_INT x, IZ_INT y)
    {
        CIntRect::InflateRect(
            *pRect,
            x, y, x, y);
    }

    /**
    * 矩形を膨らませる
    */
    void CPostEffectRectUtil::InflateRect(
        SIntRect* pDst,
        const SIntRect& rcSrc,
        IZ_INT x, IZ_INT y)
    {
        CIntRect::InflateRect(
            *pDst,
            rcSrc,
            x, y, x, y);
    }

    /**
    * 矩形を比較する
    */
    IZ_BOOL CPostEffectRectUtil::CmpRect(
        const SIntRect& rc0,
        const SIntRect& rc1)
    {
        return ((rc0.left == rc1.left)
                && (rc0.top == rc1.top)
                && (rc0.width == rc1.width)
                && (rc0.height == rc1.height));
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_POSTEFFECT_RECT_UTILITY_H__)
