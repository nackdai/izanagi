#if !defined(__POSTEFFECT_CONVERTER_PASS_UTIL_H__)
#define __POSTEFFECT_CONVERTER_PASS_UTIL_H__

#include <vector>
#include <Cg/cg.h>
#include "izPostEffect.h"
#include "izToolKit.h"

class CPassUtil {
public:
    static BOOL SetAnnValue(
        izanagi::S_PES_PASS_ANN& sAnn,
        CGpass pass);

    static BOOL SetStateValue(
        izanagi::S_PES_PASS_STATE& sDesc,
        CGpass pass);

    static BOOL SetVSType(
        izanagi::S_PES_PASS& sPass,
        CGpass pass);

    static CGprogram GetVSProgram(CGpass pass);
    static CGprogram GetPSProgram(CGpass pass);

    static IZ_PCSTR GetTextureOffsetParameterName(CGpass pass);
    static IZ_PCSTR GetRenderTargetName(CGpass pass);

    static void GetFunctorArgsString(
        CGpass pass,
        std::vector<izanagi::tool::CString>& tvRet);

    static IZ_PCSTR GetFunctorName(CGpass pass);
};

#endif  // #if !defined(__POSTEFFECT_CONVERTER_PASS_UTIL_H__)
