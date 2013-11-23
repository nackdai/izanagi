#include "posteffect/util/PostEffectUtilityRect.h"

using namespace izanagi;

/**
* テクスチャ座標を計算する
*/
void CPostEffectRectUtil::GetTextureCoord(
    SFloatRect* pTexCoord,
    const graph::CTexture* pSrcTex,
    const SIntRect* pSrcRect,
    const graph::CTexture* pDstTex,
    const SIntRect* pDstRect)
{
    IZ_ASSERT(pTexCoord != IZ_NULL);

    // NOTE
    // 矩形描画時に0.5ピクセルずらしているので、
    // テクスチャ座標をずらす必要はない

    pTexCoord->left = 0.0f;
    pTexCoord->top = 0.0f;
    pTexCoord->right = 1.0f;
    pTexCoord->bottom = 1.0f;

    if ((pSrcRect != IZ_NULL) && (pSrcTex != IZ_NULL)) {
        // 入力に基づいて補正
        IZ_INT nWidth = pSrcTex->GetWidth();
        IZ_INT nHeight = pSrcTex->GetHeight();

        IZ_FLOAT fU = 1.0f / nWidth;
        IZ_FLOAT fV = 1.0f / nHeight;

        pTexCoord->left += pSrcRect->left * fU;
        pTexCoord->top += pSrcRect->top * fV;
        pTexCoord->right -= (nWidth - CIntRect::right(*pSrcRect)) * fU;
        pTexCoord->bottom -= (nHeight - CIntRect::bottom(*pSrcRect)) * fV;
    }

    if ((pDstRect != IZ_NULL) && (pDstTex != IZ_NULL)) {
        // 出力に基づいて補正
        IZ_INT nWidth = pDstTex->GetWidth();
        IZ_INT nHeight = pDstTex->GetHeight();

        IZ_FLOAT fU = 1.0f / nWidth;
        IZ_FLOAT fV = 1.0f / nHeight;

        pTexCoord->left -= pDstRect->left * fU;
        pTexCoord->top -= pDstRect->top * fV;
        pTexCoord->right += (nWidth - CIntRect::right(*pDstRect)) * fU;
        pTexCoord->bottom += (nHeight - CIntRect::bottom(*pDstRect)) * fV;
    }
}
