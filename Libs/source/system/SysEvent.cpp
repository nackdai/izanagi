#if defined(WIN32) || defined(WIN64)

#include "system/SysEvent.h"
#include "system/SysThread.h"

namespace izanagi
{
namespace sys
{
    CEvent::CEvent()
    {
		m_isSignal = IZ_FALSE;
    }

    CEvent::~CEvent()
    {
    }

    // シグナル状態にする.
	void CEvent::Set(std::function<void(void)> func/*= nullptr*/)
    {
		std::unique_lock<std::mutex> lock(m_mutex);

		if (func) {
			func();
		}
		m_isSignal = IZ_TRUE;

		m_condVar.notify_one();
    }

    // シグナル状態になるのを待つ.
	void CEvent::Wait(std::function<IZ_BOOL(void)> func/*= nullptr*/)
    {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condVar.wait(
			lock,
			[this, func] {
			if (func) {
				return func();
			}
			return m_isSignal;
		});
    }

    // NOTE
    // 自動リセットではWait終了時に自動で非シグナル状態になるので
    // 明示的に非シグナル状態にする必要がない
    // 今回は手動リセット

    // 非シグナル状態にする.
	void CEvent::Reset(std::function<void(void)> func/*= nullptr*/)
    {
		std::unique_lock<std::mutex> lock(m_mutex);

		if (func) {
			func();
		}

		m_isSignal = IZ_FALSE;
    }
}   // namespace sys
}   // namespace iznaagi
#endif  // #if defined(WIN32) || defined(WIN64)
