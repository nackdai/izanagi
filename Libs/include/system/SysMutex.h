#if !defined(__IZANAGI_SYSTEM_SYS_MUTEX_H__)
#define __IZANAGI_SYSTEM_SYS_MUTEX_H__

#include "izDefs.h"
#include "SysThreadDefs.h"

namespace izanagi
{
namespace sys
{
    /**
     */
    class CMutex {
        friend class CCondVar;

    public:
        CMutex();
        ~CMutex();

        NO_COPIABLE(CMutex);

    public:
        /** 初期化.
         */
        IZ_BOOL Open();

        /** 終了.
         */
        void Close();

        /** 同期開始.
         */
        void Lock();

        /** 同期終了.
         */
        void Unlock();

        /** 呼び出しスレッドID取得.
         */
        const ThreadId& GetOwnerThreadId() const
        {
            return m_OwnerThreadId;
        }

    protected:
        MutexHandle m_Handle;

        ThreadId m_OwnerThreadId;
    };

    class CGuarder
    {
    public:
        CGuarder(CMutex& mutex) : m_Mutex(mutex)
        {
            m_Mutex.Lock();
        }
        ~CGuarder()
        {
            m_Mutex.Unlock();
        }

        NO_COPIABLE(CGuarder);

    private:
        CMutex& m_Mutex;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_MUTEX_H__)
