#if !defined(__UTILITY_H__)
#define __UTILITY_H__

#include "izSampleKit.h"

class Utility {
public:
    static void SetShaderParam(
        izanagi::shader::CShaderBasic* shader,
        const char* name,
        const void* value,
        IZ_UINT bytes);
};

#endif    // #if !defined(__UTILITY_H__)