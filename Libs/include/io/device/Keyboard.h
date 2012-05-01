#if !defined(__IZANAGI_IO_KEYBOARD_H__)
#define __IZANAGI_IO_KEYBOARD_H__

#include "izStd.h"
#include "InputDeviceDefs.h"

namespace izanagi {
	/**
	* キーボード
	*/
	class CKeyboard : public CObject {
	public:
		// インスタンス作成
		static CKeyboard* CreateKeyboard(IMemoryAllocator* pAllocator);

	protected:
		CKeyboard();
		~CKeyboard();

		CKeyboard(const CKeyboard& rhs);
		const CKeyboard& operator=(const CKeyboard& rhs);

	protected:
		// 解放
		void InternalRelease();

	public:
		// 初期化
		IZ_BOOL Init(SInputDeviceInitParam* pInitParam);

		// 更新
		IZ_BOOL Update();

	public:
		// キーを押し続けているか
		inline IZ_BOOL IsPushKey(E_KEYBOARD_BUTTON key);

		// キーを一度だけ押したかどうか
		inline IZ_BOOL IsPushOneShotKey(E_KEYBOARD_BUTTON key);

	public:
		inline D_INPUT_DEVICE* GetRawInterface();
		inline HWND GetHWND();

	protected:
		enum {
			KEY_NUM = 256,
		};

	protected:
		IMemoryAllocator* m_Allocator;

		D_INPUT_DEVICE* m_pKeyDevice;
		HWND m_hWnd;

		// キーバッファ
		IZ_UINT8 m_CurState[KEY_NUM];
		IZ_UINT8 m_PrevState[KEY_NUM];

		// 更新に成功したかどうか
		IZ_BOOL m_bSucceedUpdate;
	};

	// inline *****************************

	/**
	* キーを押し続けているか
	*/
	IZ_BOOL CKeyboard::IsPushKey(E_KEYBOARD_BUTTON key)
	{
		IZ_BOOL ret = IZ_FALSE;
		if (m_bSucceedUpdate) {
			ret = (m_CurState[key] > 0);
		}
		return ret;
	}

	/**
	* キーを一度だけ押したかどうか
	*/
	IZ_BOOL CKeyboard::IsPushOneShotKey(E_KEYBOARD_BUTTON key)
	{
		IZ_BOOL ret = IZ_FALSE;
		if (m_bSucceedUpdate) {
			ret = ((m_CurState[key] > 0) && (m_PrevState[key] == 0));
		}
		return ret;
	}

	D_INPUT_DEVICE* CKeyboard::GetRawInterface()
	{
		return m_pKeyDevice;
	}

	HWND CKeyboard::GetHWND()
	{
		return m_hWnd;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IO_KEYBOARD_H__)
