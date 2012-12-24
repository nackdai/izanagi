#if !defined(__IZANAGI_IMG_LIB_DLL_IMAGE_GRAPHICS_DEVICE_H__)
#define __IZANAGI_IMG_LIB_DLL_IMAGE_GRAPHICS_DEVICE_H__

#include "izStd.h"
#include "izToolKit.h"
#include "izImageLib.h"

namespace izanagi {

// グラフィックスデバイス初期化
tool::CGraphicsDeviceLite* InitGraphicsDevice(void* hWnd);

// グラフィックスデバイス解放
void ReleaseGraphicsDevice();

// グラフィックスデバイス取得
tool::CGraphicsDeviceLite* GetGraphicsDevice();

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_IMG_LIB_DLL_H__)
