#include "DXTFormatConverter.h"
#include "squish.h"

using namespace izanagi;
using namespace tool;

namespace {
	IZ_INT _GetSquishFmt(E_GRAPH_PIXEL_FMT fmt)
	{
		IZ_INT ret = squish::kDxt1;
		switch (fmt) {
		case E_GRAPH_PIXEL_FMT_DXT1:
			ret = squish::kDxt1;
			break;
		case E_GRAPH_PIXEL_FMT_DXT3:
			ret = squish::kDxt3;
			break;
		case E_GRAPH_PIXEL_FMT_DXT5:
			ret = squish::kDxt5;
			break;
		default:
			IZ_ASSERT(IZ_FALSE);
			break;
		}

		return ret;
	}
}

void CDXTFormatConverter::Compress(
	IZ_UINT width,
	IZ_UINT height,
	const void* src,
	void* dst,
	E_GRAPH_PIXEL_FMT fmt)
{
	// NOTE
	// 外部でRGBA8に変換されている前提

	IZ_INT squishFmt = _GetSquishFmt(fmt);

	squish::CompressImage(
		static_cast<const squish::u8*>(src),
		width, height,
		dst,
		squishFmt);
}

void CDXTFormatConverter::Decompress(
	IZ_UINT width,
	IZ_UINT height,
	const void* src,
	void* dst,
	E_GRAPH_PIXEL_FMT fmt)
{
	IZ_INT squishFmt = _GetSquishFmt(fmt);

	squish::DecompressImage(
		static_cast<squish::u8*>(dst),
		width, height,
		src,
		squishFmt);
}
