#include "system/Keyboard_DInput.h"

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
    CKeyboard* CKeyboardDInput::CreateKeyboard(
        IMemoryAllocator* allocator,
        SInputDeviceInitParam* initParam)
    {
	    IZ_UINT8* pBuf = IZ_NULL;
	    CKeyboardDInput* pInstance = IZ_NULL;
	    IZ_BOOL result = IZ_FALSE;

	    // メモリ確保
	    pBuf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CKeyboardDInput));
	    result = (pBuf != IZ_NULL);
	    if (!result) {
		    IZ_ASSERT(IZ_FALSE);
		    goto __EXIT__;
	    }

	    // インスタンス作成
	    pInstance = new(pBuf) CKeyboardDInput;
	    {
		    pInstance->AddRef();
		    pInstance->m_Allocator = allocator;
	    }

        result = pInstance->Init(initParam);

    __EXIT__:
	    if (!result) {
		    if (pInstance != IZ_NULL) {
			    SAFE_RELEASE(pInstance);
		    }
		    else {
			    allocator->Free(pBuf);
		    }
	    }
	    return pInstance;
    }

    // コンストラクタ
    CKeyboardDInput::CKeyboardDInput()
    {
	    m_pKeyDevice = IZ_NULL;
	    m_hWnd = IZ_NULL;
    }

    // デストラクタ
    CKeyboardDInput::~CKeyboardDInput()
    {
	    // 念のため
	    if (m_pKeyDevice != IZ_NULL) {
		    m_pKeyDevice->Unacquire();
	    }

	    SAFE_RELEASE(m_pKeyDevice);
	    m_hWnd = IZ_NULL;
    }

    // 初期化
    IZ_BOOL CKeyboardDInput::Init(SInputDeviceInitParam* initParam)
    {
        SInputDeviceInitParam* param = reinterpret_cast<SInputDeviceInitParam*>(initParam);

        D_INPUT* input = IZ_NULL;
        VRETURN(InitDirectInput(*param, &input));

        HWND hWnd = (HWND)param->nativeWindowHandle;

        IZ_BOOL ret = IZ_FALSE;

	    // 本体作成
	    HRESULT hr = input->CreateDevice(
					    GUID_SysKeyboard,
					    &m_pKeyDevice,
					    IZ_NULL);
	    ret = SUCCEEDED(hr);
        VGOTO(ret, __EXIT__);
	
	    // デバイスをキーボードに設定
	    // デフォルトの設定を使用
	    hr = m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	    ret = SUCCEEDED(hr);
        VGOTO(ret, __EXIT__);

	    // 協調レベルの設定
	    hr = m_pKeyDevice->SetCooperativeLevel(
			    hWnd,
			    DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	    ret = SUCCEEDED(hr);
        VGOTO(ret, __EXIT__);

	    // 一応・・・
	    m_hWnd = hWnd;

	    //hr = m_pKeyDevice->Acquire();

__EXIT__:
        if (!ret)
        {
            SAFE_RELEASE(input);
        }

	    return IZ_TRUE;
    }

    /**
    * 更新
    */
    IZ_BOOL CKeyboardDInput::Update()
    {
	    IZ_BOOL ret = (m_pKeyDevice != IZ_NULL);

	    if (ret) {
		    // デバイスの取得
		    HRESULT hr = m_pKeyDevice->Acquire();

		    ret = SUCCEEDED(hr);

		    // 前の状態を保持
		    memcpy(m_LastState, m_CurState, sizeof(m_LastState));

		    if (ret) {
			    m_bSucceedUpdate = IZ_TRUE;
			    m_pKeyDevice->GetDeviceState(sizeof(m_CurState), m_CurState);
		    }
	    }

	    return ret;
    }
}   // namespace sys
}   // namespace izanagi
