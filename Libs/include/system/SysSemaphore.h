#if !defined(__IZANAGI_SYSTEM_SYS_SEMAPHORE_H__)
#define __IZANAGI_SYSTEM_SYS_SEMAPHORE_H__

#include <mutex>
#include <condition_variable>

#include "izDefs.h"
#include "SysThreadDefs.h"

namespace izanagi
{
namespace sys
{
    /** セマフォ.
     */
    class CSemaphore {
    public:
        CSemaphore();
        ~CSemaphore();

        NO_COPIABLE(CSemaphore);

    public:
        /** 待機.
         */
        void wait();

        /** セマフォカウントを解放.
         */
        void notify();

    private:
		std::mutex m_mutex;
		std::condition_variable m_condVar;

		IZ_UINT16 m_count;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_SEMAPHORE_H__)
