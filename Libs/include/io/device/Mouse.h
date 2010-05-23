#if !defined(__URANUS_IO_MOUSE_H__)
#define __URANUS_IO_MOUSE_H__

#include "../../unStd.h"
#include "InputDeviceDefs.h"

namespace uranus {
	/**
	* キーボード
	*/
	class CMouse : public CObject {
	public:
		// インスタンス作成
		static CMouse* CreateMouse(IMemoryAllocator* pAllocator);

	protected:
		CMouse();
		~CMouse();

		CMouse(const CMouse& rhs);
		const CMouse& operator=(const CMouse& rhs);

	protected:
		// 解放
		void InternalRelease();

	public:
		// 初期化
		UN_BOOL Init(SInputDeviceInitParam* pInitParam);

		// 更新
		UN_BOOL Update();

	protected:
		inline UN_BOOL IsClick(UN_UINT idx) const;

	public:
		inline UN_BOOL IsClickL() const;	// 右
		inline UN_BOOL IsClickR() const;	// 左
		inline UN_BOOL IsClickM() const;	// 中

		inline UN_DWORD GetX() const;
		inline UN_DWORD GetY() const;
		inline UN_DWORD GetZ() const;

		inline const DIMOUSESTATE2& GetMouseState() const;

	public:
		inline D_INPUT_DEVICE* GetRawInterface();
		inline HWND GetHWND();

	protected:
		IMemoryAllocator* m_pAllocator;

		D_INPUT_DEVICE* m_pMouseDevice;
		HWND m_hWnd;

		DIMOUSESTATE2 m_MouseState;

		// 更新に成功したかどうか
		UN_BOOL m_bSucceedUpdate;
	};

	// inline *****************************

	// マウスクリック
	UN_BOOL CMouse::IsClick(UN_UINT idx) const
	{
		UN_ASSERT(idx < COUNTOF(m_MouseState.rgbButtons));

		UN_BOOL ret = m_bSucceedUpdate;
		if (ret) {
			ret = ((m_MouseState.rgbButtons[idx] & 0x80) > 0);
		}
		return ret;
	}

	/**
	* マウス右クリック
	*/
	UN_BOOL CMouse::IsClickL() const
	{
		return IsClick(0);
	}

	/**
	* マウス左クリック
	*/
	UN_BOOL CMouse::IsClickR() const
	{
		return IsClick(1);
	}

	/**
	* マウス中クリック
	*/
	UN_BOOL CMouse::IsClickM() const
	{
		return IsClick(2);
	}

	/**
	* マウス移動Ｘ
	*/
	UN_DWORD CMouse::GetX() const
	{
		UN_DWORD ret = 0;
		if (m_bSucceedUpdate) {
			ret = (UN_DWORD)m_MouseState.lX;
		}
		return ret;
	}

	/**
	* マウス移動Ｙ
	*/
	UN_DWORD CMouse::GetY() const
	{
		UN_DWORD ret = 0;
		if (m_bSucceedUpdate) {
			ret = (UN_DWORD)m_MouseState.lY;
		}
		return ret;
	}

	/**
	* マウスホイール
	*/
	UN_DWORD CMouse::GetZ() const
	{
		UN_DWORD ret = 0;
		if (m_bSucceedUpdate) {
			ret = (UN_DWORD)m_MouseState.lZ;
		}
		return ret;
	}

	D_INPUT_DEVICE* CMouse::GetRawInterface()
	{
		return m_pMouseDevice;
	}

	HWND CMouse::GetHWND()
	{
		return m_hWnd;
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_IO_MOUSE_H__)
