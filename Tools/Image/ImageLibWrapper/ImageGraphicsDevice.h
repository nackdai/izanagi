#if !defined(__IZANAGI_IMG_LIB_DLL_IMAGE_GRAPHICS_DEVICE_H__)
#define __IZANAGI_IMG_LIB_DLL_IMAGE_GRAPHICS_DEVICE_H__

#include "izStd.h"
#include "izToolKit.h"
#include "izImageLib.h"

namespace izanagi {

// �O���t�B�b�N�X�f�o�C�X������
tool::CGraphicsDeviceLite* InitGraphicsDevice(void* hWnd);

// �O���t�B�b�N�X�f�o�C�X���
void ReleaseGraphicsDevice();

// �O���t�B�b�N�X�f�o�C�X�擾
tool::CGraphicsDeviceLite* GetGraphicsDevice();

}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_IMG_LIB_DLL_H__)
