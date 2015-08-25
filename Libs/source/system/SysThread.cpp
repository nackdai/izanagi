#include "system/SysThread.h"

namespace izanagi
{
namespace sys
{
    // 実行中のスレッドを、指定されたミリ秒数の間、スリープ.
    void CThread::Sleep(IZ_UINT millisec)
    {
		std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
    }

    // 現在実行中のスレッドを一時的に休止させ、ほかのスレッドが実行できるようにする.
    void CThread::YieldThread()
    {
		std::this_thread::yield();
    }

    CThread::CThread()
    {
		m_ThreadResult = 0;
    }

    CThread::CThread(const ThreadName& name)
        : CThread()
    {
		m_Name = name;
    }

    CThread::~CThread()
    {
        Join();
    }

    // このスレッドの実行を開始.
	IZ_BOOL CThread::Start(
        std::function<void(void*)> func,
        void* userData)
    {
        m_isRunning = IZ_TRUE;
        m_func = func;
        m_userData = userData;
        m_thread = std::thread([this] { Run(); });

		return IZ_TRUE;
    }

    // このスレッドの実行を開始.
    IZ_BOOL CThread::Start()
    {
        m_isRunning = IZ_TRUE;
        m_thread = std::thread([this] { Run(); });

        return IZ_TRUE;
    }

    // スレッド実行中かどうかを取得.
    IZ_BOOL CThread::IsRunning()
    {
        return m_isRunning;
    }

	void CThread::Run()
	{
		if (m_func) {
			m_func(m_userData);
		}
	}

    // このスレッドが終了するのを待機.
    void CThread::Join()
    {
        if (m_isRunning) {
            m_thread.join();
            m_isRunning = IZ_FALSE;
            m_userData = nullptr;
        }
    }

    // 名前取得.
    const ThreadName& CThread::GetName() const
    {
        return m_Name;
    }
}   // namespace sys
}   // namespace izanagi
