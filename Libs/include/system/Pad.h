#if !defined(__IZANAGI_SYSTEM_PAD_H__)
#define __IZANAGI_SYSTEM_PAD_H__

#include "izStd.h"
#include "InputDeviceDefs.h"

namespace izanagi {
    /**
     */
    struct PadState
    {
        IZ_FLOAT axisLeft[2];
        IZ_FLOAT axisRight[2];

        IZ_FLOAT triggerLeft;
        IZ_FLOAT triggerRight;
        
        IZ_BYTE  buttons[32];
    };

    /** パッドボタン
     */
	enum E_PAD_BUTTON
    {
        E_PAD_BUTTON_R_DOWN,
		E_PAD_BUTTON_R_RIGHT,
		E_PAD_BUTTON_R_LEFT,
		E_PAD_BUTTON_R_UP,

		E_PAD_BUTTON_L_1,
		E_PAD_BUTTON_R_1,

		E_PAD_BUTTON_SELECT,
		E_PAD_BUTTON_START,

		E_PAD_BUTTON_L_3,
		E_PAD_BUTTON_R_3,

        // TODO
        E_PAD_BUTTON_L_2,
		E_PAD_BUTTON_R_2,

        E_PAD_BUTTON_L_UP,
		E_PAD_BUTTON_L_RIGHT,
        E_PAD_BUTTON_L_DOWN,
        E_PAD_BUTTON_L_LEFT,

		E_PAD_BUTTON_NUM,
		E_PAD_BUTTON_FORCE_INT32 = 0x7fffffff,
	};

    /** パッドタイプ
     */
    enum E_PAD_TYPE
    {
        E_PAD_TYPE_DIRECT_INPUT,
        E_PAD_TYPE_X_INPUT,
    };

    /**
     */
	class CPad : public CObject {
	public:
		// インスタンス作成
		static CPad* CreatePad(
            IMemoryAllocator* allocator,
            SInputDeviceInitParam* initParam,
            IZ_FLOAT analogStickDeadZone);

	protected:
		CPad();
		virtual ~CPad();

		NO_COPIABLE(CPad);

        IZ_DEFINE_INTERNAL_RELEASE();

	public:
		// 更新
		PURE_VIRTUAL(IZ_BOOL Update());

	public:
		// キーを押し続けているか
		IZ_BOOL IsPressKey(E_PAD_BUTTON key)
        {
		    IZ_BOOL ret = IZ_FALSE;
		    if (m_bSucceedUpdate) {
			    ret = (m_CurState.buttons[key] > 0);
		    }
		    return ret;
	    }

		// キーを一度だけ押したかどうか
		IZ_BOOL IsPressOneShotKey(E_PAD_BUTTON key)
        {
		    IZ_BOOL ret = IZ_FALSE;
		    if (m_bSucceedUpdate) {
			    ret = (m_CurState.buttons[key] > 0
                        && m_LastState.buttons[key] == 0);
		    }
		    return ret;
	    }

        // キーが離されたかどうか
        IZ_BOOL IsReleaseKey(E_PAD_BUTTON key)
        {
            IZ_BOOL ret = IZ_FALSE;
		    if (m_bSucceedUpdate)
            {
                ret = (m_CurState.buttons[key] == 0
                        && m_LastState.buttons[key] > 0);
            }
            return ret;
        }

		const PadState& GetCurState() const
        {
		    return m_CurState;
	    }
		const PadState& GetLastState() const
        {
            return m_LastState;
        }

        E_PAD_TYPE GetType() const
        {
            return m_Type;
        }

        virtual IZ_BOOL IsEnableVibrate() const
        {
            return IZ_FALSE;
        }

        PURE_VIRTUAL(const void* GetRawState() const);

	protected:
		IMemoryAllocator* m_Allocator;

		PadState m_CurState;
		PadState m_LastState;

		// 更新に成功したかどうか
		IZ_BOOL m_bSucceedUpdate;

        // アナログスティックのデッドゾーン
        IZ_FLOAT m_AnalogStickDeadZone;

        E_PAD_TYPE m_Type;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_PAD_H__)
