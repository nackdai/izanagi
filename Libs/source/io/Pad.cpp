#include "io/device/Pad.h"

using namespace izanagi;

/**
* インスタンス作成
*/
CPad* CPad::CreatePad(
    IMemoryAllocator* pAllocator,
    SInputDeviceInitParam* initParam)
{
	IZ_UINT8* pBuf = IZ_NULL;
	CPad* pInstance = IZ_NULL;
	IZ_BOOL result = IZ_FALSE;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CPad));
	result = (pBuf != IZ_NULL);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new(pBuf) CPad;
	{
		pInstance->AddRef();
		pInstance->m_Allocator = pAllocator;

        result = pInstance->Init(initParam);
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

namespace {
	D_INPUT* g_pInputTmp = IZ_NULL;
	D_INPUT_DEVICE* g_pDevTmp = IZ_NULL;
}	// namespace 

// TODO
// デバイス列挙コールバック
BOOL CALLBACK CPad::EnumGameCtrlCallback(
	const DIDEVICEINSTANCE* pdidInstance,
	void* pContext)
{
	HRESULT hr;

	// ゲームスティックデバイスを作成する
	hr = g_pInputTmp->CreateDevice(
			pdidInstance->guidInstance,
			&g_pDevTmp,
			NULL);

	if(FAILED(hr)){
		// デバイスの作成に失敗したら列挙を続ける
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}

// パッド設定コールバック
BOOL CALLBACK CPad::EnumPadAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
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

		hr = g_pDevTmp->SetProperty(DIPROP_RANGE, &prop.diph);
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
		prop.dwData = ANALOG_STICK_DEAD_ZONE;

		hr = g_pDevTmp->SetProperty(DIPROP_DEADZONE, &prop.diph);
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

		hr = g_pDevTmp->SetProperty(DIPROP_AXISMODE, &prop.diph);
		if(FAILED(hr)){
			return DIENUM_STOP;
		}
	}

	return DIENUM_CONTINUE;
}

// コンストラクタ
CPad::CPad()
{
	m_Allocator = IZ_NULL;
	m_pPadDevice = IZ_NULL;
	m_hWnd = IZ_NULL;

	FILL_ZERO(&m_CurState, sizeof(m_CurState));
	FILL_ZERO(&m_PrevState, sizeof(m_PrevState));

	m_bSucceedUpdate = IZ_FALSE;
}

// デストラクタ
CPad::~CPad()
{
	// 念のため
	if (m_pPadDevice != IZ_NULL) {
		m_pPadDevice->Unacquire();
	}

	SAFE_RELEASE(m_pPadDevice);
	m_hWnd = IZ_NULL;
}

/**
* 初期化
*/
IZ_BOOL CPad::Init(SInputDeviceInitParam* pInitParam)
{
	g_pInputTmp = pInitParam->pInput;

	HRESULT hr;

	// 利用可能なパッドの列挙関数を実行
	hr = pInitParam->pInput->EnumDevices(
			DI8DEVCLASS_GAMECTRL,
			EnumGameCtrlCallback,
			NULL,
			DIEDFL_ATTACHEDONLY);
	VRETURN(SUCCEEDED(hr));

	m_pPadDevice = g_pDevTmp;
#if 0
	VRETURN(m_pPadDevice != IZ_NULL);
#else
    if (m_pPadDevice == IZ_NULL)
    {
        return IZ_FALSE;
    }
#endif
	
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
			pInitParam->hWnd,
			DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	VRETURN(SUCCEEDED(hr));

	// パッドのプロパティを列挙して設定

	// アナログスティック設定
	hr = m_pPadDevice->EnumObjects(
			EnumPadAxisCallback,
			(void*)pInitParam->hWnd,
			DIDFT_AXIS);
	VRETURN(SUCCEEDED(hr));

	// 一応・・・
	m_hWnd = pInitParam->hWnd;

	return IZ_TRUE;
}

/**
* 更新
*/
IZ_BOOL CPad::Update()
{
	IZ_BOOL ret = (m_pPadDevice != IZ_NULL);

	if (ret) {
		// デバイスの取得
		HRESULT hr = m_pPadDevice->Acquire();

		ret = SUCCEEDED(hr);

		// 前の状態を保持
		memcpy(&m_PrevState, &m_CurState, sizeof(m_CurState));

		if (ret) {
			m_bSucceedUpdate = IZ_TRUE;

            DIJOYSTATE state;
			m_pPadDevice->GetDeviceState(sizeof(state), &state);

            // TODO
            m_pPadDevice->GetDeviceState(sizeof(m_RawState), &m_RawState);

#if 1
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
                m_CurState.axisX = (IZ_FLOAT)state.lX / ANALOG_STCIK_MAX;
                m_CurState.axisY = (IZ_FLOAT)state.lY / ANALOG_STCIK_MAX;

                for (IZ_UINT i = 0; i < E_PAD_BUTTON_NUM; i++)
                {
                    m_CurState.buttons[i] = state.rgbButtons[i];
                }
            }
#endif
		}
		else {
			FILL_ZERO(&m_CurState, sizeof(m_CurState));
		}
	}

	return ret;
}
