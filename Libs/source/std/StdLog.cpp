#include "std/StdLog.h"

using namespace izanagi;

namespace {
    IZ_UINT s_EnableLogType = 0xffffffff;

    inline IZ_BOOL _OutputString(IZ_PCSTR pszFormat, ...)
    {
        static IZ_CHAR pszBuf[1024];

        va_list argp;
        va_start(argp, pszFormat);
        vsprintf_s(pszBuf, sizeof(pszBuf), pszFormat, argp);
        va_end(argp);
        ::OutputDebugString(pszBuf);

        IZ_BOOL ret = (pszBuf[strlen(pszBuf)] != '\n');
        return ret;
    }
}   // namespace

void CLog::SetEnableFlag(IZ_UINT flag)
{
    s_EnableLogType = flag;
}

IZ_BOOL CLog::IsLoggable(LOG_TYPE type)
{
    return (s_EnableLogType & type);
}

#define OUT_STRING(type, str)\
    do {\
        if (s_EnableLogType & type) {\
            _OutputString(str);\
            va_list argp;\
            va_start(argp, format);\
            if (_OutputString(format, argp)) { _OutputString("\n"); }\
            va_end(argp);\
        }\
    } while (0)

void CLog::Debug(IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_DEBUG, "Debug   :"); 
}

void CLog::Error(IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_ERROR, "Error   :"); 
}

void CLog::Info(IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_INFO, "Info    :"); 
}

void CLog::Verbose(IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_VERBOSE, "Verbose :"); 
}

void CLog::Warn(IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_WARN, "Warn    :"); 
}

#undef OUT_STRING
#define OUT_STRING(type, str, tag)\
    do {\
        if (s_EnableLogType & type) {\
            _OutputString(str, tag);\
            va_list argp;\
            va_start(argp, format);\
            if (_OutputString(format, argp)) { _OutputString("\n"); }\
            va_end(argp);\
        }\
    } while(IZ_FALSE)

void CLog::Debug(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_DEBUG, "Debug [%s] : ", tag);
}

void CLog::Error(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_ERROR, "Error [%s] : ", tag);
}

void CLog::Info(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_INFO, "Info [%s] : ", tag);
}

void CLog::Verbose(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_VERBOSE, "Verbose [%s] : ", tag);
}

void CLog::Warn(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    OUT_STRING(LOG_TYPE_WARN, "Warn [%s] : ", tag);
}
