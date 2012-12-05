#include "system/Pad.h"
#include "system/Pad_DInput.h"
#include "system/Pad_XInput.h"

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
        CPad* ret = IZ_NULL;
        analogStickDeadZone = IZ_MIN(IZ_MAX(0.0f, analogStickDeadZone), 1.0f);

        if (initParam != IZ_NULL)
        {
            ret = CPadDInput::CreatePad(
                allocator,
                initParam,
                analogStickDeadZone);
        }
        else
        {
            ret = CPadXInput::CreatePad(
                allocator,
                analogStickDeadZone);
        }

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
