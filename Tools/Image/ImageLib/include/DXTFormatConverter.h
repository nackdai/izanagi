#if !defined(__IZANAGI_IMAGE_LIB_DXT_FORMAT_CONVERTER_H__)
#define __IZANAGI_IMAGE_LIB_DXT_FORMAT_CONVERTER_H__

#include "izDefs.h"
#include "izStd.h"
#include "izGraph.h"

namespace izanagi {
namespace tool {
	class CDXTFormatConverter {
	private:
		CDXTFormatConverter();
		~CDXTFormatConverter();

		NO_COPIABLE(CDXTFormatConverter);

	public:
		static void Compress(
			IZ_UINT width,
			IZ_UINT height,
			const void* src,
			void* dst,
			E_GRAPH_PIXEL_FMT fmt);

		static void Decompress(
			IZ_UINT width,
			IZ_UINT height,
			const void* src,
			void* dst,
			E_GRAPH_PIXEL_FMT fmt);
	};
}	// namespace tool
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IMAGE_LIB_DXT_FORMAT_CONVERTER_H__)
