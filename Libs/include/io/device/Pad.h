#if !defined(__IZANAGI_IO_PAD_H__)
#define __IZANAGI_IO_PAD_H__

#include "izStd.h"
#include "InputDeviceDefs.h"

namespace izanagi {
    /**
     */
    struct PadState
    {
        IZ_FLOAT axisX[2];
        IZ_FLOAT axisY[2];
        
        IZ_BYTE  buttons[32];
    };

    /**
     */
	class CPad : public CObject {
    public:
        // TODO
        typedef DIJOYSTATE RawPadState;

	public:
		// インスタンス作成
		static CPad* CreatePad(
            IMemoryAllocator* pAllocator,
            SInputDeviceInitParam* initParam,
            IZ_FLOAT analogStickDeadZone);

	protected:
		enum {
			ANALOG_STCIK_MAX = 65535,

			ANALOG_STICK_DEAD_ZONE = 1500,	// 15%
		};

	protected:
		// デバイス列挙コールバック
		static BOOL CALLBACK EnumGameCtrlCallback(
			const DIDEVICEINSTANCE* pdidInstance,
			void* pContext);

		// パッド設定コールバック
		static BOOL CALLBACK EnumPadAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

	protected:
		CPad();
		~CPad();

		NO_COPIABLE(CPad);

        IZ_DEFINE_INTERNAL_RELEASE();

	protected:
        // 初期化
		IZ_BOOL Init(SInputDeviceInitParam* pInitParam);

	public:
		// 更新
		IZ_BOOL Update();

	public:
		// キーを押し続けているか
		IZ_BOOL IsPushKey(E_PAD_BUTTON key)
        {
		    IZ_BOOL ret = IZ_FALSE;
		    if (m_bSucceedUpdate) {
			    ret = (m_CurState.buttons[key] > 0);
		    }
		    return ret;
	    }

		// キーを一度だけ押したかどうか
		IZ_BOOL IsPushOneShotKey(E_PAD_BUTTON key)
        {
		    IZ_BOOL ret = IZ_FALSE;
		    if (m_bSucceedUpdate) {
			    ret = (m_CurState.buttons[key] > 0);
		    }
		    return ret;
	    }

		const PadState& GetCurState() const
        {
		    return m_CurState;
	    }
		const PadState& GetPrevState() const
        {
            return m_PrevState;
        }

        // TODO
        const RawPadState& GetRawState() const
        {
            return m_RawState;
        }

	protected:
		IMemoryAllocator* m_Allocator;

		D_INPUT_DEVICE* m_pPadDevice;
		HWND m_hWnd;

		PadState m_CurState;
		PadState m_PrevState;

        RawPadState m_RawState;

		// 更新に成功したかどうか
		IZ_BOOL m_bSucceedUpdate;

        // アナログスティックのデッドゾーン
        IZ_FLOAT m_AnalogStickDeadZone
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IO_PAD_H__)
