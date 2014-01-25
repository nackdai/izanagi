#if !defined(__IZANAGI_SYSTEM_DINPUT_H__)
#define __IZANAGI_SYSTEM_DINPUT_H__

#ifdef __IZ_DX9__

    #ifndef DIRECTINPUT_VERSION
        #define DIRECTINPUT_VERSION 0x0800 
    #endif

    #include <dinput.h>

    #define D_INPUT         IDirectInput8
    #define D_INPUT_DEVICE  IDirectInputDevice8
#endif

#endif  // #if !defined(__IZANAGI_SYSTEM_DINPUT_H__)
