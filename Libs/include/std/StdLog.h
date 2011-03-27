#if !defined(__IZANAGI_STD_STD_LOG_H__)
#define __IZANAGI_STD_STD_LOG_H__

#include "izDefs.h"

namespace izanagi {
	/**
	* ログ
	*/
	class CLog {
	private:
		static CLog s_cInstance;

	public:
		static CLog& GetInstance() { return s_cInstance; }

		enum LOG_TYPE {
			LOG_TYPE_DEBUG   = 1,
			LOG_TYPE_ERROR   = 1 << 1,
			LOG_TYPE_INFO    = 1 << 2,
			LOG_TYPE_VERBOSE = 1 << 3,
			LOG_TYPE_WARN    = 1 << 4,
		};

	private:
		CLog() { m_EnableLogType = 0xffffffff; }
		~CLog() {}

		NO_COPIABLE(CLog);

	public:
		void SetEnableFlag(IZ_UINT flag) { m_EnableLogType = flag; }
		IZ_BOOL IsLoggable(LOG_TYPE type) { return (m_EnableLogType & type); }

		void LogDebug(IZ_PCSTR format, ...);
		void LogError(IZ_PCSTR format, ...);
		void LogInfo(IZ_PCSTR format, ...);
		void LogVerbose(IZ_PCSTR format, ...);
		void LogWarn(IZ_PCSTR format, ...);

		void LogDebug(IZ_PCSTR tag, IZ_PCSTR format, ...);
		void LogError(IZ_PCSTR tag, IZ_PCSTR format, ...);
		void LogInfo(IZ_PCSTR tag, IZ_PCSTR format, ...);
		void LogVerbose(IZ_PCSTR tag, IZ_PCSTR format, ...);
		void LogWarn(IZ_PCSTR tag, IZ_PCSTR format, ...);

	private:
		IZ_UINT m_EnableLogType;
	};
}	// namespace izanagi

#define IZ_LOGD(t, f, ...)		izanagi::CLog::GetInstance().LogDebug(t, f, __VA_ARGS__)
#define IZ_LOGE(t, f, ...)		izanagi::CLog::GetInstance().LogError(t, f, __VA_ARGS__)
#define IZ_LOGI(t, f, ...)		izanagi::CLog::GetInstance().LogInfo(t, f, __VA_ARGS__)
#define IZ_LOGV(t, f, ...)		izanagi::CLog::GetInstance().LogVerbose(t, f, __VA_ARGS__)
#define IZ_LOGW(t, f, ...)		izanagi::CLog::GetInstance().LogWarn(t, f, __VA_ARGS__)

#define IZ_LOG_ASSERT(b, t, f, ...)			if (!b) { IZ_LOGE(t, f, __VA_ARGS__); IZ_ASSERT(IZ_FALSE); }
#define IZ_LOG_VRETURN(b, t, f, ...)		if (!b) { IZ_LOGE(t, f, __VA_ARGS__); VRETURN(IZ_FALSE); }
#define IZ_LOG_VGOTO(b, label, t, f, ...)	if (!b) { IZ_LOGE(t, f, __VA_ARGS__); VGOTO(IZ_FALSE, label); }

#endif	// #if !defined(__IZANAGI_STD_STD_LOG_H__)
