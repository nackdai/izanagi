#include "PixelFormatConverter.h"
#include "ImageLibUtility.h"
#include "DXTFormatConverter.h"
#include "izMath.h"
#include "squish.h"

using namespace izanagi;
using namespace tool;

CPixelFormatConverter CPixelFormatConverter::s_cInstance;

namespace {
    // RGBA8/BGRA8
    IZ_UINT _ComputeByteSize_RGBA8(IZ_UINT nWidth, IZ_UINT nHeight)
    {
        IZ_UINT ret = nWidth * nHeight * 4;
        return ret;
    }

    // RGBA4
    IZ_UINT _ComputeByteSize_RGBA4(IZ_UINT nWidth, IZ_UINT nHeight)
    {
        IZ_UINT ret = nWidth * nHeight * 2;
        return ret;
    }

    // RGB10A2
    IZ_UINT _ComputeByteSize_RGB10A2(IZ_UINT nWidth, IZ_UINT nHeight)
    {
        IZ_UINT ret = nWidth * nHeight * 4;
        return ret;
    }

    // A8
    IZ_UINT _ComputeByteSize_A8(IZ_UINT nWidth, IZ_UINT nHeight)
    {
        IZ_UINT ret = nWidth * nHeight * 1;
        return ret;
    }

    // R32F
    IZ_UINT _ComputeByteSize_R32F(IZ_UINT nWidth, IZ_UINT nHeight)
    {
        IZ_UINT ret = nWidth * nHeight * 4;
        return ret;
    }

    // RGBA16F
    IZ_UINT _ComputeByteSize_RGBA16F(IZ_UINT nWidth, IZ_UINT nHeight)
    {
        IZ_UINT ret = nWidth * nHeight * 8;
        return ret;
    }

    // RGBA32F
    IZ_UINT _ComputeByteSize_RGBA32F(IZ_UINT nWidth, IZ_UINT nHeight)
    {
        IZ_UINT ret = nWidth * nHeight * 16;
        return ret;
    }

    // DXT1
    IZ_UINT _ComputeByteSize_DXT1(IZ_UINT nWidth, IZ_UINT nHeight)
    {
#if 0
        // NOTE
        // 4 x 4 [pixel] -> 64 [bit]
        IZ_UINT ret = (nWidth >> 2) * (nHeight >> 2) * 8;
#else
        IZ_UINT ret = squish::GetStorageRequirements(
                        nWidth, nHeight, 
                        squish::kDxt1);
#endif
        return ret;
    }

    // DXT3/DXT5
    IZ_UINT _ComputeByteSize_DXT5(IZ_UINT nWidth, IZ_UINT nHeight)
    {
#if 0
        // NOTE
        // 4 x 4 [pixel] -> 128 [bit]
        IZ_UINT ret = (nWidth >> 2) * (nHeight >> 2) * 16;
#else
        // NOTE
        // サイズは DXT3 or DXT5 どちらでも同じ
        IZ_UINT ret = squish::GetStorageRequirements(
                        nWidth, nHeight, 
                        squish::kDxt3);
#endif
        return ret;
    }

    typedef IZ_UINT (*ComputeByteSizeFunc)(IZ_UINT, IZ_UINT);

    ComputeByteSizeFunc ComputeByteSizeFuncTbl[] = {
        _ComputeByteSize_RGBA8,     // RGBA8
        _ComputeByteSize_RGBA8,     // BGRA8

        _ComputeByteSize_RGBA4,     // RGBA4
        _ComputeByteSize_RGB10A2,   // RGB10A2
        _ComputeByteSize_A8,        // A8
        
        _ComputeByteSize_R32F,      // R32F
        _ComputeByteSize_RGBA16F,   // RGBA16F
        _ComputeByteSize_RGBA32F,   // RGBA32F

        _ComputeByteSize_DXT1,      // DXT1
        _ComputeByteSize_DXT5,      // DXT3
        _ComputeByteSize_DXT5,      // DXT5

        IZ_NULL,    // RGBX8
        IZ_NULL,    // D24S8

        // Only for OpenGL...
        IZ_NULL,    // RGBA32UI,
        IZ_NULL,    // RGB32UI,
    };

    IZ_C_ASSERT(COUNTOF(ComputeByteSizeFuncTbl) == graph::E_GRAPH_PIXEL_FMT_NUM);
}   // namespace

/**
* バイトサイズ計算
*/
IZ_UINT CPixelFormatConverter::ComputeByteSize(
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    graph::E_GRAPH_PIXEL_FMT nFmt)
{
    ComputeByteSizeFunc func = ComputeByteSizeFuncTbl[nFmt];
    VRETURN_VAL(func != IZ_NULL, 0);

    IZ_UINT ret = (*func)(nWidth, nHeight);
    return ret;
}

typedef void (*ConvertPixelFormat)(void*, void*, IZ_UINT);
#define ConvertFunc(src, dst)   _Convert##src##dst

namespace {
    // RGBA8反転
    void _InverseRGBA8(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_BYTE* src = reinterpret_cast<IZ_BYTE*>(pSrc);
        IZ_BYTE* dst = reinterpret_cast<IZ_BYTE*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_BYTE r = *(src++);
            IZ_BYTE g = *(src++);
            IZ_BYTE b = *(src++);
            IZ_BYTE a = *(src++);

            *(dst++) = b;
            *(dst++) = g;
            *(dst++) = r;
            *(dst++) = a;
        }
    }
}   // namespace

namespace {
    // RGBA8 -> RGBA32F
    void ConvertFunc(RGBA8, RGBA32F)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_BYTE* src = reinterpret_cast<IZ_BYTE*>(pSrc);
        IZ_FLOAT* dst = reinterpret_cast<IZ_FLOAT*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_BYTE r = *(src++);
            IZ_BYTE g = *(src++);
            IZ_BYTE b = *(src++);
            IZ_BYTE a = *(src++);

            *(dst++) = r * DIV_RGBA8_MAX;
            *(dst++) = g * DIV_RGBA8_MAX;
            *(dst++) = b * DIV_RGBA8_MAX;
            *(dst++) = a * DIV_RGBA8_MAX;
        }
    }

    // BGRA8 -> RGBA32F
    void ConvertFunc(BGRA8, RGBA32F)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        _InverseRGBA8(pSrc, pSrc, nWidth);
        ConvertFunc(RGBA8, RGBA32F)(pSrc, pDst, nWidth);
    }

    // RGBA4 -> RGBA32F
    void ConvertFunc(RGBA4, RGBA32F)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        S_RGBA4_VAL* src = reinterpret_cast<S_RGBA4_VAL*>(pSrc);
        IZ_FLOAT* dst = reinterpret_cast<IZ_FLOAT*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_BYTE r = src->r;
            r <<= 4;

            IZ_BYTE g = src->g;
            g <<= 4;
            
            IZ_BYTE b = src->b;
            b <<= 4;
            
            IZ_BYTE a = src->a;
            a <<= 4;

            src++;

            *(dst++) = r * DIV_RGBA8_MAX;
            *(dst++) = g * DIV_RGBA8_MAX;
            *(dst++) = b * DIV_RGBA8_MAX;
            *(dst++) = a * DIV_RGBA8_MAX;
        }
    }

    // RGB10A2 -> RGBA32F
    void ConvertFunc(RGB10A2, RGBA32F)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        S_RGB10A2_VAL* src = reinterpret_cast<S_RGB10A2_VAL*>(pSrc);
        IZ_FLOAT* dst = reinterpret_cast<IZ_FLOAT*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_FLOAT r = src->r * DIV_RGB10A2_RGB_MAX;
            IZ_FLOAT g = src->g * DIV_RGB10A2_RGB_MAX;
            IZ_FLOAT b = src->b * DIV_RGB10A2_RGB_MAX;
            IZ_FLOAT a = src->a * DIV_RGB10A2_A_MAX;
            src++;

            *(dst++) = r;
            *(dst++) = g;
            *(dst++) = b;
            *(dst++) = a;
        }
    }

    // A8 -> RGBA32F
    void ConvertFunc(A8, RGBA32F)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_BYTE* src = reinterpret_cast<IZ_BYTE*>(pSrc);
        IZ_FLOAT* dst = reinterpret_cast<IZ_FLOAT*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_BYTE a = *(src++);

            IZ_FLOAT fA = a * DIV_RGBA8_MAX;

            *(src++) = static_cast<IZ_BYTE>(fA);
            *(src++) = static_cast<IZ_BYTE>(fA);
            *(src++) = static_cast<IZ_BYTE>(fA);
            *(src++) = static_cast<IZ_BYTE>(fA);
        }
    }

    // R32F -> RGBA32F
    void ConvertFunc(R32F, RGBA32F)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_FLOAT* src = reinterpret_cast<IZ_FLOAT*>(pSrc);
        IZ_FLOAT* dst = reinterpret_cast<IZ_FLOAT*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_FLOAT r = *(src++);

            *(dst++) = r;
            *(dst++) = r;
            *(dst++) = r;
            *(dst++) = r;
        }
    }

    // RGBA16F -> RGBA32F
    void ConvertFunc(RGBA16F, RGBA32F)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_UINT16* src = reinterpret_cast<IZ_UINT16*>(pSrc);
        IZ_FLOAT* dst = reinterpret_cast<IZ_FLOAT*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_UINT16 r = *(src++);
            IZ_UINT16 g = *(src++);
            IZ_UINT16 b = *(src++);
            IZ_UINT16 a = *(src++);

            *(dst++) = izanagi::math::CFloat16::Float16ToFloat32(r);
            *(dst++) = izanagi::math::CFloat16::Float16ToFloat32(g);
            *(dst++) = izanagi::math::CFloat16::Float16ToFloat32(b);
            *(dst++) = izanagi::math::CFloat16::Float16ToFloat32(a);
        }
    }
}   // namespace

namespace {
    // RGBA32F -> RGBA8
    void ConvertFunc(RGBA32F, RGBA8)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_FLOAT* src = reinterpret_cast<IZ_FLOAT*>(pSrc);
        IZ_BYTE* dst = reinterpret_cast<IZ_BYTE*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_FLOAT r = *(src++);
            IZ_FLOAT g = *(src++);
            IZ_FLOAT b = *(src++);
            IZ_FLOAT a = *(src++);

            *(dst++) = static_cast<IZ_BYTE>(IZ_MIN(r * RGBA8_MAX, RGBA8_MAX));
            *(dst++) = static_cast<IZ_BYTE>(IZ_MIN(g * RGBA8_MAX, RGBA8_MAX));
            *(dst++) = static_cast<IZ_BYTE>(IZ_MIN(b * RGBA8_MAX, RGBA8_MAX));
            *(dst++) = static_cast<IZ_BYTE>(IZ_MIN(a * RGBA8_MAX, RGBA8_MAX));
        }
    }

    // RGBA32F -> BGRA8
    void ConvertFunc(RGBA32F, BGRA8)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        ConvertFunc(RGBA32F, RGBA8)(pSrc, pDst, nWidth);
        _InverseRGBA8(pDst, pDst, nWidth);
    }

    // RGBA32F -> RGBA4
    void ConvertFunc(RGBA32F, RGBA4)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_FLOAT* src = reinterpret_cast<IZ_FLOAT*>(pSrc);
        S_RGBA4_VAL* dst = reinterpret_cast<S_RGBA4_VAL*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_FLOAT r = *(src++);
            IZ_FLOAT g = *(src++);
            IZ_FLOAT b = *(src++);
            IZ_FLOAT a = *(src++);

            dst->r = (IZ_BYTE)(IZ_MIN(r * RGBA8_MAX, RGBA8_MAX)) >> 4;
            dst->g = (IZ_BYTE)(IZ_MIN(g * RGBA8_MAX, RGBA8_MAX)) >> 4;
            dst->b = (IZ_BYTE)(IZ_MIN(b * RGBA8_MAX, RGBA8_MAX)) >> 4;
            dst->a = (IZ_BYTE)(IZ_MIN(a * RGBA8_MAX, RGBA8_MAX)) >> 4;
            dst++;
        }
    }

    // RGBA32F -> RGB10A2
    void ConvertFunc(RGBA32F, RGB10A2)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_FLOAT* src = reinterpret_cast<IZ_FLOAT*>(pSrc);
        S_RGB10A2_VAL* dst = reinterpret_cast<S_RGB10A2_VAL*>(pDst);
        
        for (IZ_UINT x = 0; x  < nWidth; x++) {
            IZ_FLOAT r = *(src++);
            r = math::CMath::Clamp(r, 0.0f, 1.0f);
            
            IZ_FLOAT g = *(src++);
            g = math::CMath::Clamp(g, 0.0f, 1.0f);
            
            IZ_FLOAT b = *(src++);
            b = math::CMath::Clamp(b, 0.0f, 1.0f);
            
            IZ_FLOAT a = *(src++);
            a = math::CMath::Clamp(a, 0.0f, 1.0f);

            dst->r = static_cast<IZ_UINT32>(IZ_MIN(r * RGB10A2_RGB_MAX, RGB10A2_RGB_MAX));
            dst->g = static_cast<IZ_UINT32>(IZ_MIN(g * RGB10A2_RGB_MAX, RGB10A2_RGB_MAX));
            dst->b = static_cast<IZ_UINT32>(IZ_MIN(b * RGB10A2_RGB_MAX, RGB10A2_RGB_MAX));
            dst->a = static_cast<IZ_UINT32>(IZ_MIN(a * RGB10A2_A_MAX, RGB10A2_A_MAX));
            dst++;
        }
    }

    // RGBA32F -> A8
    void ConvertFunc(RGBA32F, A8)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_FLOAT* src = reinterpret_cast<IZ_FLOAT*>(pSrc);
        IZ_BYTE* dst = reinterpret_cast<IZ_BYTE*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_FLOAT r = *(src++);
            IZ_FLOAT g = *(src++);
            IZ_FLOAT b = *(src++);
            IZ_FLOAT a = *(src++);

            *(dst++) = static_cast<IZ_BYTE>(IZ_MIN(a * RGBA8_MAX, RGBA8_MAX));
        }
    }

    // RGBA32F -> R32F
    void ConvertFunc(RGBA32F, R32F)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_FLOAT* src = reinterpret_cast<IZ_FLOAT*>(pSrc);
        IZ_FLOAT* dst = reinterpret_cast<IZ_FLOAT*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_FLOAT r = *(src++);
            IZ_FLOAT g = *(src++);
            IZ_FLOAT b = *(src++);
            IZ_FLOAT a = *(src++);

            *(dst++) = r;
        }
    }

    // RGBA32F -> RGBA16F
    void ConvertFunc(RGBA32F, RGBA16F)(void* pSrc, void* pDst, IZ_UINT nWidth)
    {
        IZ_FLOAT* src = reinterpret_cast<IZ_FLOAT*>(pSrc);
        IZ_UINT16* dst = reinterpret_cast<IZ_UINT16*>(pDst);

        for (IZ_UINT x = 0; x < nWidth; x++) {
            IZ_FLOAT r = *(src++);
            IZ_FLOAT g = *(src++);
            IZ_FLOAT b = *(src++);
            IZ_FLOAT a = *(src++);

            *(dst++) = izanagi::math::CFloat16::Float32ToFloat16(r);
            *(dst++) = izanagi::math::CFloat16::Float32ToFloat16(g);
            *(dst++) = izanagi::math::CFloat16::Float32ToFloat16(b);
            *(dst++) = izanagi::math::CFloat16::Float32ToFloat16(a);
        }
    }
}   // namespace

namespace {
    // 中間フォーマット変換テーブル
    ConvertPixelFormat InterFmtConvTbl[] = {
        ConvertFunc(RGBA8, RGBA32F),    // RGBA8
        ConvertFunc(BGRA8, RGBA32F),    // BGRA8
        ConvertFunc(RGBA4, RGBA32F),    // RGBA4
        ConvertFunc(RGB10A2, RGBA32F),  // RGB10A2
        ConvertFunc(A8, RGBA32F),       // A8
        ConvertFunc(R32F, RGBA32F),     // R32F
        ConvertFunc(RGBA16F, RGBA32F),  // RGBA16F
        IZ_NULL,                        // RGBA32F
        IZ_NULL,                        // DXT1
        IZ_NULL,                        // DXT3
        IZ_NULL,                        // DXT5
        IZ_NULL,                        // RGBX8
        IZ_NULL,                        // D24S8

        // Only for OpenGL...
        IZ_NULL,                        // RGBA32UI
        IZ_NULL,                        // RGB32UI
    };
    IZ_C_ASSERT(COUNTOF(InterFmtConvTbl) == graph::E_GRAPH_PIXEL_FMT_NUM);

    // 最終フォーマット変換テーブル
    ConvertPixelFormat DstFmtConvTbl[] = {
        ConvertFunc(RGBA32F, RGBA8),    // RGBA8
        ConvertFunc(RGBA32F, BGRA8),    // BGRA8
        ConvertFunc(RGBA32F, RGBA4),    // RGBA4
        ConvertFunc(RGBA32F, RGB10A2),  // RGB10A2
        ConvertFunc(RGBA32F, A8),       // A8
        ConvertFunc(RGBA32F, R32F),     // R32F
        ConvertFunc(RGBA32F, RGBA16F),  // RGBA16F
        IZ_NULL,                        // RGBA32F
        IZ_NULL,                        // DXT1
        IZ_NULL,                        // DXT3
        IZ_NULL,                        // DXT5
        IZ_NULL,                        // RGBX8
        IZ_NULL,                        // D24S8

        // Only for OpenGL...
        IZ_NULL,                        // RGBA32UI
        IZ_NULL,                        // RGB32UI
    };
    IZ_C_ASSERT(COUNTOF(DstFmtConvTbl) == graph::E_GRAPH_PIXEL_FMT_NUM);
}   // namespace

/**
* コンバート
*/
IZ_BOOL CPixelFormatConverter::Convert(
    IZ_BYTE* pSrc,
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    graph::E_GRAPH_PIXEL_FMT nSrcFmt,
    IZ_BYTE* pDst,
    graph::E_GRAPH_PIXEL_FMT nDstFmt)
{
    if (nSrcFmt == nDstFmt) {
        // ある？
        size_t nSize = ComputeByteSize(nWidth, nHeight, nSrcFmt);
        memcpy(pDst, pSrc, nSize);
        return IZ_TRUE;
    }

    if (CImageUtil::IsDXT(nDstFmt)) {
        // まず RGBA8 に変換する
        std::vector<IZ_UINT8> tmp(ComputeByteSize(nWidth, nHeight, graph::E_GRAPH_PIXEL_FMT_RGBA8));

        VRETURN(
            Convert(
                pSrc,
                nWidth, nHeight,
                nSrcFmt,
                &tmp[0],
                graph::E_GRAPH_PIXEL_FMT_RGBA8));

        // DXTに変換する
        CDXTFormatConverter::Compress(
            nWidth, nHeight,
            pSrc, pDst,
            nDstFmt);
    }
    else if (CImageUtil::IsDXT(nSrcFmt)) {
        // DXT -> RGBA8に変換される
        std::vector<IZ_UINT8> tmp(ComputeByteSize(nWidth, nHeight, graph::E_GRAPH_PIXEL_FMT_RGBA8));

        CDXTFormatConverter::Decompress(
            nWidth, nHeight,
            pSrc, &tmp[0],
            nSrcFmt);

        // 最終フォーマットに変換する
        VRETURN(
            Convert(
                &tmp[0],
                nWidth, nHeight,
                graph::E_GRAPH_PIXEL_FMT_RGBA8,
                pDst,
                nDstFmt));
    }
    else {
        // RGBA32F変換用１ラインバッファ
        IZ_UINT nBufSize = nWidth * CImageUtil::GetBpp(graph::E_GRAPH_PIXEL_FMT_RGBA32F);
        if (m_LineBuffer.size() < nBufSize) {
            m_LineBuffer.resize(nBufSize);
        }

        // ピッチ
        IZ_UINT nSrcPitch = nWidth * CImageUtil::GetBpp(nSrcFmt);
        IZ_UINT nDstPitch = nWidth * CImageUtil::GetBpp(nDstFmt);

        // 変換関数
        ConvertPixelFormat pIterFunc = InterFmtConvTbl[nSrcFmt];
        ConvertPixelFormat pDstFunc = DstFmtConvTbl[nDstFmt];

        for (IZ_UINT y = 0; y < nHeight; y++) {
            IZ_BYTE* src = pSrc + nSrcPitch * y;
            IZ_BYTE* dst = pDst + nDstPitch * y;

            if (pIterFunc != IZ_NULL)
            {
                // RGBA32F に変換
                (*pIterFunc)(src, &m_LineBuffer[0], nWidth);
            }
            else
            {
                memcpy(&m_LineBuffer[0], src, nSrcPitch);
            }

            if (pDstFunc != IZ_NULL)
            {
                // 最終フォーマットに変換
                (*pDstFunc)(&m_LineBuffer[0], dst, nWidth);
            }
            else
            {
                memcpy(dst, &m_LineBuffer[0], nDstPitch);
            }
        }
    }

    return IZ_TRUE;
}
