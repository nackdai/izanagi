#if !defined(__IZANAGI_SYSTEM_SYS_EVENT_H__)
#define __IZANAGI_SYSTEM_SYS_EVENT_H__

#include <mutex>
#include <condition_variable>

#include "izDefs.h"
#include "SysThreadDefs.h"

namespace izanagi
{
namespace sys
{
    /**
     */
    class CEvent {
    public:
		CEvent();
        ~CEvent();

        NO_COPIABLE(CEvent);

    public:
        /** シグナル状態にする.
         *
         * スレッドの動作を開始する。
         */
		void Set(std::function<void(void)> func = nullptr);

        /** シグナル状態になるのを待つ.
         *
         * スレッドの動作が開始されるのを待つ。
         */
		void Wait(std::function<IZ_BOOL(void)> func = nullptr);

        /** 非シグナル状態にする.
         *
         * スレッドの動作を一時停止させる。
         */
		void Reset(std::function<void(void)> func = nullptr);

    private:
		std::mutex m_mutex;
		std::condition_variable m_condVar;

		IZ_BOOL m_isSignal;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_EVENT_H__)
