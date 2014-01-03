#include "system/Pad.h"

namespace izanagi
{
namespace sys
{
    class CPadLinux : public CPad {
    public:
        virtual IZ_BOOL Update()
        {
            return IZ_TRUE;
        }
    };
}   // namespace sys
}   // namespce izanagi
