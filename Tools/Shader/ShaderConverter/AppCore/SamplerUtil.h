#if !defined(__SHADER_CONVERTER_SAMPLER_UTIL_H__)
#define __SHADER_CONVERTER_SAMPLER_UTIL_H__

#include <vector>
#include <Cg/cgD3D9.h>
#include "izGraph.h"
#include "izShader.h"

class CSamplerUtil {
public:
	static BOOL SetStateValue(
		izanagi::S_SHD_SAMPLER& sDesc, 
		CGparameter param);

	static BOOL BindTexture(
		izanagi::S_SHD_SAMPLER& sDesc, 
		CGparameter param,
		std::vector<CGparameter>& tvTexList);
};

#endif	 //#if !defined(__SHADER_CONVERTER_SAMPLER_UTIL_H__)
