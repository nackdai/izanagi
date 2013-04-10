#include "threadmodel/ParallelFor.h"

namespace izanagi
{
namespace threadmodel
{
    CParallelFor::CParallelFor()
    {
        m_Num = 0;
        m_RegisteredNum = 0;
        m_Jobs = IZ_NULL;
    }

    CParallelFor::~CParallelFor()
    {
        Join();

        for (IZ_UINT i = 0; i < m_Num; i++) {
            delete &m_Jobs[i];
        }

        FREE(m_Allocator, m_Jobs);
    }

    IZ_BOOL CParallelFor::AllocJobs(
        IMemoryAllocator* allocator,
        IZ_UINT num)
    {
        if (m_Jobs == IZ_NULL) {
            size_t size = num * sizeof(CParallelForJob*);
            size += num * sizeof(CParallelForJob);

            IZ_UINT8* buf = reinterpret_cast<IZ_UINT8*>(ALLOC(allocator, size));

            m_Jobs = reinterpret_cast<CParallelForJob**>(buf);
            buf += num * sizeof(CParallelForJob*);

            for (IZ_UINT i = 0; i < num; i++) {
                m_Jobs[i] = new(buf) CParallelForJob;
                buf += sizeof(CParallelForJob);

                m_Jobs[i]->SetIdx(i);
            }

            m_Allocator = allocator;
            m_Num = num;

            return IZ_TRUE;
        }

        return IZ_FALSE;
    }

    IZ_BOOL CParallelFor::Init(
        IMemoryAllocator* allocator, 
        IZ_UINT num,
        Func func)
    {
        IZ_BOOL ret = AllocJobs(allocator, num);

        if (ret) {
            for (IZ_UINT i = 0; i < num; i++) {
                m_Jobs[i]->SetFunc(func);
            }
        }

        return ret;
    }

    IZ_BOOL CParallelFor::Init(
        IMemoryAllocator* allocator, 
        IZ_UINT num,
        Action* action)
    {
        IZ_BOOL ret = AllocJobs(allocator, num);

        if (ret) {
            for (IZ_UINT i = 0; i < num; i++) {
                m_Jobs[i]->SetDelegate(action);
            }
        }

        return ret;
    }
}   // namespace threadmodel
}   // namespace izanagi
