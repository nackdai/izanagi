#if !defined(__IZANAGI_GRAPH_GRAPH_UTIL_H__)
#define __IZANAGI_GRAPH_GRAPH_UTIL_H__

#include "izStd.h"
#include "GraphDefs.h"

namespace izanagi {
	/**
	*/
	class CGraphUtil {
	private:
		CGraphUtil();
		~CGraphUtil();

		NO_COPIABLE(CGraphUtil);

	public:
		// floatフォーマットかどうか
		inline IZ_BOOL IsFloatPixelFormat(E_GRAPH_PIXEL_FMT fmt)
		{
			IZ_BOOL ret = ((fmt == E_GRAPH_PIXEL_FMT_R32F)
							|| (fmt == E_GRAPH_PIXEL_FMT_RGBA32F)
							|| (fmt == E_GRAPH_PIXEL_FMT_RGBA16F));
			return ret;
		}

		// RGBA8フォーマットかどうか
		inline IZ_BOOL IsRGBA8PixelFormat(E_GRAPH_PIXEL_FMT fmt)
		{
			IZ_BOOL ret = ((fmt == E_GRAPH_PIXEL_FMT_RGBA8)
							|| (fmt == E_GRAPH_PIXEL_FMT_BGRA8));
			return ret;
		}

		// DXTフォーマットかどうか
		inline IZ_BOOL IsCompressedPixelFormat(E_GRAPH_PIXEL_FMT fmt)
		{
			IZ_BOOL ret = ((fmt == E_GRAPH_PIXEL_FMT_DXT1)
							|| (fmt == E_GRAPH_PIXEL_FMT_DXT3)
							|| (fmt == E_GRAPH_PIXEL_FMT_DXT5));
			return ret;
		}
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_GRAPH_GRAPH_UTIL_H__)
