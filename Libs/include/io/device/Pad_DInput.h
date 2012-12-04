#if !defined(__IZANAGI_IO_PAD_DINPUT_H__)
#define __IZANAGI_IO_PAD_DINPUT_H__

#include "izStd.h"
#include "Pad.h"

namespace izanagi
{
    // DirectInput
	class CPadDInput : public CPad
    {
        friend class CPad;

	private:
		// インスタンス作成
		static CPad* CreatePad(
            IMemoryAllocator* pAllocator,
            void* initParam,
            IZ_FLOAT analogStickDeadZone);

		// デバイス列挙コールバック
		static BOOL CALLBACK EnumGameCtrlCallback(
			const DIDEVICEINSTANCE* pdidInstance,
			void* pContext);

		// パッド設定コールバック
		static BOOL CALLBACK EnumPadAxisCallback(
            LPCDIDEVICEOBJECTINSTANCE lpddoi, 
            LPVOID pvRef);

	private:
		CPadDInput();
		virtual ~CPadDInput();

	private:
        // 初期化
		IZ_BOOL Init(
            void* initParam,
            IZ_FLOAT analogStickDeadZone);

		// 更新
		virtual IZ_BOOL Update();

        virtual const void* GetRawState() const
        {
            return &m_RawState;
        }

	protected:
		D_INPUT_DEVICE* m_pPadDevice;
		HWND m_hWnd;

        D_INPUT* m_InputTmp;
	    D_INPUT_DEVICE* m_DevTmp;

        DIJOYSTATE m_RawState;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_IO_PAD_DINPUT_H__)
