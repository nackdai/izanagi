#include "ImageGraphicsDevice.h"

namespace izanagi {

static tool::CGraphicsDeviceLite* s_Device = IZ_NULL;

// グラフィックスデバイス初期化
tool::CGraphicsDeviceLite* InitGraphicsDevice(void* hWnd)
{
	// GraphcsDevice作成
	if (s_Device == IZ_NULL) {
		s_Device = izanagi::tool::CGraphicsDeviceLite::CreateGraphicsDeviceLight((HWND)hWnd);
		VRETURN(s_Device != IZ_NULL);
	}
	return s_Device;
}

// グラフィックスデバイス解放
void ReleaseGraphicsDevice()
{
	SAFE_RELEASE(s_Device);
}

// グラフィックスデバイス取得
tool::CGraphicsDeviceLite* GetGraphicsDevice()
{
	return s_Device;
}

}	// namespace izanagi
