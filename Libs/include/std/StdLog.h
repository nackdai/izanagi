#if !defined(__URANUS_STD_STD_LOG_H__)
#define __URANUS_STD_STD_LOG_H__

#include "unDefs.h"

namespace uranus {
	/**
	* ÉçÉO
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
		void SetEnableFlag(UN_UINT flag) { m_EnableLogType = flag; }
		UN_BOOL IsLoggable(LOG_TYPE type) { return (m_EnableLogType & type); }

		void LogDebug(UN_PCSTR format, ...);
		void LogError(UN_PCSTR format, ...);
		void LogInfo(UN_PCSTR format, ...);
		void LogVerbose(UN_PCSTR format, ...);
		void LogWarn(UN_PCSTR format, ...);

		void LogDebug(UN_PCSTR tag, UN_PCSTR format, ...);
		void LogError(UN_PCSTR tag, UN_PCSTR format, ...);
		void LogInfo(UN_PCSTR tag, UN_PCSTR format, ...);
		void LogVerbose(UN_PCSTR tag, UN_PCSTR format, ...);
		void LogWarn(UN_PCSTR tag, UN_PCSTR format, ...);

	private:
		UN_UINT m_EnableLogType;
	};
}	// namespace uranus

#define UN_LOGD(t, f, ...)		uranus::CLog::GetInstance().LogDebug(t, f, __VA_ARGS__)
#define UN_LOGE(t, f, ...)		uranus::CLog::GetInstance().LogError(t, f, __VA_ARGS__)
#define UN_LOGI(t, f, ...)		uranus::CLog::GetInstance().LogInfo(t, f, __VA_ARGS__)
#define UN_LOGV(t, f, ...)		uranus::CLog::GetInstance().LogVerbose(t, f, __VA_ARGS__)
#define UN_LOGW(t, f, ...)		uranus::CLog::GetInstance().LogWarn(t, f, __VA_ARGS__)

#define UN_LOG_ASSERT(b, t, f, ...)			if (!b) { UN_LOGE(t, f, __VA_ARGS__); UN_ASSERT(UN_FALSE); }
#define UN_LOG_VRETURN(b, t, f, ...)		if (!b) { UN_LOGE(t, f, __VA_ARGS__); VRETURN(UN_FALSE); }
#define UN_LOG_VGOTO(b, label, t, f, ...)	if (!b) { UN_LOGE(t, f, __VA_ARGS__); VGOTO(UN_FALSE, label); }

#endif	// #if !defined(__URANUS_STD_STD_LOG_H__)
