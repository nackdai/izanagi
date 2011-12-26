#if !defined(__ENV_MAP_CONVERTER_IMPL_H__)
#define __ENV_MAP_CONVERTER_IMPL_H__

#include <vector>
#include "izToolKit.h"
#include "EnvMapConverterDefs.h"

void Convert(
	std::vector<izanagi::izanagi_tk::CTextureLite*>& inTex,
	EnvMapType inType,
	std::vector<izanagi::izanagi_tk::CTextureLite*>& outTex,
	EnvMapType outType);

#endif	// #if !defined(__ENV_MAP_CONVERTER_IMPL_H__)
