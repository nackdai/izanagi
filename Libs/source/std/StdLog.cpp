#include "std/StdLog.h"

using namespace izanagi;

namespace {
	inline IZ_BOOL _OutputString(IZ_PCSTR pszFormat, ...)
	{
		static IZ_CHAR pszBuf[1024];

		va_list	argp;
		va_start(argp, pszFormat);
		vsprintf_s(pszBuf, sizeof(pszBuf), pszFormat, argp);
		va_end(argp);
		::OutputDebugString(pszBuf);

		IZ_BOOL ret = (pszBuf[strlen(pszBuf)] != '\n');
		return ret;
	}
}	// namespace

CLog CLog::s_cInstance;

#define OUT_STRING(type, str)\
	do {\
		if (m_EnableLogType & type) {\
			_OutputString(str);\
			va_list	argp;\
			va_start(argp, format);\
			if (_OutputString(format, argp)) { _OutputString("\n"); }\
			va_end(argp);\
		}\
	} while(IZ_FALSE)

void CLog::LogDebug(IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_DEBUG, "Debug   :"); 
}

void CLog::LogError(IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_ERROR, "Error   :"); 
}

void CLog::LogInfo(IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_INFO, "Info    :"); 
}

void CLog::LogVerbose(IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_VERBOSE, "Verbose :"); 
}

void CLog::LogWarn(IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_WARN, "Warn    :"); 
}

#undef OUT_STRING
#define OUT_STRING(type, str, tag)\
	do {\
		if (m_EnableLogType & type) {\
			_OutputString(str, tag);\
			va_list	argp;\
			va_start(argp, format);\
			if (_OutputString(format, argp)) { _OutputString("\n"); }\
			va_end(argp);\
		}\
	} while(IZ_FALSE)

void CLog::LogDebug(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_DEBUG, "Debug [%s] : ", tag);
}

void CLog::LogError(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_ERROR, "Error [%s] : ", tag);
}

void CLog::LogInfo(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_INFO, "Info [%s] : ", tag);
}

void CLog::LogVerbose(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_VERBOSE, "Verbose [%s] : ", tag);
}

void CLog::LogWarn(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
	OUT_STRING(LOG_TYPE_WARN, "Warn [%s] : ", tag);
}
