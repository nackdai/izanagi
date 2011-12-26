#if !defined(__IZANAGI_SYSTEM_SYS_MUTEX_H__)
#define __IZANAGI_SYSTEM_SYS_MUTEX_H__

#include "izDefs.h"
#include "SysThreadDefs.h"

namespace izanagi {
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
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_MUTEX_H__)
