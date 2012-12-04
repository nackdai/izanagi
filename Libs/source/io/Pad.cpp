#include "io/device/Pad.h"
#include "io/device/Pad_DInput.h"

namespace izanagi
{
    /**
    * インスタンス作成
    */
    CPad* CPad::CreatePad(
        IMemoryAllocator* allocator,
        void* initParam,
        IZ_FLOAT analogStickDeadZone)
    {
        analogStickDeadZone = IZ_MIN(IZ_MAX(0.0f, analogStickDeadZone), 1.0f);

        CPad* ret = CPadDInput::CreatePad(
            allocator,
            initParam,
            analogStickDeadZone);

        return ret;
    }

    // コンストラクタ
    CPad::CPad()
    {
	    m_Allocator = IZ_NULL;

	    FILL_ZERO(&m_CurState, sizeof(m_CurState));
	    FILL_ZERO(&m_PrevState, sizeof(m_PrevState));

	    m_bSucceedUpdate = IZ_FALSE;
    }

    // デストラクタ
    CPad::~CPad()
    {
    }
}   // using namespace izanagi
