#if !defined(__IZANAGI_IMG_LIB_DLL_H__)
#define __IZANAGI_IMG_LIB_DLL_H__

#include "izStd.h"
#include "izToolKit.h"
#include "izImageLib.h"

namespace izanagi {

tool::CGraphicsDeviceLite* InitGraphicsDevice(void* hWnd);
void ReleaseGraphicsDevice();
tool::CGraphicsDeviceLite* GetGraphicsDevice();

// ì«Ç›çûÇ›
tool::CIMGTexture* Read(
	const char* path,
	izanagi::E_GRAPH_TEX_TYPE type);

}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IMG_LIB_DLL_H__)
