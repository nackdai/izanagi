#if !defined(__SHADER_CONVERTER_PASS_UTIL_H__)
#define __SHADER_CONVERTER_PASS_UTIL_H__

#include <vector>
#include <Cg/cg.h>
#include "izShader.h"
#include "izToolKit.h"

class CPassUtil {
public:
#if 0
    static BOOL SetAnnValue(
        izanagi::S_SHD_PASS_ANN& sAnn,
        CGpass pass);
#endif

    static BOOL SetStateValue(
        izanagi::S_SHD_PASS_STATE& sDesc,
        CGpass pass);

#if 0
    static BOOL SetVSType(
        izanagi::S_SHD_PASS& sPass,
        CGpass pass);
#endif

    static CGprogram GetVSProgram(CGpass pass);
    static CGprogram GetPSProgram(CGpass pass);

    static IZ_PCSTR GetTextureOffsetParameterName(CGpass pass);
    static IZ_PCSTR GetRenderTargetName(CGpass pass);

    static void GetFunctorArgsString(
        CGpass pass,
        std::vector<izanagi::tool::CString>& tvRet);

    static IZ_PCSTR GetFunctorName(CGpass pass);
};

#endif  // #if !defined(__SHADER_CONVERTER_PASS_UTIL_H__)
