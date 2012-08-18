#if !defined(__IZANAGI_SYSTEM_SYS_COND_H__)
#define __IZANAGI_SYSTEM_SYS_COND_H__

#include "izDefs.h"
#include "SysThreadDefs.h"

namespace izanagi {
	class CMutex;

	/** 条件変数.
	 */
	class CCondVar {
	public:
		CCondVar();
		~CCondVar();

		NO_COPIABLE(CCondVar);

	public:
		/** 初期化.
		 */
		IZ_BOOL Init();

		/** 終了.
		 */
		void Destroy();

		/** 待機.
		 */
		void Wait(CMutex& externalMiutex);

		/** 待機中スレッドをひとつ起動.
		 */
		void Signal();

		/** 待機中スレッドを全て起動
		 */
		void Broadcast();

		// TODO
		// こればかりは・・・
#if defined(WIN32) || defined(WIN64)
	private:
		class CCondImpl;

		CCondImpl* m_Impl;
		IZ_UINT8 m_Buf[44];
#endif
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SYSTEM_SYS_COND_H__)
