#if !defined(__IZANAGI_SYSTEM_MOUSE_DINPUT_H__)
#define __IZANAGI_SYSTEM_MOUSE_DINPUT_H__

#include "Mouse.h"

namespace izanagi
{
	class CMouseDInput : public CMouse {
	public:
		// インスタンス作成
		static CMouse* CreateMouse(
            IMemoryAllocator* allocator,
            SInputDeviceInitParam* initParam);

	protected:
		CMouseDInput();
		~CMouseDInput();

    protected:
        // 初期化
		IZ_BOOL Init(SInputDeviceInitParam* initParam);

	public:
		// 更新
		virtual IZ_BOOL Update();

		IZ_BOOL IsClick(IZ_UINT idx) const
        {
		    IZ_ASSERT(idx < COUNTOF(m_MouseState.rgbButtons));

		    IZ_BOOL ret = m_bSucceedUpdate;
		    if (ret) {
			    ret = ((m_MouseState.rgbButtons[idx] & 0x80) > 0);
		    }
		    return ret;
	    }

		virtual IZ_BOOL IsClickL() const;	// 右
		virtual IZ_BOOL IsClickR() const;	// 左
		virtual IZ_BOOL IsClickM() const;	// 中

		virtual IZ_DWORD GetX() const;
		virtual IZ_DWORD GetY() const;
		virtual IZ_DWORD GetZ() const;

        virtual const void* GetRawState() const
        {
            return &m_MouseState;
        }

	protected:
		IMemoryAllocator* m_Allocator;

		D_INPUT_DEVICE* m_pMouseDevice;
		HWND m_hWnd;

		DIMOUSESTATE2 m_MouseState;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_MOUSE_DINPUT_H__)
