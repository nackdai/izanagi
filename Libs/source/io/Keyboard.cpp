#include "io/device/Keyboard.h"

using namespace izanagi;

/**
* インスタンス作成
*/
CKeyboard* CKeyboard::CreateKeyboard(IMemoryAllocator* pAllocator)
{
	IZ_UINT8* pBuf = IZ_NULL;
	CKeyboard* pInstance = IZ_NULL;
	IZ_BOOL result = IZ_FALSE;

	// メモリ確保
	pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CKeyboard));
	result = (pBuf != IZ_NULL);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new(pBuf) CKeyboard;
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
CKeyboard::CKeyboard()
{
	m_pAllocator = IZ_NULL;
	m_pKeyDevice = IZ_NULL;
	m_hWnd = IZ_NULL;

	FILL_ZERO(m_CurState, sizeof(m_CurState));
	FILL_ZERO(m_PrevState, sizeof(m_PrevState));

	m_bSucceedUpdate = IZ_FALSE;
}

// デストラクタ
CKeyboard::~CKeyboard()
{
	// 念のため
	if (m_pKeyDevice != IZ_NULL) {
		m_pKeyDevice->Unacquire();
	}

	SAFE_RELEASE(m_pKeyDevice);
	m_hWnd = IZ_NULL;
}

// 解放
void CKeyboard::InternalRelease()
{
	delete this;

	if (m_pAllocator != IZ_NULL) {
		m_pAllocator->Free(this);
	}
}

// 初期化
IZ_BOOL CKeyboard::Init(SInputDeviceInitParam* pInitParam)
{
	// 本体作成
	HRESULT hr = pInitParam->pInput->CreateDevice(
					GUID_SysKeyboard,
					&m_pKeyDevice,
					IZ_NULL);
	VRETURN(SUCCEEDED(hr));
	
	// デバイスをキーボードに設定
	// デフォルトの設定を使用
	hr = m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	VRETURN(SUCCEEDED(hr));

	// 協調レベルの設定
	hr = m_pKeyDevice->SetCooperativeLevel(
			pInitParam->hWnd,
			DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	VRETURN(SUCCEEDED(hr));

	// 一応・・・
	m_hWnd = pInitParam->hWnd;

	//hr = m_pKeyDevice->Acquire();

	return IZ_TRUE;
}

/**
* 更新
*/
IZ_BOOL CKeyboard::Update()
{
	IZ_BOOL ret = (m_pKeyDevice != IZ_NULL);

	if (ret) {
		// デバイスの取得
		HRESULT hr = m_pKeyDevice->Acquire();

		ret = SUCCEEDED(hr);

		// 前の状態を保持
		memcpy(m_PrevState, m_CurState, sizeof(m_PrevState));

		if (ret) {
			m_bSucceedUpdate = IZ_TRUE;
			m_pKeyDevice->GetDeviceState(sizeof(m_CurState), m_CurState);
		}
	}

	return ret;
}
