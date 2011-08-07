#include "IMGImage.h"
#include "ImageLibUtility.h"
#include "PixelFormatConverter.h"
#include "ToolKitIoDefs.h"

using namespace izanagi;
using namespace izanagi_tk;

// コンストラクタ
CIMGImage::CIMGImage()
{
	Init(0, 0, E_GRAPH_PIXEL_FMT_RGBA8);
}

// コンストラクタ
CIMGImage::CIMGImage(
	IZ_UINT nWidth,
	IZ_UINT nHeight,
	E_GRAPH_PIXEL_FMT nFmt)
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
	E_GRAPH_PIXEL_FMT nFmt,
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
IZ_BOOL CIMGImage::ConvertPixelFormat(E_GRAPH_PIXEL_FMT nFmt)
{
	// 出力サイズ
	IZ_UINT nDstSize = CPixelFormatConverter::GetInstance()->ComputeByteSize(
						m_nWidth,
						m_nHeight,
						nFmt);

	// 出力バッファ
	std::vector<IZ_BYTE> tDstBuffer(nDstSize);

	// 変換
	IZ_BOOL ret = CPixelFormatConverter::GetInstance()->Convert(
					&m_DataBuffer[0],
					m_nWidth,
					m_nHeight,
					m_Fmt,
					&tDstBuffer[0],
					nFmt);

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
