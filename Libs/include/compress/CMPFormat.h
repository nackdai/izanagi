#if !defined(__IZANAGI_COMPRESS_CMP_FORMAT_H__)
#define __IZANAGI_COMPRESS_CMP_FORMAT_H__

#include "izDefs.h"

// CMP = CoMPress

namespace izanagi {
    // CMPファイルバージョン
    enum E_CMP_VERSION {
        E_CMP_VERSION_0010 = 0x30303130,                // バージョン0010

        E_CMP_VERSION_CURRENT = E_CMP_VERSION_0010,     // 現在のバージョン
        E_CMP_VERSION_FORCE_INT32 = 0x7FFFFFFF,         // enum のサイズを IZ_INT と同じにするため.
    };

    enum {
        // TODO
        // "CMP "
        CMP_MAGIC_NUMBER = 0x464E5400,
    };

    enum {
        IZ_CMP_MAX_BLOCK_SIZE = 2048,
        IZ_CMP_MAX_MATCH_LEN = 256,

        IZ_CMP_TYPE_FLAG_SLIDE_DIC   = 1 << 0,  // SlideDictionary
        IZ_CMP_TYPE_FLAG_RANGE_CODER = 1 << 1,  // RangeCoder
    };

    /**
    */
    struct S_CMP_HEADER {
        IZ_UINT magic;
        IZ_UINT version;

        IZ_UINT sizeFile;       // ファイル長
        IZ_UINT sizeHeader;     // ヘッダ長

        IZ_UINT numBlock;       // ブロック数
        IZ_UINT sizeOrg;        // オリジナルサイズ
        IZ_UINT sizeBlock;      // ブロックサイズ
    };

    /**
    */
    struct S_CMP_BLOCK_HEADER {
        IZ_UINT sizeCompressed;     // ブロックサイズ
        IZ_UINT sizeOrg;            // オリジナルサイズ

        IZ_UINT sizeDictMatchLen;
        IZ_UINT cmpFlag;
    };
}   // namespace izanagi

// CMPフォーマットかどうか
#define IS_CMP_FORMAT(f)            ((f) == izanagi::CMP_MAGIC_NUMBER)

// 最新バージョンかどうか
#define IS_CURRENT_CMP_VERSION(v)   ((v) == izanagi::E_CMP_VERSION_CURRENT)

#endif  // #if !defined(__IZANAGI_COMPRESS_CMP_FORMAT_H__)
