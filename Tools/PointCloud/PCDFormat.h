#if !defined(__PCD_FORMAT_H__)
#define __PCD_FORMAT_H__

#include "izTypes.h"

enum VtxFormat {
    Position = 1 << 0,
    Color = 1 << 1,
    Normal = 1 << 2,
};

struct PCDHeader {
    IZ_UINT32 fileSize;
    IZ_UINT32 vtxFormat;

    IZ_UINT64 vtxNum;

    float aabbMin[3];
    float aabbMax[3];
};

struct Point {
    IZ_FLOAT pos[3];
    IZ_UINT8 rgba[4];
};

#endif  // #if !defined(__PCD_FORMAT_H__)
