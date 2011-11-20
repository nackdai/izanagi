#if !defined(__IZANAGI_SYSTEM_SYS_MONITOR_H__)
#define __IZANAGI_SYSTEM_SYS_MONITOR_H__

#include "izDefs.h"
#include "SysThreadDefs.h"

namespace izanagi {
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
		void BeginSyncronized();

		// syncronized終了.
		IZ_BOOL EndSyncronized();

	public:
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
		class CMonitorImpl;

		CMonitorImpl* m_Impl;
		IZ_UINT8 m_Buf[1];	// TODO
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_MONITOR_H__)
