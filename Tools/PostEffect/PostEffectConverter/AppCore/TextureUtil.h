#if !defined(__POSTEFFECT_CONVERTER_TEXTURE_UTIL_H__)
#define __POSTEFFECT_CONVERTER_TEXTURE_UTIL_H__

#include <Cg/cgD3D9.h>
#include "izToolKit.h"
#include "izGraph.h"
#include "izPostEffect.h"

class CTextureUtil {
public:
	static BOOL CTextureUtil::SetAnnValue(
		izanagi::S_PES_TEXTURE_ANN& sAnn, 
		::CGparameter param);
};

#endif	 //#if !defined(__POSTEFFECT_CONVERTER_TEXTURE_UTIL_H__)
