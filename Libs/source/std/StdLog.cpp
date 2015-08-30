#include "std/StdLog.h"

using namespace izanagi;

namespace {
    IZ_UINT s_EnableLogType = 0xffffffff;

    inline IZ_BOOL _OutputString(IZ_PCSTR pszFormat, va_list& argp)
    {
        IZ_CHAR pszBuf[1024];

        IZ_VSPRINTF(pszBuf, sizeof(pszBuf), pszFormat, argp);
        IZ_PRINTF("%s", pszBuf);

        IZ_BOOL ret = (pszBuf[strlen(pszBuf)] != '\n');
        return ret;
    }
}   // namespace

void CLog::enableFlag(LogType type)
{
    s_EnableLogType |= type;
}

void CLog::disableFlag(LogType type)
{
    s_EnableLogType &= ~(type);
}

IZ_BOOL CLog::isLoggable(LogType type)
{
    return (s_EnableLogType & type);
}

#define IZ_DISP_LOG(type, str)\
    if (s_EnableLogType & type) {\
        IZ_PRINTF(str);\
        va_list argp;\
        va_start(argp, format);\
        if (_OutputString(format, argp)) { IZ_PRINTF("\n"); }\
        va_end(argp);\
    }

#define IZ_DISP_LOG_WITH_TAG(type, str, tag)\
    if (s_EnableLogType & type) {\
        IZ_PRINTF(str, tag);\
        va_list argp;\
        va_start(argp, format);\
        if (_OutputString(format, argp)) { IZ_PRINTF("\n"); }\
        va_end(argp);\
    }

void CLog::Debug(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    if (tag) {
        IZ_DISP_LOG_WITH_TAG(LogType::Debug, "Debug   [%s] : ", tag);
    }
    else {
        IZ_DISP_LOG(LogType::Debug, "Debug   : ");
    }
}

void CLog::Error(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    if (tag) {
        IZ_DISP_LOG_WITH_TAG(LogType::Error, "Error   [%s] : ", tag);
    }
    else {
        IZ_DISP_LOG(LogType::Error, "Error   : ");
    }
}

void CLog::Info(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    if (tag) {
        IZ_DISP_LOG_WITH_TAG(LogType::Info, "Info    [%s] : ", tag);
    }
    else {
        IZ_DISP_LOG(LogType::Info, "Info    : ");
    }
}

void CLog::Verbose(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    if (tag) {
        IZ_DISP_LOG_WITH_TAG(LogType::Verbose, "Verbose [%s] : ", tag);
    }
    else {
        IZ_DISP_LOG(LogType::Verbose, "Verbose : ");
    }
}

void CLog::Warn(IZ_PCSTR tag, IZ_PCSTR format, ...)
{
    if (tag) {
        IZ_DISP_LOG_WITH_TAG(LogType::Warn, "Warn    [%s] : ", tag);
    }
    else {
        IZ_DISP_LOG(LogType::Warn, "Warn    : ");
    }
}
