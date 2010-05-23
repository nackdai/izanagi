#if !defined(__URANUS_IO_PAD_H__)
#define __URANUS_IO_PAD_H__

#include "../../unStd.h"
#include "InputDeviceDefs.h"

namespace uranus {
	class CPad : public CObject {
	public:
#if 0
		typedef DIJOYSTATE2 PadState;
#else
		typedef DIJOYSTATE PadState;
#endif

	public:
		// インスタンス作成
		static CPad* CreatePad(IMemoryAllocator* pAllocator);

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

		CPad(const CPad& rhs);
		const CPad& operator=(const CPad& rhs);

	protected:
		// 解放
		void InternalRelease();

	public:
		// 初期化
		UN_BOOL Init(SInputDeviceInitParam* pInitParam);

		// 更新
		UN_BOOL Update();

	public:
		// キーを押し続けているか
		inline UN_BOOL IsPushKey(E_PAD_BUTTON key);

		// キーを一度だけ押したかどうか
		inline UN_BOOL IsPushOneShotKey(E_PAD_BUTTON key);

		// アナログスティック位置取得
		inline void GetLeftAnalogStick(UN_FLOAT* x, UN_FLOAT* y);
		inline void GetRightAnalogStick(UN_FLOAT* x, UN_FLOAT* y);

	public:
		inline D_INPUT_DEVICE* GetRawInterface();
		inline HWND GetHWND();

		inline const PadState& GetCurState() const;
		inline const PadState& GetPrevState() const;

	protected:
		IMemoryAllocator* m_pAllocator;

		D_INPUT_DEVICE* m_pPadDevice;
		HWND m_hWnd;

		PadState m_CurState;
		PadState m_PrevState;

		// 更新に成功したかどうか
		UN_BOOL m_bSucceedUpdate;
	};

	// inlien *******************************

	/**
	* キーを押し続けているか
	*/
	UN_BOOL CPad::IsPushKey(E_PAD_BUTTON key)
	{
		UN_BOOL ret = UN_FALSE;
		if (m_bSucceedUpdate) {
			ret = (m_CurState.rgbButtons[key] > 0);
		}
		return ret;
	}

	/**
	* キーを一度だけ押したかどうか
	*/
	UN_BOOL CPad::IsPushOneShotKey(E_PAD_BUTTON key)
	{
		UN_BOOL ret = UN_FALSE;
		if (m_bSucceedUpdate) {
			ret = ((m_CurState.rgbButtons[key] > 0) && (m_PrevState.rgbButtons[key] == 0));
		}
		return ret;
	}

	/**
	* アナログスティック位置取得（左）
	*/
	void CPad::GetLeftAnalogStick(UN_FLOAT* x, UN_FLOAT* y)
	{
		if (x != UN_NULL) {
			*x = (UN_FLOAT)m_CurState.lX / ANALOG_STCIK_MAX;
		}
		if (y != UN_NULL) {
			*y = (UN_FLOAT)m_CurState.lX / ANALOG_STCIK_MAX;
		}
	}

	/**
	* アナログスティック位置取得（右）
	*/
	void CPad::GetRightAnalogStick(UN_FLOAT* x, UN_FLOAT* y)
	{
		if (x != UN_NULL) {
			*x = (UN_FLOAT)m_CurState.lZ / ANALOG_STCIK_MAX;
		}
		if (y != UN_NULL) {
			*y = (UN_FLOAT)m_CurState.lRz / ANALOG_STCIK_MAX;
		}
	}

	D_INPUT_DEVICE* CPad::GetRawInterface()
	{
		return m_pPadDevice;
	}

	HWND CPad::GetHWND()
	{
		return m_hWnd;
	}

	const CPad::PadState& CPad::GetCurState() const
	{
		return m_CurState;
	}

	const CPad::PadState& CPad::GetPrevState() const
	{
		return m_PrevState;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_PAD_H__)
