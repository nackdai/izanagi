#if !defined(__IZANAGI_SYSTEM_PAD_XINPUT_H__)
#define __IZANAGI_SYSTEM_PAD_XINPUT_H__

#include "Pad.h"
#include <XInput.h>

namespace izanagi
{
namespace sys
{
    // XInput
    class CPadXInput : public CPad
    {
        friend class CPad;

	private:
		// インスタンス作成
		static CPad* CreatePad(
            IMemoryAllocator* pAllocator,
            IZ_FLOAT analogStickDeadZone);

	private:
		CPadXInput();
		virtual ~CPadXInput();

	private:
        // 初期化
		IZ_BOOL Init(IZ_FLOAT analogStickDeadZone);

		// 更新
		virtual IZ_BOOL Update();

        virtual IZ_BOOL IsEnableVibrate() const
        {
            return IZ_TRUE;
        }

        virtual const void* GetRawState() const
        {
            return &m_RawState;
        }

	protected:
        XINPUT_STATE m_RawState;
	};
}   // namespace sys
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_PAD_XINPUT_H__)
