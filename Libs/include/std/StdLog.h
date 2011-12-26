#if !defined(__IZANAGI_STD_STD_LOG_H__)
#define __IZANAGI_STD_STD_LOG_H__

#include "izDefs.h"

namespace izanagi {
	/**
	* ログ
	*/
	class CLog {
	public:
		enum LOG_TYPE {
			LOG_TYPE_DEBUG   = 1,
			LOG_TYPE_ERROR   = 1 << 1,
			LOG_TYPE_INFO    = 1 << 2,
			LOG_TYPE_VERBOSE = 1 << 3,
			LOG_TYPE_WARN    = 1 << 4,
		};

	private:
		CLog();
		~CLog();

		NO_COPIABLE(CLog);

	public:
		static void SetEnableFlag(IZ_UINT flag);
		static IZ_BOOL IsLoggable(LOG_TYPE type);

		static void LogDebug(IZ_PCSTR format, ...);
		static void LogError(IZ_PCSTR format, ...);
		static void LogInfo(IZ_PCSTR format, ...);
		static void LogVerbose(IZ_PCSTR format, ...);
		static void LogWarn(IZ_PCSTR format, ...);

		static void LogDebug(IZ_PCSTR tag, IZ_PCSTR format, ...);
		static void LogError(IZ_PCSTR tag, IZ_PCSTR format, ...);
		static void LogInfo(IZ_PCSTR tag, IZ_PCSTR format, ...);
		static void LogVerbose(IZ_PCSTR tag, IZ_PCSTR format, ...);
		static void LogWarn(IZ_PCSTR tag, IZ_PCSTR format, ...);
	};
}	// namespace izanagi

#define IZ_LOGD(t, f, ...)		izanagi::CLog::LogDebug(t, f, __VA_ARGS__)
#define IZ_LOGE(t, f, ...)		izanagi::CLog::LogError(t, f, __VA_ARGS__)
#define IZ_LOGI(t, f, ...)		izanagi::CLog::LogInfo(t, f, __VA_ARGS__)
#define IZ_LOGV(t, f, ...)		izanagi::CLog::LogVerbose(t, f, __VA_ARGS__)
#define IZ_LOGW(t, f, ...)		izanagi::CLog::LogWarn(t, f, __VA_ARGS__)

#define IZ_LOG_ASSERT(b, t, f, ...)			if (!b) { IZ_LOGE(t, f, __VA_ARGS__); IZ_ASSERT(IZ_FALSE); }
#define IZ_LOG_VRETURN(b, t, f, ...)		if (!b) { IZ_LOGE(t, f, __VA_ARGS__); VRETURN(IZ_FALSE); }
#define IZ_LOG_VGOTO(b, label, t, f, ...)	if (!b) { IZ_LOGE(t, f, __VA_ARGS__); VGOTO(IZ_FALSE, label); }

#endif	// #if !defined(__IZANAGI_STD_STD_LOG_H__)
