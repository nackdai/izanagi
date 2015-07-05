#include "system/SysSemaphore.h"

namespace izanagi
{
namespace sys
{
    CSemaphore::CSemaphore()
    {
		m_count = 0;
    }

    CSemaphore::~CSemaphore()
    {
		IZ_ASSERT(m_count == 0);
    }

    // 待機.
    void CSemaphore::wait()
    {
		std::unique_lock<std::mutex> lock(m_mutex);

		m_condVar.wait(lock, [this]{ return (m_count > 0); });

		--m_count;
    }

    // セマフォカウントを解放.
	void CSemaphore::notify()
    {
        // もし、セマフォが非シグナル状態ならセマフォをシグナル状態にする
		std::unique_lock<std::mutex> lock(m_mutex);

		m_count++;

		m_condVar.notify_one();
    }
}   // namespace sys
}   // namespace izanagi
