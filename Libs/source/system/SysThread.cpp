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
		m_allocator = nullptr;
		m_thread = nullptr;
    }

    CThread::CThread(const ThreadName& name)
    {
		m_ThreadResult = 0;
		m_Name = name;
		m_allocator = nullptr;
		m_thread = nullptr;
    }

    CThread::~CThread()
    {
        Join();
    }

	void CThread::Init(
		const ThreadName& name,
		std::function<void(void*)> func,
		void* userData)
	{
		m_Name = name;
		Init(func, userData);
	}

	void CThread::Init(
		std::function<void(void*)> func,
		void* userData)
	{
		m_func = func;
		m_userData = userData;
	}

    // このスレッドの実行を開始.
	IZ_BOOL CThread::Start(IMemoryAllocator* allocator)
    {
		if (m_thread == nullptr) {
			m_allocator = allocator;
			IZ_ASSERT(m_allocator != nullptr);

			void* buf = ALLOC(m_allocator, sizeof(std::thread));
			VRETURN(buf != IZ_NULL);

			m_thread = new(buf) std::thread([this] { Run(); });
		}

		return IZ_TRUE;
    }

    // スレッド実行中かどうかを取得.
    IZ_BOOL CThread::IsRunning()
    {
		IZ_BOOL ret = (m_thread != nullptr);
		return ret;
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
		if (m_thread != nullptr) {
			m_thread->join();

			FREE(m_allocator, m_thread);
			m_thread = nullptr;
		}
    }

    // 名前取得.
    const ThreadName& CThread::GetName() const
    {
        return m_Name;
    }
}   // namespace sys
}   // namespace izanagi
