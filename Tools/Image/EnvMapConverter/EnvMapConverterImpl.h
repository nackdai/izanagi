#if !defined(__ENV_MAP_CONVERTER_IMPL_H__)
#define __ENV_MAP_CONVERTER_IMPL_H__

#include "izToolKit.h"
#include "EnvMapConverterDefs.h"

void Convert(
	izanagi::izanagi_tk::CTextureLite* inTex,
	EnvMapType inType,
	izanagi::izanagi_tk::CTextureLite* outTex,
	EnvMapType outType);

#endif	// #if !defined(__ENV_MAP_CONVERTER_IMPL_H__)
