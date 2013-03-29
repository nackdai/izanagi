#include "system/Pad_DInput.h"

#define ANALOG_STCIK_MAX    (65535)

extern IZ_BOOL InitDirectInput(
    izanagi::sys::SInputDeviceInitParam& param,
    D_INPUT** input);

namespace izanagi
{
namespace sys
{
    /**
    * インスタンス作成
    */
    CPad* CPadDInput::CreatePad(
        IMemoryAllocator* pAllocator,
        SInputDeviceInitParam* initParam,
        IZ_FLOAT analogStickDeadZone)
    {
        IZ_UINT8* pBuf = IZ_NULL;
        CPadDInput* pInstance = IZ_NULL;
        IZ_BOOL result = IZ_FALSE;

        // メモリ確保
        pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CPadDInput));
        result = (pBuf != IZ_NULL);
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        pInstance = new(pBuf) CPadDInput;
        {
            pInstance->AddRef();
            pInstance->m_Allocator = pAllocator;

            result = pInstance->Init(
                initParam,
                analogStickDeadZone);
        }

    __EXIT__:
        if (!result) {
            if (pInstance != IZ_NULL) {
                SAFE_RELEASE(pInstance);
            }
            else {
                pAllocator->Free(pBuf);
            }
        }
        return pInstance;
    }

    // TODO
    // デバイス列挙コールバック
    BOOL CALLBACK CPadDInput::EnumGameCtrlCallback(
        const DIDEVICEINSTANCE* pdidInstance,
        void* pContext)
    {
        CPadDInput* pad = reinterpret_cast<CPadDInput*>(pContext);
        HRESULT hr;

        // ゲームスティックデバイスを作成する
        hr = pad->m_InputTmp->CreateDevice(
                pdidInstance->guidInstance,
                &pad->m_DevTmp,
                NULL);

        if(FAILED(hr)){
            // デバイスの作成に失敗したら列挙を続ける
            return DIENUM_CONTINUE;
        }

        return DIENUM_STOP;
    }

    // パッド設定コールバック
    BOOL CALLBACK CPadDInput::EnumPadAxisCallback(
        LPCDIDEVICEOBJECTINSTANCE lpddoi,
        LPVOID pvRef)
    {
        CPadDInput* pad = reinterpret_cast<CPadDInput*>(pvRef);
        HRESULT hr;
    
        // 軸の範囲
        {
            DIPROPRANGE prop; 

            prop.diph.dwSize = sizeof(DIPROPRANGE); 
            prop.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
            prop.diph.dwHow = DIPH_BYID; 
            prop.diph.dwObj = lpddoi->dwType; 
            prop.lMin = -ANALOG_STCIK_MAX; 
            prop.lMax = ANALOG_STCIK_MAX; 

            hr = pad->m_DevTmp->SetProperty(DIPROP_RANGE, &prop.diph);
            if(FAILED(hr)){
                return DIENUM_STOP;
            }
        }

        // デッドゾーン
        {
            DIPROPDWORD prop;

            prop.diph.dwSize = sizeof(DIPROPDWORD); 
            prop.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
            prop.diph.dwHow = DIPH_BYID; 
            prop.diph.dwObj = lpddoi->dwType; 

            prop.dwData = (DWORD)(ANALOG_STCIK_MAX * pad->m_AnalogStickDeadZone);

            hr = pad->m_DevTmp->SetProperty(DIPROP_DEADZONE, &prop.diph);
            if(FAILED(hr)){
                return DIENUM_STOP;
            }
        }

        // 軸のモード
        {
            DIPROPDWORD prop;

            prop.diph.dwSize = sizeof(DIPROPDWORD);
            prop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
            prop.diph.dwObj = 0;
            prop.diph.dwHow = DIPH_DEVICE;

            // 絶対位置モード
            prop.dwData = DIPROPAXISMODE_ABS;

            hr = pad->m_DevTmp->SetProperty(DIPROP_AXISMODE, &prop.diph);
            if(FAILED(hr)){
                return DIENUM_STOP;
            }
        }

        return DIENUM_CONTINUE;
    }

    // コンストラクタ
    CPadDInput::CPadDInput()
    {
        m_pPadDevice = IZ_NULL;
        m_hWnd = IZ_NULL;

        m_InputTmp = IZ_NULL;
        m_DevTmp = IZ_NULL;

        m_Type = E_PAD_TYPE_DIRECT_INPUT;
    }

    // デストラクタ
    CPadDInput::~CPadDInput()
    {
        // 念のため
        if (m_pPadDevice != IZ_NULL) {
            m_pPadDevice->Unacquire();
        }

        SAFE_RELEASE(m_pPadDevice);
        m_hWnd = IZ_NULL;

        SAFE_RELEASE(m_InputTmp);
    }

    /**
    * 初期化
    */
    IZ_BOOL CPadDInput::Init(
        SInputDeviceInitParam* initParam,
        IZ_FLOAT analogStickDeadZone)
    {
        SInputDeviceInitParam* param = reinterpret_cast<SInputDeviceInitParam*>(initParam);

        D_INPUT* input = IZ_NULL;
        VRETURN(InitDirectInput(*param, &input));

        HWND hWnd = (HWND)param->nativeWindowHandle;

        m_InputTmp = input;

        HRESULT hr;

        // 利用可能なパッドの列挙関数を実行
        hr = m_InputTmp->EnumDevices(
                DI8DEVCLASS_GAMECTRL,
                EnumGameCtrlCallback,
                this,
                DIEDFL_ATTACHEDONLY);
        VRETURN(SUCCEEDED(hr));

        m_pPadDevice = m_DevTmp;

        if (m_pPadDevice == IZ_NULL)
        {
            return IZ_FALSE;
        }
    
        // デバイスをパッドに設定
        // デフォルトの設定を使用
#if 0
        hr = m_pPadDevice->SetDataFormat(&c_dfDIJoystick2);
#else
        hr = m_pPadDevice->SetDataFormat(&c_dfDIJoystick);
#endif
        VRETURN(SUCCEEDED(hr));

        // 協調レベルの設定
        hr = m_pPadDevice->SetCooperativeLevel(
                hWnd,
                DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
        VRETURN(SUCCEEDED(hr));

        // パッドのプロパティを列挙して設定

        // アナログスティック設定
        hr = m_pPadDevice->EnumObjects(
                EnumPadAxisCallback,
                //(void*)pInitParam->hWnd,
                this,
                DIDFT_AXIS);
        VRETURN(SUCCEEDED(hr));

        // 一応・・・
        m_hWnd = hWnd;

        return IZ_TRUE;
    }

    /**
    * 更新
    */
    IZ_BOOL CPadDInput::Update()
    {
        IZ_BOOL ret = (m_pPadDevice != IZ_NULL);

        if (ret) {
            // デバイスの取得
            HRESULT hr = m_pPadDevice->Acquire();

            ret = SUCCEEDED(hr);

            // 前の状態を保持
            memcpy(&m_LastState, &m_CurState, sizeof(m_CurState));

            if (ret) {
                m_bSucceedUpdate = IZ_TRUE;

                DIJOYSTATE state;
                m_pPadDevice->GetDeviceState(sizeof(state), &state);

                // TODO
                m_pPadDevice->GetDeviceState(sizeof(m_RawState), &m_RawState);

                // 左キーの状態を POV -> Button に移す
                switch (state.rgdwPOV[0]) {
                case 0:
                    state.rgbButtons[E_PAD_BUTTON_L_UP] = 128;
                    break;
                case 4500:
                    state.rgbButtons[E_PAD_BUTTON_L_UP] = 128;
                    state.rgbButtons[E_PAD_BUTTON_L_RIGHT] = 128;
                    break;
                case 9000:
                    state.rgbButtons[E_PAD_BUTTON_L_RIGHT] = 128;
                    break;
                case 13500:
                    state.rgbButtons[E_PAD_BUTTON_L_RIGHT] = 128;
                    state.rgbButtons[E_PAD_BUTTON_L_DOWN] = 128;
                    break;
                case 18000:
                    state.rgbButtons[E_PAD_BUTTON_L_DOWN] = 128;
                    break;
                case 22500:
                    state.rgbButtons[E_PAD_BUTTON_L_DOWN] = 128;
                    state.rgbButtons[E_PAD_BUTTON_L_LEFT] = 128;
                    break;
                case 27000:
                    state.rgbButtons[E_PAD_BUTTON_L_LEFT] = 128;
                    break;
                case 31500:
                    state.rgbButtons[E_PAD_BUTTON_L_LEFT] = 128;
                    state.rgbButtons[E_PAD_BUTTON_L_UP] = 128;
                    break;
                }

                {
                    m_CurState.axisLeft[0] = (IZ_FLOAT)state.lX / ANALOG_STCIK_MAX;
                    m_CurState.axisLeft[1] = (IZ_FLOAT)state.lY / ANALOG_STCIK_MAX;

                    m_CurState.axisRight[0] = (IZ_FLOAT)state.lRx / ANALOG_STCIK_MAX;
                    m_CurState.axisRight[1] = (IZ_FLOAT)state.lRy / ANALOG_STCIK_MAX;

                    // Right Buttons
                    //   3
                    // 2   1
                    //   0

                    for (IZ_UINT i = 0; i < E_PAD_BUTTON_NUM; i++)
                    {
                        m_CurState.buttons[i] = (state.rgbButtons[i] > 0 ? 1 : 0);
                    }
                }
            }
            else {
                FILL_ZERO(&m_CurState, sizeof(m_CurState));
            }
        }

        return ret;
    }
}   // namespace sys
}   // namespce izanagi
