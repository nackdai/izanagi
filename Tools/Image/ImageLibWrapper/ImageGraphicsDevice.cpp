#include "ImageGraphicsDevice.h"

namespace izanagi {

static tool::CGraphicsDeviceLite* s_Device = IZ_NULL;

// �O���t�B�b�N�X�f�o�C�X������
tool::CGraphicsDeviceLite* InitGraphicsDevice(void* hWnd)
{
	// GraphcsDevice�쐬
	if (s_Device == IZ_NULL) {
		s_Device = izanagi::tool::CGraphicsDeviceLite::CreateGraphicsDeviceLight((HWND)hWnd);
		VRETURN(s_Device != IZ_NULL);
	}
	return s_Device;
}

// �O���t�B�b�N�X�f�o�C�X���
void ReleaseGraphicsDevice()
{
	SAFE_RELEASE(s_Device);
}

// �O���t�B�b�N�X�f�o�C�X�擾
tool::CGraphicsDeviceLite* GetGraphicsDevice()
{
	return s_Device;
}

}	// namespace izanagi
