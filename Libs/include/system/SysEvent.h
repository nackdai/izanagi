#if !defined(__IZANAGI_SYSTEM_SYS_EVENT_H__)
#define __IZANAGI_SYSTEM_SYS_EVENT_H__

#include "izDefs.h"
#include "SysThreadDefs.h"

namespace izanagi {
	/**
	 */
	class CEvent {
	public:
		CEvent();
		~CEvent();

		NO_COPIABLE(CEvent);

	public:
		/** 初期化.
		 */
		IZ_BOOL Open();

		/** 終了.
		 */
		void Close();

		/** シグナル状態にする.
		 *
		 * スレッドの動作を開始する。
		 */
		void Set();

		/** シグナル状態になるのを待つ.
		 *
		 * スレッドの動作が開始されるのを待つ。
		 */
		IZ_BOOL Wait();

		/** 非シグナル状態にする.
		 *
		 * スレッドの動作を一時停止させる。
		 */
		void Reset();

	private:
		EventHandle m_Handle;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_EVENT_H__)
