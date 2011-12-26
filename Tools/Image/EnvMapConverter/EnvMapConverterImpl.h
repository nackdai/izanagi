#if !defined(__ENV_MAP_CONVERTER_IMPL_H__)
#define __ENV_MAP_CONVERTER_IMPL_H__

#include <vector>
#include "izToolKit.h"
#include "EnvMapConverterDefs.h"

void Convert(
	std::vector<izanagi::tool::CTextureLite*>& inTex,
	EnvMapType inType,
	std::vector<izanagi::tool::CTextureLite*>& outTex,
	EnvMapType outType);

#endif	// #if !defined(__ENV_MAP_CONVERTER_IMPL_H__)
