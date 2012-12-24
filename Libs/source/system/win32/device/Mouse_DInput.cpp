#include "system/Mouse_DInput.h"

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
    CMouse* CMouseDInput::CreateMouse(
        IMemoryAllocator* allocator,
        SInputDeviceInitParam* initParam)
    {
        IZ_UINT8* pBuf = IZ_NULL;
        CMouseDInput* pInstance = IZ_NULL;
        IZ_BOOL result = IZ_FALSE;

        // メモリ確保
        pBuf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CMouseDInput));
        result = (pBuf != IZ_NULL);
        if (!result) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // インスタンス作成
        pInstance = new(pBuf) CMouseDInput;
        {
            pInstance->AddRef();
            pInstance->m_Allocator = allocator;
        }

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
    CMouseDInput::CMouseDInput()
    {
        m_Allocator = IZ_NULL;
        m_pMouseDevice = IZ_NULL;
        m_hWnd = IZ_NULL;

        FILL_ZERO(&m_MouseState, sizeof(m_MouseState));

        m_bSucceedUpdate = IZ_FALSE;
    }

    // デストラクタ
    CMouseDInput::~CMouseDInput()
    {
        // 念のため
        if (m_pMouseDevice != IZ_NULL) {
            m_pMouseDevice->Unacquire();
        }

        SAFE_RELEASE(m_pMouseDevice);
        m_hWnd = IZ_NULL;
    }

    /**
    * 初期化
    */
    IZ_BOOL CMouseDInput::Init(SInputDeviceInitParam* initParam)
    {
        SInputDeviceInitParam* param = reinterpret_cast<SInputDeviceInitParam*>(initParam);

        D_INPUT* input = IZ_NULL;
        VRETURN(InitDirectInput(*param, &input));

        HWND hWnd = (HWND)param->nativeWindowHandle;

        IZ_BOOL ret = IZ_FALSE;

        // 本体作成
        HRESULT hr = input->CreateDevice(
                        GUID_SysMouse,
                        &m_pMouseDevice,
                        IZ_NULL);
        ret = SUCCEEDED(hr);
        VGOTO(ret, __EXIT__);
    
        // デバイスをマウスに設定
        // デフォルトの設定を使用
        hr = m_pMouseDevice->SetDataFormat(&c_dfDIMouse2);
        ret = SUCCEEDED(hr);
        VGOTO(ret, __EXIT__);

        // 協調レベルの設定
        hr = m_pMouseDevice->SetCooperativeLevel(
                hWnd,
                DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
        ret = SUCCEEDED(hr);
        VGOTO(ret, __EXIT__);

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
        ret = SUCCEEDED(hr);
        VGOTO(ret, __EXIT__);

        // 一応・・・
        m_hWnd = hWnd;

        //hr = m_pMouseDevice->Acquire();

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
    IZ_BOOL CMouseDInput::Update()
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

    /**
    * マウス右クリック
    */
    IZ_BOOL CMouseDInput::IsClickL() const
    {
        return IsClick(0);
    }

    /**
    * マウス左クリック
    */
    IZ_BOOL CMouseDInput::IsClickR() const
    {
        return IsClick(1);
    }

    /**
    * マウス中クリック
    */
    IZ_BOOL CMouseDInput::IsClickM() const
    {
        return IsClick(2);
    }

    /**
    * マウス移動Ｘ
    */
    IZ_DWORD CMouseDInput::GetX() const
    {
        IZ_DWORD ret = 0;
        if (m_bSucceedUpdate) {
            ret = (IZ_DWORD)m_MouseState.lX;
        }
        return ret;
    }

    /**
    * マウス移動Ｙ
    */
    IZ_DWORD CMouseDInput::GetY() const
    {
        IZ_DWORD ret = 0;
        if (m_bSucceedUpdate) {
            ret = (IZ_DWORD)m_MouseState.lY;
        }
        return ret;
    }

    /**
    * マウスホイール
    */
    IZ_DWORD CMouseDInput::GetZ() const
    {
        IZ_DWORD ret = 0;
        if (m_bSucceedUpdate) {
            ret = (IZ_DWORD)m_MouseState.lZ;
        }
        return ret;
    }
}   // namespace sys
}   // namespace izanagi
