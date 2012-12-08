#include "system/Keyboard.h"
#include "system/Keyboard_DInput.h"

namespace izanagi
{
namespace sys
{
    /**
    * インスタンス作成
    */
    CKeyboard* CKeyboard::CreateKeyboard(
        IMemoryAllocator* allocator,
        SInputDeviceInitParam* initParam)
    {
        CKeyboard* ret = CKeyboardDInput::CreateKeyboard(allocator, initParam);
        return ret;
    }

    // コンストラクタ
    CKeyboard::CKeyboard()
    {
	    m_Allocator = IZ_NULL;

	    FILL_ZERO(m_CurState, sizeof(m_CurState));
	    FILL_ZERO(m_LastState, sizeof(m_LastState));

	    m_bSucceedUpdate = IZ_FALSE;
    }

    // デストラクタ
    CKeyboard::~CKeyboard()
    {
    }
}   // namespace sys
}   // namespace izanagi
