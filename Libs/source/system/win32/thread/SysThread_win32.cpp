#if defined(WIN32) || defined(WIN64)

#include <process.h>
#include "system/SysThread.h"
#include "system/SysThreadUtil.h"

namespace izanagi
{
namespace sys
{
    // Get current thread id.
    ThreadId CThread::GetCurrentThreadId()
    {
        ThreadId ret = static_cast<ThreadId>(::GetCurrentThreadId());
        return ret;
    }

    // Return whethrer two threadid are equal.
    IZ_BOOL CThread::IsEqualThreadId(
         const ThreadId& t1,
         const ThreadId& t2)
    {
        return (t1 == t2);
    }

    // 実行中のスレッドを、指定されたミリ秒数の間、スリープ.
    void CThread::Sleep(IZ_UINT millisec)
    {
        ::Sleep(millisec);
    }

    // 現在実行中のスレッドを一時的に休止させ、ほかのスレッドが実行できるようにする.
    void CThread::YieldThread()
    {
        CThread::Sleep(0);
    }

    // スレッド実行関数
    static UINT __stdcall procThreadFunc(LPVOID param)
    {
        CThread* thread = (CThread*)param;

        thread->Run();

        return 0;
    }

    CThread::CThread()
    {
        Init(0, IZ_NULL, IZ_NULL, IZ_NULL);
    }

    CThread::CThread(const ThreadName& name)
    {
        Init(0, &name, IZ_NULL, IZ_NULL);
    }

    CThread::CThread(IRunnable* runnable, void* userData)
    {
        Init(0, IZ_NULL, runnable, userData);
    }

    CThread::CThread(const ThreadName& name, IRunnable* runnable, void* userData)
    {
        Init(0, &name, runnable, userData);
    }

    CThread::CThread(IZ_UINT cpu)
    {
        Init(cpu, IZ_NULL, IZ_NULL, IZ_NULL);
    }

    CThread::CThread(IZ_UINT cpu, const ThreadName& name)
    {
        Init(cpu, &name, IZ_NULL, IZ_NULL);
    }

    CThread::CThread(IZ_UINT cpu, IRunnable* runnable, void* userData)
    {
        Init(cpu, IZ_NULL, runnable, userData);
    }

    CThread::CThread(IZ_UINT cpu, const ThreadName& name, IRunnable* runnable, void* userData)
    {
        Init(cpu, &name, runnable, userData);
    }

    CThread::~CThread()
    {
        Join();
    }

    void CThread::Init(
        IZ_UINT cpu,
        const ThreadName* name,
        IRunnable* runnable,
        void* userData)
    {
        m_Handle = IZ_NULL;
        m_Id = 0;
        m_Cpu = cpu;

        m_UserData = userData;
        m_Runnable = runnable;

        if (name) {
            m_Name.SetString(name->GetString());
        }
    }

    void CThread::Init(
        const ThreadName* name,
        IRunnable* runnable,
        void* userData)
    {
        Init(0, name, runnable, userData);
    }

    void CThread::Init(
        IRunnable* runnable,
        void* userData)
    {
        Init(0, IZ_NULL, runnable, userData);
    }

    // このスレッドの実行を開始.
    IZ_BOOL CThread::Start()
    {
        if (m_Handle == IZ_NULL) {
            // スレッドハンドル作成
            m_Handle = (ThreadHandle)::_beginthreadex(
                            NULL,
                            256 * 1024,
                            procThreadFunc,
                            (void*)this,
                            CREATE_SUSPENDED,   // 待機
                            &m_Id);

            if (m_Handle == IZ_NULL) {
                IZ_ASSERT(IZ_FALSE);
                Join();
                return IZ_FALSE;
            }

            // 実行開始
            ::ResumeThread(m_Handle);
        }

        return IZ_TRUE;
    }

    // このスレッドが終了するのを待機.
    void CThread::Join()
    {
        if (m_Handle) {
            ::WaitForSingleObject(m_Handle, INFINITE);
            ::CloseHandle(m_Handle);

            m_Handle = IZ_NULL;
        }
    }

    // 処理実行.
    void CThread::Run()
    {
        IZ_ASSERT(m_Runnable != IZ_NULL);

        if (m_Runnable) {
            m_Runnable->Run(m_UserData);
        }
    }

    // スレッドID取得.
    const ThreadId& CThread::GetId() const
    {
        return m_Id;
    }

    // 名前取得.
    const ThreadName& CThread::GetName() const
    {
        return m_Name;
    }

    // 名前取得.
    const char* CThread::GetNameString() const
    {
        return m_Name.GetString();
    }

    // 名前設定.
    void CThread::SetName(ThreadName name)
    {
        m_Name.SetString(name.GetString());
    }
}   // namespace sys
}   // namespace izanagi
#endif  // #if defined(WIN32) || defined(WIN64)
