#include <algorithm>
#include "SamplerUtil.h"
#include "izToolKit.h"

namespace {
	izanagi::E_GRAPH_TEX_ADDRESS TexAddrTable[] = {
		izanagi::E_GRAPH_TEX_ADDRESS_CLAMP,
		izanagi::E_GRAPH_TEX_ADDRESS_WRAP,
		izanagi::E_GRAPH_TEX_ADDRESS_MIRROR,
	};

	izanagi::E_GRAPH_TEX_FILTER TexFilterTable[] = {
		izanagi::E_GRAPH_TEX_FILTER_POINT,
		izanagi::E_GRAPH_TEX_FILTER_POINT,
		izanagi::E_GRAPH_TEX_FILTER_LINEAR,
	};

	// AddressU
	BOOL _SetValueAddressU(izanagi::S_PES_SAMPLER_STATE& sState, CGstateassignment sa)
	{
		INT num;
		const int* p = cgGetIntStateAssignmentValues(sa, &num);
		if (p != NULL) {
			int n = *p;
			sState.addrU = TexAddrTable[n];
		}
		return (p != NULL);
	}

	// AddressV
	BOOL _SetValueAddressV(izanagi::S_PES_SAMPLER_STATE& sState, CGstateassignment sa)
	{
		INT num;
		const int* p = cgGetIntStateAssignmentValues(sa, &num);
		if (p != NULL) {
			int n = *p;
			sState.addrV = TexAddrTable[n];
		}
		return (p != NULL);
	}

	// MinFilter
	BOOL _SetValueMinFilter(izanagi::S_PES_SAMPLER_STATE& sState, CGstateassignment sa)
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
	BOOL _SetValueMagFilter(izanagi::S_PES_SAMPLER_STATE& sState, CGstateassignment sa)
	{
		INT num;
		const int* p = cgGetIntStateAssignmentValues(sa, &num);
		if (p != NULL) {
			int n = *p;
			sState.magFilter = TexFilterTable[n];
		}
		return (p != NULL);
	}


	// 各種ステートをセット
	BOOL _SetStateValue(
		izanagi::S_PES_SAMPLER_STATE& sState,
		CGstateassignment sa)
	{
		static struct {
			const char* Name;
			BOOL (*Func)(izanagi::S_PES_SAMPLER_STATE&, CGstateassignment);
		} StateTable[] = {
			{"AddressU", _SetValueAddressU},
			{"AddressV", _SetValueAddressV},
			{"MinFilter", _SetValueMinFilter},
			{"MagFilter", _SetValueMagFilter},
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
}	// namespace

BOOL CSamplerUtil::SetStateValue(
	izanagi::S_PES_SAMPLER_STATE& sState, 
	CGparameter param)
{
	CGstateassignment sa = ::cgGetFirstSamplerStateAssignment(param);
	
	while (sa != NULL) {
		VRETURN(_SetStateValue(sState, sa));
		sa = ::cgGetNextStateAssignment(sa);
	}

	return TRUE;
}

BOOL CSamplerUtil::BindTexture(
	izanagi::S_PES_SAMPLER_STATE& sState, 
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

			sState.BindTexIdx = std::distance(tvTexList.begin(), it);

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
