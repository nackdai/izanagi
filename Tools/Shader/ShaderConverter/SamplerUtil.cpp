#include <algorithm>
#include "izToolKit.h"
#include "SamplerUtil.h"

namespace {
    izanagi::graph::E_GRAPH_TEX_ADDRESS TexAddrTable[] = {
        izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP,
        izanagi::graph::E_GRAPH_TEX_ADDRESS_WRAP,
        izanagi::graph::E_GRAPH_TEX_ADDRESS_MIRROR,
    };

    izanagi::graph::E_GRAPH_TEX_FILTER TexFilterTable[] = {
        izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
        izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
        izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR,
    };

    // AddressU
    BOOL _SetValueAddressU(izanagi::graph::S_SAMPLER_STATE& sState, CGstateassignment sa)
    {
        INT num;
        const int* p = cgGetIntStateAssignmentValues(sa, &num);
        if (p != NULL) {
            int n = *p;
            sState.addressU = TexAddrTable[n];
        }
        return (p != NULL);
    }

    // AddressV
    BOOL _SetValueAddressV(izanagi::graph::S_SAMPLER_STATE& sState, CGstateassignment sa)
    {
        INT num;
        const int* p = cgGetIntStateAssignmentValues(sa, &num);
        if (p != NULL) {
            int n = *p;
            sState.addressV = TexAddrTable[n];
        }
        return (p != NULL);
    }

    // MinFilter
    BOOL _SetValueMinFilter(izanagi::graph::S_SAMPLER_STATE& sState, CGstateassignment sa)
    {
        INT num;
        const int* p = cgGetIntStateAssignmentValues(sa, &num);
        if (p != NULL) {
            int n = *p;
            sState.minFilter = TexFilterTable[n];
        }
        return (p != NULL);
    }

    // MagFilter
    BOOL _SetValueMagFilter(izanagi::graph::S_SAMPLER_STATE& sState, CGstateassignment sa)
    {
        INT num;
        const int* p = cgGetIntStateAssignmentValues(sa, &num);
        if (p != NULL) {
            int n = *p;
            sState.magFilter = TexFilterTable[n];
        }
        return (p != NULL);
    }

    // MipFilter
    BOOL _SetValueMipFilter(izanagi::graph::S_SAMPLER_STATE& sState, CGstateassignment sa)
    {
        INT num;
        const int* p = cgGetIntStateAssignmentValues(sa, &num);
        if (p != NULL) {
            int n = *p;
            sState.mipFilter = TexFilterTable[n];
        }
        return (p != NULL);
    }

    // 各種ステートをセット
    BOOL _SetStateValue(
        izanagi::graph::S_SAMPLER_STATE& sState,
        CGstateassignment sa)
    {
        static struct {
            const char* Name;
            BOOL (*Func)(izanagi::graph::S_SAMPLER_STATE&, CGstateassignment);
        } StateTable[] = {
            {"AddressU", _SetValueAddressU},
            {"AddressV", _SetValueAddressV},
            {"MinFilter", _SetValueMinFilter},
            {"MagFilter", _SetValueMagFilter},
            {"MipFilter", _SetValueMipFilter},
        };

        BOOL ret = TRUE;

        CGstate state = ::cgGetSamplerStateAssignmentState(sa);
        izanagi::tool::CString str(::cgGetStateName(state));

        for (IZ_UINT i = 0; i < COUNTOF(StateTable); i++) {
            if (str == StateTable[i].Name) {
                ret = (*StateTable[i].Func)(sState, sa);
                break;
            }
        }

        IZ_ASSERT(ret);
        return ret;
    }
}   // namespace

BOOL CSamplerUtil::SetStateValue(
    izanagi::shader::S_SHD_SAMPLER& sDesc, 
    CGparameter param)
{
    CGstateassignment sa = ::cgGetFirstSamplerStateAssignment(param);
    
    while (sa != NULL) {
        VRETURN(_SetStateValue(sDesc.state, sa));
        sa = ::cgGetNextStateAssignment(sa);
    }

    return TRUE;
}

BOOL CSamplerUtil::BindTexture(
    izanagi::shader::S_SHD_SAMPLER& sDesc,
    CGparameter param,
    std::vector<CGparameter>& tvTexList)
{
    CGstateassignment sa = ::cgGetFirstSamplerStateAssignment(param);
    
    while (sa != NULL) {
        CGstate state = cgGetSamplerStateAssignmentState(sa);
        izanagi::tool::CString str(::cgGetStateName(state));

        if (str == "Texture") {
            CGparameter tex = ::cgGetTextureStateAssignmentValue(sa);

            std::vector<CGparameter>::iterator it = std::find(
                                                        tvTexList.begin(),
                                                        tvTexList.end(),
                                                        tex);
            VRETURN(it != tvTexList.end());

            // サンプラにバインドするテクスチャのインデックス
            sDesc.BindTexIdx = static_cast<IZ_INT>(std::distance(tvTexList.begin(), it));

            break;
        }

        sa = ::cgGetNextStateAssignment(sa);
    }

    return TRUE;
}

#if 0


    // Texture
    inline BOOL _SetValueTexture(SSamplerDesc& sDesc, CGstateassignment sa)
    {
        sDesc.Texture = cgGetTextureStateAssignmentValue(sa);
        return (sDesc.Texture != NULL);
    }


#endif
