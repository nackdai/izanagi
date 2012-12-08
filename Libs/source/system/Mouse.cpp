#include "system/Mouse.h"
#include "system\Mouse_DInput.h"

namespace izanagi
{
    /**
    * インスタンス作成
    */
    CMouse* CMouse::CreateMouse(
        IMemoryAllocator* allocator,
        SInputDeviceInitParam* initParam)
    {
        CMouse* ret = CMouseDInput::CreateMouse(allocator, initParam);
        return ret;
    }

    // コンストラクタ
    CMouse::CMouse()
    {
	    m_Allocator = IZ_NULL;
	    m_bSucceedUpdate = IZ_FALSE;
    }

    // デストラクタ
    CMouse::~CMouse()
    {
    }
}   // namespace izanagi
