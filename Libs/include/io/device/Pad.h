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
		// インスタンス作成
		static CPad* CreatePad(
            IMemoryAllocator* allocator,
            void* initParam,
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

        PURE_VIRTUAL(const void* GetRawState() const);

	protected:
		IMemoryAllocator* m_Allocator;

		PadState m_CurState;
		PadState m_PrevState;

		// 更新に成功したかどうか
		IZ_BOOL m_bSucceedUpdate;

        // アナログスティックのデッドゾーン
        IZ_FLOAT m_AnalogStickDeadZone;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IO_PAD_H__)
