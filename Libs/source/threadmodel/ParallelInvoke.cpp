#include "threadmodel/ParallelInvoke.h"

namespace izanagi
{
namespace threadmodel
{
    CParallelInvoke::CParallelInvoke()
    {
        m_Num = 0;
        m_RegisteredNum = 0;
        m_Jobs = IZ_NULL;
    }

    CParallelInvoke::~CParallelInvoke()
    {
        Join();

        for (IZ_UINT i = 0; i < m_Num; i++) {
            delete &m_Jobs[i];
        }

        FREE(m_Allocator, m_Jobs);
    }

    IZ_BOOL CParallelInvoke::Init(IMemoryAllocator* allocator, IZ_UINT num)
    {
        if (m_Jobs == IZ_NULL) {
            size_t size = num * sizeof(CParallelInvokeJob*);
            size += num * sizeof(CParallelInvokeJob);

            IZ_UINT8* buf = reinterpret_cast<IZ_UINT8*>(ALLOC(allocator, size));

            m_Jobs = reinterpret_cast<CParallelInvokeJob**>(buf);
            buf += num * sizeof(CParallelInvokeJob*);

            for (IZ_UINT i = 0; i < num; i++) {
                m_Jobs[i] = new(buf) CParallelInvokeJob;
                buf += sizeof(CParallelInvokeJob);
            }

            m_Allocator = allocator;
            m_Num = num;

            return IZ_TRUE;
        }

        return IZ_FALSE;
    }

    IZ_BOOL CParallelInvoke::Register(CParallelInvoke::Action* action)
    {
        if (!IsRunning()) {
            if (m_RegisteredNum < m_Num) {
                m_Jobs[m_RegisteredNum]->SetDelegate(action);
                m_Jobs[m_RegisteredNum]->SetIdx(m_RegisteredNum);

                m_RegisteredNum++;

                return IZ_TRUE;
            }
        }

        return IZ_FALSE;
    }

    IZ_BOOL CParallelInvoke::Register(CParallelInvoke::Func func)
    {
        if (!IsRunning()) {
            if (m_RegisteredNum < m_Num) {
                m_Jobs[m_RegisteredNum]->SetFunc(func);
                m_Jobs[m_RegisteredNum]->SetIdx(m_RegisteredNum);

                m_RegisteredNum++;

                return IZ_TRUE;
            }
        }

        return IZ_FALSE;
    }
}   // namespace threadmodel
}   // namespace izanagi
