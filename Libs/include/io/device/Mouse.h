#if !defined(__IZANAGI_IO_MOUSE_H__)
#define __IZANAGI_IO_MOUSE_H__

#include "izStd.h"
#include "InputDeviceDefs.h"

namespace izanagi {
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
		IZ_BOOL Init(SInputDeviceInitParam* pInitParam);

		// 更新
		IZ_BOOL Update();

	protected:
		inline IZ_BOOL IsClick(IZ_UINT idx) const;

	public:
		inline IZ_BOOL IsClickL() const;	// 右
		inline IZ_BOOL IsClickR() const;	// 左
		inline IZ_BOOL IsClickM() const;	// 中

		inline IZ_DWORD GetX() const;
		inline IZ_DWORD GetY() const;
		inline IZ_DWORD GetZ() const;

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
		IZ_BOOL m_bSucceedUpdate;
	};

	// inline *****************************

	// マウスクリック
	IZ_BOOL CMouse::IsClick(IZ_UINT idx) const
	{
		IZ_ASSERT(idx < COUNTOF(m_MouseState.rgbButtons));

		IZ_BOOL ret = m_bSucceedUpdate;
		if (ret) {
			ret = ((m_MouseState.rgbButtons[idx] & 0x80) > 0);
		}
		return ret;
	}

	/**
	* マウス右クリック
	*/
	IZ_BOOL CMouse::IsClickL() const
	{
		return IsClick(0);
	}

	/**
	* マウス左クリック
	*/
	IZ_BOOL CMouse::IsClickR() const
	{
		return IsClick(1);
	}

	/**
	* マウス中クリック
	*/
	IZ_BOOL CMouse::IsClickM() const
	{
		return IsClick(2);
	}

	/**
	* マウス移動Ｘ
	*/
	IZ_DWORD CMouse::GetX() const
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
	IZ_DWORD CMouse::GetY() const
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
	IZ_DWORD CMouse::GetZ() const
	{
		IZ_DWORD ret = 0;
		if (m_bSucceedUpdate) {
			ret = (IZ_DWORD)m_MouseState.lZ;
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
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IO_MOUSE_H__)
