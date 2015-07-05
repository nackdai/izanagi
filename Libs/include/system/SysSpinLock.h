#if !defined(__IZANAGI_SYSTEM_SYS_SPINLOCK_H__)
#define __IZANAGI_SYSTEM_SYS_SPINLOCK_H__

#include <atomic>

#include "izStd.h"

namespace izanagi
{
namespace sys
{
	class CSpinLock {
	public:
		CSpinLock()
			: m_lock(ATOMIC_FLAG_INIT)
		{}

		~CSpinLock() {}

		NO_COPIABLE(CSpinLock);

	public:
		void lock()
		{
			while (m_lock.test_and_set(std::memory_order_acquire))
			{
			}
		}

		void unlock()
		{
			m_lock.clear(std::memory_order_release);
		}

	private:
		std::atomic_flag m_lock;
	};
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_SEMAPHORE_H__)
