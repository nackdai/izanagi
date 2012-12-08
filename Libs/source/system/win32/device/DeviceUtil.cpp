#include "izStd.h"
#include "izD3DDefs.h"
#include "system/InputDeviceDefs.h"

IZ_BOOL InitDirectInput(
    izanagi::sys::SInputDeviceInitParam& param,
    D_INPUT** input)
{
#if 0
		HRESULT hr = CoInitialize(NULL);
		IZ_ASSERT(SUCCEEDED(hr));

		D_INPUT input = NULL;
		hr = CoCreateInstance(
				CLSID_DirectInput8,
				NULL, 
				CLSCTX_INPROC,
				IID_IDirectInput8,
				(void**)&input);
		IZ_ASSERT(SUCCEEDED(hr));

		hr = input->Initialize(hInst, DIRECTINPUT_VERSION);
#else
		HRESULT hr = DirectInput8Create(
						(HINSTANCE)param.systemData,
						DIRECTINPUT_VERSION,
						IID_IDirectInput8,
						(void**)input,
						NULL);

        IZ_BOOL ret = SUCCEEDED(hr);
        IZ_ASSERT(ret);

        return ret;
#endif
}
