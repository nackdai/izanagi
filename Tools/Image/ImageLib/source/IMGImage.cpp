#include "IMGImage.h"
#include "ImageLibUtility.h"
#include "PixelFormatConverter.h"
#include "izToolKit.h"

using namespace izanagi;
using namespace tool;

// コンストラクタ
CIMGImage::CIMGImage()
{
    Init(0, 0, graph::E_GRAPH_PIXEL_FMT_RGBA8);
}

// コンストラクタ
CIMGImage::CIMGImage(
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    graph::E_GRAPH_PIXEL_FMT nFmt)
{
    Init(nWidth, nHeight, nFmt);
}

// デストラクタ
CIMGImage::~CIMGImage()
{
    m_DataBuffer.clear();
}

/**
* 初期化
*/
IZ_BOOL CIMGImage::Init(
    IZ_UINT nWidth,
    IZ_UINT nHeight,
    graph::E_GRAPH_PIXEL_FMT nFmt,
    IZ_BOOL bIsAllocBuffer/*= IZ_TRUE*/)
{
    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_Fmt = nFmt;

    if ((m_nWidth == 0) || (m_nHeight == 0)) {
        return IZ_TRUE;
    }

    if (bIsAllocBuffer) {
        IZ_UINT nSize = CPixelFormatConverter::GetInstance()->ComputeByteSize(
                            m_nWidth,
                            m_nHeight,
                            m_Fmt);

        m_DataBuffer.clear();
        m_DataBuffer.resize(nSize, 0);
    }

    return IZ_TRUE;
}

/**
* フォーマット変換
*/
IZ_BOOL CIMGImage::ConvertPixelFormat(
    izanagi::E_PLATFORM type,
    graph::E_GRAPH_PIXEL_FMT nFmt)
{
    // 出力サイズ
    IZ_UINT nDstSize = CPixelFormatConverter::GetInstance()->ComputeByteSize(
                        m_nWidth,
                        m_nHeight,
                        nFmt);

    // 出力バッファ
    std::vector<IZ_BYTE> tDstBuffer(nDstSize);

    // TODO
    // 変換フォーマット
    graph::E_GRAPH_PIXEL_FMT convFmt = nFmt;

    if (type == izanagi::E_PLATFORM_DX9) {
        if (convFmt == graph::E_GRAPH_PIXEL_FMT_RGBA8) {
            convFmt = graph::E_GRAPH_PIXEL_FMT_BGRA8;
        }
    }

    // 変換
    IZ_BOOL ret = CPixelFormatConverter::GetInstance()->Convert(
                    &m_DataBuffer[0],
                    m_nWidth,
                    m_nHeight,
                    m_Fmt,
                    &tDstBuffer[0],
                    convFmt);

    IZ_ASSERT(ret);

    if (ret) {
        m_DataBuffer.resize(nDstSize);
        memcpy(
            &m_DataBuffer[0],
            &tDstBuffer[0],
            nDstSize);

        m_Fmt = nFmt;
    }

    return ret;
}

// RGBA8としてピクセルデータを取得.
IZ_BYTE* CIMGImage::GetPixelDataAsBGRA8()
{
    ClearTemporaryBuffer();

    if (m_Fmt == graph::E_GRAPH_PIXEL_FMT_BGRA8)
    {
        m_TmpBuffer.resize(m_DataBuffer.size());
        memcpy(&m_TmpBuffer[0], &m_DataBuffer[0], m_TmpBuffer.size());
    }
    else
    {
        m_TmpBuffer.resize(m_DataBuffer.size());

        IZ_BOOL result = CPixelFormatConverter::GetInstance()->Convert(
            &m_DataBuffer[0],
            m_nWidth,
            m_nHeight,
            m_Fmt,
            &m_TmpBuffer[0],
            graph::E_GRAPH_PIXEL_FMT_BGRA8);

        VRETURN_NULL(result);
    }

    return &m_TmpBuffer[0];
}

// 内部のテンポラリバッファをクリアする.
void CIMGImage::ClearTemporaryBuffer()
{
    m_TmpBuffer.clear();
}

// 読み込み
IZ_BOOL CIMGImage::Read(IInputStream* pIn)
{
    IZ_ASSERT(pIn != IZ_NULL);

    // 読み込みサイズ
    IZ_UINT nSize = GetSize();

    // 読み込み
    IZ_BOOL ret = IZ_INPUT_READ(pIn, &m_DataBuffer[0], 0, nSize);
    IZ_ASSERT(ret);

    return ret;
}

// 出力
IZ_BOOL CIMGImage::Write(IOutputStream* pOut)
{
    IZ_ASSERT(pOut != IZ_NULL);

    // データ
    IZ_BOOL ret = IZ_OUTPUT_WRITE(
                    pOut, 
                    &m_DataBuffer[0],
                    0,
                    m_DataBuffer.size());

    return ret;
}
