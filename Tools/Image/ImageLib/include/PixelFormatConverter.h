#if !defined(__IZANAGI_IMAGE_LIB_PIXEL_FORMAT_CONVERTER_H__)
#define __IZANAGI_IMAGE_LIB_PIXEL_FORMAT_CONVERTER_H__

#include <vector>

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"

namespace izanagi {
namespace izanagi_tk {
	/**
	*/
	class CPixelFormatConverter {
	private:
		static CPixelFormatConverter s_cInstance;

	public:
		static CPixelFormatConverter* GetInstance() { return &s_cInstance; }

	private:
		CPixelFormatConverter() {}
		~CPixelFormatConverter() {}

		NO_COPIABLE(CPixelFormatConverter);

	public:
		// バイトサイズ計算
		IZ_UINT ComputeByteSize(
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			E_GRAPH_PIXEL_FMT nFmt);

		// コンバート
		IZ_BOOL Convert(
			IZ_BYTE* pSrc,
			IZ_UINT nWidth,
			IZ_UINT nHeight,
			E_GRAPH_PIXEL_FMT nSrcFmt,
			IZ_BYTE* pDst,
			E_GRAPH_PIXEL_FMT nDstFmt);

	private:
		// ラインバッファ
		// 一度確保した最大サイズを保持し続ける
		std::vector<IZ_BYTE> m_LineBuffer;
	};
}	// namespace izanagi_tk
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IMAGE_LIB_PIXEL_FORMAT_CONVERTER_H__)
