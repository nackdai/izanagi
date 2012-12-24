#if !defined(__IZANAGI_SCENEGRAPH_MDL_FORMAT_H__)
#define __IZANAGI_SCENEGRAPH_MDL_FORMAT_H__

#include "izDefs.h"
#include "izStd.h"
#include "izMath.h"
#include "izGraph.h"

// MDL = MoDeL

namespace izanagi {
    enum {
        // TODO
        MDL_CHUNK_MAGIC_MESH      = 0x01234567,
        MDL_CHUNK_MAGIC_SKELETON  = 0x12345678,
        MDL_CHUNK_MAGIC_TERMINATE = 0x7fffffff,
    };

    // NOTE
    // フォーマット
    // +--------------------+
    // |   ファイルヘッダ   |
    // +--------------------+
    // |  メッシュチャンク  |
    // +--------------------+
    // | スケルトンチャンク |
    // +--------------------+
    
    struct S_MDL_HEADER {
        IZ_UINT magic;
        IZ_UINT version;

        IZ_UINT sizeHeader;
        IZ_UINT sizeFile;
    };

    struct S_MDL_CHUNK_HEADER {
        IZ_UINT magicChunk;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SCENEGRAPH_MDL_FORMAT_H__)
