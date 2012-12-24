#if !defined(__POSTEFFECT_CONVERTER_SAMPLER_UTIL_H__)
#define __POSTEFFECT_CONVERTER_SAMPLER_UTIL_H__

#include <vector>
#include <Cg/cgD3D9.h>
#include "izGraph.h"
#include "izPostEffect.h"

class CSamplerUtil {
public:
    static BOOL SetStateValue(
        izanagi::S_PES_SAMPLER_STATE& sState, 
        CGparameter param);

    static BOOL BindTexture(
        izanagi::S_PES_SAMPLER_STATE& sState, 
        CGparameter param,
        std::vector<CGparameter>& tvTexList);
};

#endif   //#if !defined(__POSTEFFECT_CONVERTER_SAMPLER_UTIL_H__)
