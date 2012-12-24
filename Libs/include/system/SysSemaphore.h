#if !defined(__IZANAGI_SYSTEM_SYS_SEMAPHORE_H__)
#define __IZANAGI_SYSTEM_SYS_SEMAPHORE_H__

#include "izDefs.h"
#include "SysThreadDefs.h"

namespace izanagi
{
namespace sys
{
    /** セマフォ.
     */
    class CSemaphore {
        friend class CCondVar;

    public:
        CSemaphore();
        ~CSemaphore();

        NO_COPIABLE(CSemaphore);

    public:
        /** 初期化.
         */
        IZ_BOOL Init(IZ_UINT16 initialCount);

        /** 終了.
         */
        void Close();

        /** 待機.
         */
        IZ_BOOL Wait(IZ_INT timeout = -1);

        /** セマフォカウントを指定された数だけ増やす.
         */
        void Release(IZ_UINT count = 1);

    private:
        SemaHandle m_Handle;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_SEMAPHORE_H__)
