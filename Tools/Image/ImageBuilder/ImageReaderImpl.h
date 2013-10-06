#if !defined(__IMAGE_READER_IMPL_H__)
#define __IMAGE_READER_IMPL_H__

#include "izGraph.h"

namespace izanagi {
namespace tool {
    class CIMGTexture;
}   // namespace tool
}   // namespace izanagi

class CImageReaderImpl {
private:
    CImageReaderImpl();
    ~CImageReaderImpl();

public:
    // 読み込み
    static izanagi::tool::CIMGTexture* Read(
        LPCSTR lpszPath,
        izanagi::graph::E_GRAPH_TEX_TYPE nType);
};

#endif  // #if !defined(__IMAGE_READER_IMPL_H__)
