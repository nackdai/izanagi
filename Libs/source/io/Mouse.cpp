#include "io/device/Mouse.h"

using namespace izanagi;

/**
* インスタンス作成
*/
CMouse* CMouse::CreateMouse(IMemoryAllocator* pAllocator)
{
	IZ_UINT8* pBuf = IZ_NULL;
	CMouse* pInstance = IZ_NULL;
	IZ_BOOL result = IZ_FALSE;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CMouse));
	result = (pBuf != IZ_NULL);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
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
		if (pInstance != IZ_NULL) {
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
	m_pAllocator = IZ_NULL;
	m_pMouseDevice = IZ_NULL;
	m_hWnd = IZ_NULL;

	FILL_ZERO(&m_MouseState, sizeof(m_MouseState));

	m_bSucceedUpdate = IZ_FALSE;
}

// デストラクタ
CMouse::~CMouse()
{
	// 念のため
	if (m_pMouseDevice != IZ_NULL) {
		m_pMouseDevice->Unacquire();
	}

	SAFE_RELEASE(m_pMouseDevice);
	m_hWnd = IZ_NULL;
}

// 解放
void CMouse::InternalRelease()
{
	delete this;

	if (m_pAllocator != IZ_NULL) {
		m_pAllocator->Free(this);
	}
}

/**
* 初期化
*/
IZ_BOOL CMouse::Init(SInputDeviceInitParam* pInitParam)
{
	// 本体作成
	HRESULT hr = pInitParam->pInput->CreateDevice(
					GUID_SysMouse,
					&m_pMouseDevice,
					IZ_NULL);
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

	return IZ_TRUE;
}

/**
* 更新
*/
IZ_BOOL CMouse::Update()
{
	IZ_BOOL ret = (m_pMouseDevice != IZ_NULL);

	if (ret) {
		// デバイスの取得
		HRESULT hr = m_pMouseDevice->Acquire();

		ret = SUCCEEDED(hr);

		if (ret) {
			m_bSucceedUpdate = IZ_TRUE;
			m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_MouseState);
		}
	}

	return ret;
}
