#if !defined(__IZANAGI_SYSTEM_SYS_MONITOR_H__)
#define __IZANAGI_SYSTEM_SYS_MONITOR_H__

#include "izDefs.h"
#include "SysMutex.h"
#include "SysCond.h"

namespace izanagi {
	class IMemoryAllocator;

	/**
	 */
	class CMonitor {
		friend class CSyncronized;

	public:
		CMonitor();
		~CMonitor();

		NO_COPIABLE(CMonitor);

	private:
		// syncronized開始.
		void Lock();

		// syncronized終了.
		void Unlock();

	public:
		/** 初期化.
		 */
		IZ_BOOL Init();

		/** 終了.
		 */
		void Destroy();

		/** 待機.
		 */
		void Wait();

		/** 待機中のスレッドを１つ起こす.
		 */
		void Notify();

		/** 待機中のスレッドを全て起こす.
		 */
		void NotifyAll();

	private:
		CMutex m_LockMutex;
		CCondVar m_WaitCond;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_MONITOR_H__)
