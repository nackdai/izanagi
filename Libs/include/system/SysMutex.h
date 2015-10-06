#if !defined(__IZANAGI_SYSTEM_SYS_MUTEX_H__)
#define __IZANAGI_SYSTEM_SYS_MUTEX_H__

#if 0
#include "izDefs.h"
#include "SysThreadDefs.h"
#else
#include <mutex>
#include "izDefs.h"
#endif

namespace izanagi
{
namespace sys
{
#if 0
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
#else
    class CMutex {
    public:
        CMutex() {}
        ~CMutex()
        {
            IZ_ASSERT(m_refCnt == 0);
        }

        NO_COPIABLE(CMutex);

    public:
        void lock()
        {
            if (m_refCnt == 0) {
                m_mutex.lock();
            }
            m_refCnt++;
        }

        void unlock()
        {
            if (m_refCnt == 1) {
                m_mutex.unlock();
            }
            m_refCnt--;
            IZ_ASSERT(m_refCnt >= 0);
        }

        void safeExec(std::function<void()> func)
        {
            lock();
            func();
            unlock();
        }

    private:
        IZ_INT m_refCnt{ 0 };
        std::mutex m_mutex;
    };
#endif
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_MUTEX_H__)
