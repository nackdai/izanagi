#include "io/device/Mouse.h"

using namespace uranus;

/**
* インスタンス作成
*/
CMouse* CMouse::CreateMouse(IMemoryAllocator* pAllocator)
{
	UN_UINT8* pBuf = UN_NULL;
	CMouse* pInstance = UN_NULL;
	UN_BOOL result = UN_FALSE;

	// メモリ確保
	pBuf = (UN_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CMouse));
	result = (pBuf != UN_NULL);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new(pBuf) CMouse;
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
	}

__EXIT__:
	if (!result) {
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else {
			pAllocator->Free(pBuf);
		}
	}
	return pInstance;
}

// コンストラクタ
CMouse::CMouse()
{
	m_pAllocator = UN_NULL;
	m_pMouseDevice = UN_NULL;
	m_hWnd = UN_NULL;

	FILL_ZERO(&m_MouseState, sizeof(m_MouseState));

	m_bSucceedUpdate = UN_FALSE;
}

// デストラクタ
CMouse::~CMouse()
{
	// 念のため
	if (m_pMouseDevice != UN_NULL) {
		m_pMouseDevice->Unacquire();
	}

	SAFE_RELEASE(m_pMouseDevice);
	m_hWnd = UN_NULL;
}

// 解放
void CMouse::InternalRelease()
{
	delete this;

	if (m_pAllocator != UN_NULL) {
		m_pAllocator->Free(this);
	}
}

/**
* 初期化
*/
UN_BOOL CMouse::Init(SInputDeviceInitParam* pInitParam)
{
	// 本体作成
	HRESULT hr = pInitParam->pInput->CreateDevice(
					GUID_SysMouse,
					&m_pMouseDevice,
					UN_NULL);
	VRETURN(SUCCEEDED(hr));
	
	// デバイスをマウスに設定
	// デフォルトの設定を使用
	hr = m_pMouseDevice->SetDataFormat(&c_dfDIMouse2);
	VRETURN(SUCCEEDED(hr));

	// 協調レベルの設定
	hr = m_pMouseDevice->SetCooperativeLevel(
			pInitParam->hWnd,
			DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	VRETURN(SUCCEEDED(hr));

	// 軸モードの設定
	DIPROPDWORD diprop;
	{
		diprop.diph.dwSize = sizeof(DIPROPDWORD);
		diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;

#if 1
		// 相対位置モード
		diprop.dwData = DIPROPAXISMODE_REL;
#else
		// 絶対位置モード
		diprop.dwData = DIPROPAXISMODE_ABS;
#endif
	}
	hr = m_pMouseDevice->SetProperty(
			DIPROP_AXISMODE,
			&diprop.diph);
	VRETURN(SUCCEEDED(hr));

	// 一応・・・
	m_hWnd = pInitParam->hWnd;

	//hr = m_pMouseDevice->Acquire();

	return UN_TRUE;
}

/**
* 更新
*/
UN_BOOL CMouse::Update()
{
	UN_BOOL ret = (m_pMouseDevice != UN_NULL);

	if (ret) {
		// デバイスの取得
		HRESULT hr = m_pMouseDevice->Acquire();

		ret = SUCCEEDED(hr);

		if (ret) {
			m_bSucceedUpdate = UN_TRUE;
			m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState);
		}
	}

	return ret;
}
