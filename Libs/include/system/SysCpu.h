#if !defined(__IZANAGI_SYSTEM_SYS_CPU_H__)
#define __IZANAGI_SYSTEM_SYS_CPU_H__

#include "izDefs.h"

namespace izanagi
{
namespace sys
{
    /**
    */
    class CSysCpu {
    private:
        CSysCpu();
        ~CSysCpu();

        NO_COPIABLE(CSysCpu);

    public:
        // Return CPU num.
        static IZ_UINT GetCpuNum();
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_CPU_H__)
