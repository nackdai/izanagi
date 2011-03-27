#if !defined(__IMAGE_READER_IMPL_H__)
#define __IMAGE_READER_IMPL_H__

#include "izGraph.h"

namespace izanagi {
namespace izanagi_tk {
	class CIMGTexture;
}	// namespace izanagi_tk
}	// namespace izanagi

class CImageReaderImpl {
private:
	CImageReaderImpl();
	~CImageReaderImpl();

public:
	// 読み込み
	static izanagi::izanagi_tk::CIMGTexture* Read(
		LPCSTR lpszPath,
		izanagi::E_GRAPH_TEX_TYPE nType);
};

#endif	// #if !defined(__IMAGE_READER_IMPL_H__)
