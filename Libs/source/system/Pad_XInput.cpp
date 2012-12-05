#include "io/device/Pad_XInput.h"

#pragma comment(lib, "XInput.lib")

namespace izanagi
{
    /**
    * インスタンス作成
    */
    CPad* CPadXInput::CreatePad(
        IMemoryAllocator* pAllocator,
        IZ_FLOAT analogStickDeadZone)
    {
	    IZ_UINT8* pBuf = IZ_NULL;
	    CPadXInput* pInstance = IZ_NULL;
	    IZ_BOOL result = IZ_FALSE;

	    // メモリ確保
	    pBuf = (IZ_UINT8*)ALLOC_ZERO(pAllocator, sizeof(CPadXInput));
	    result = (pBuf != IZ_NULL);
	    if (!result) {
		    IZ_ASSERT(IZ_FALSE);
		    goto __EXIT__;
	    }

	    // インスタンス作成
	    pInstance = new(pBuf) CPadXInput;
	    {
		    pInstance->AddRef();
		    pInstance->m_Allocator = pAllocator;
            pInstance->m_AnalogStickDeadZone = analogStickDeadZone;
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
    CPadXInput::CPadXInput()
    {
        m_Type = E_PAD_TYPE_X_INPUT;
    }

    // デストラクタ
    CPadXInput::~CPadXInput()
    {
    }

    IZ_FLOAT _ComputeAxis(
        SHORT value,
        SHORT defaultValue)
    {
        static const IZ_FLOAT axisXDiv = 1.0f / 32767.0f;
        static const IZ_FLOAT axisYDiv = 1.0f / 32768.0f;

        IZ_FLOAT ret = 0;

        if (value != defaultValue)
        {
            ret = (value > 0
                ? value * axisXDiv
                : value * axisYDiv);
        }

        return ret;
    }

    /**
    * 更新
    */
    IZ_BOOL CPadXInput::Update()
    {
        // TODO
        // とりあえずコントローラは１つのみ
        DWORD result = XInputGetState(0, &m_RawState);
        IZ_BOOL ret = (result == ERROR_SUCCESS);

        if (ret)
        {
            // 前の状態を保持
		    memcpy(&m_PrevState, &m_CurState, sizeof(m_CurState));

            static IZ_UINT btnflags[] =
            {
                XINPUT_GAMEPAD_A,               // E_PAD_BUTTON_R_DOWN,
		        XINPUT_GAMEPAD_B,               // E_PAD_BUTTON_R_RIGHT,
                XINPUT_GAMEPAD_X,               // E_PAD_BUTTON_R_LEFT,
                XINPUT_GAMEPAD_Y,               // E_PAD_BUTTON_R_UP,

		        XINPUT_GAMEPAD_LEFT_SHOULDER,   // E_PAD_BUTTON_L_1,
		        XINPUT_GAMEPAD_RIGHT_SHOULDER,  // E_PAD_BUTTON_R_1,

		        XINPUT_GAMEPAD_BACK,            // E_PAD_BUTTON_SELECT,
		        XINPUT_GAMEPAD_START,           // E_PAD_BUTTON_START,

		        XINPUT_GAMEPAD_LEFT_THUMB,      // E_PAD_BUTTON_L_3,
		        XINPUT_GAMEPAD_RIGHT_THUMB,     // E_PAD_BUTTON_R_3,

                // TODO
                0,
		        0,

                XINPUT_GAMEPAD_DPAD_UP,         // E_PAD_BUTTON_L_UP,
		        XINPUT_GAMEPAD_DPAD_RIGHT,      // E_PAD_BUTTON_L_RIGHT,
                XINPUT_GAMEPAD_DPAD_DOWN,       // E_PAD_BUTTON_L_DOWN,
                XINPUT_GAMEPAD_DPAD_LEFT,       // E_PAD_BUTTON_L_LEFT,
            };

            for (IZ_UINT i = 0; i < COUNTOF(btnflags); i++)
            {
                m_CurState.buttons[i] = (((m_RawState.Gamepad.wButtons & btnflags[i]) > 0) ? 1 : 0);
            }

            static const IZ_FLOAT triggerDiv = 1.0f / 255.0f;
            

            m_CurState.triggerLeft = m_RawState.Gamepad.bLeftTrigger * triggerDiv;
            m_CurState.buttons[E_PAD_BUTTON_L_2] = (m_RawState.Gamepad.bLeftTrigger > 0 ? 1 : 0);

            m_CurState.triggerRight = m_RawState.Gamepad.bRightTrigger * triggerDiv;
            m_CurState.buttons[E_PAD_BUTTON_R_2] = (m_RawState.Gamepad.bRightTrigger > 0 ? 1 : 0);

            // NOTE
            // なぜかY軸方向のデフォルト値は-1

            m_CurState.axisLeft[0] = _ComputeAxis(m_RawState.Gamepad.sThumbLX, 0);
            m_CurState.axisLeft[1] = _ComputeAxis(m_RawState.Gamepad.sThumbLY, -1);

            m_CurState.axisRight[0] = _ComputeAxis(m_RawState.Gamepad.sThumbRX, 0);
            m_CurState.axisRight[1] = _ComputeAxis(m_RawState.Gamepad.sThumbRY, -1);
        }
        else
        {
            FILL_ZERO(&m_CurState, sizeof(m_CurState));
        }

        return ret;
    }
}   // namespce izanagi
