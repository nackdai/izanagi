#if !defined(__IZANAGI_STD_STD_LOG_H__)
#define __IZANAGI_STD_STD_LOG_H__

#include "izDefs.h"

namespace izanagi {
    enum LogType : IZ_UINT {
        Debug = 1,
        Error = 1 << 1,
        Info = 1 << 2,
        Verbose = 1 << 3,
        Warn = 1 << 4,
    };

    /**
    * ログ
    */
    class CLog {
    private:
        CLog();
        ~CLog();

        NO_COPIABLE(CLog);

    public:
        static void enableFlag(LogType type);
        static void disableFlag(LogType type);

        static IZ_BOOL isLoggable(LogType type);

        static void Debug(IZ_PCSTR tag, IZ_PCSTR format, ...);
        static void Error(IZ_PCSTR tag, IZ_PCSTR format, ...);
        static void Info(IZ_PCSTR tag, IZ_PCSTR format, ...);
        static void Verbose(IZ_PCSTR tag, IZ_PCSTR format, ...);
        static void Warn(IZ_PCSTR tag, IZ_PCSTR format, ...);
    };
}   // namespace izanagi

#define IZ_LOGD(f, ...)      izanagi::CLog::Debug(nullptr, f, __VA_ARGS__)
#define IZ_LOGE(f, ...)      izanagi::CLog::Error(nullptr, f, __VA_ARGS__)
#define IZ_LOGI(f, ...)      izanagi::CLog::Info(nullptr, f, __VA_ARGS__)
#define IZ_LOGV(f, ...)      izanagi::CLog::Verbose(nullptr, f, __VA_ARGS__)
#define IZ_LOGW(f, ...)      izanagi::CLog::Warn(nullptr, f, __VA_ARGS__)

#define IZ_LOGD_WITH_TAG(t, f, ...)      izanagi::CLog::Debug(t, f, __VA_ARGS__)
#define IZ_LOGE_WITH_TAG(t, f, ...)      izanagi::CLog::Error(t, f, __VA_ARGS__)
#define IZ_LOGI_WITH_TAG(t, f, ...)      izanagi::CLog::Info(t, f, __VA_ARGS__)
#define IZ_LOGV_WITH_TAG(t, f, ...)      izanagi::CLog::Verbose(t, f, __VA_ARGS__)
#define IZ_LOGW_WITH_TAG(t, f, ...)      izanagi::CLog::Warn(t, f, __VA_ARGS__)

#define IZ_LOG_ASSERT(b, t, f, ...)         if (!b) { IZ_LOGE(t, f, __VA_ARGS__); IZ_ASSERT(IZ_FALSE); }
#define IZ_LOG_VRETURN(b, t, f, ...)        if (!b) { IZ_LOGE(t, f, __VA_ARGS__); VRETURN(IZ_FALSE); }
#define IZ_LOG_VGOTO(b, label, t, f, ...)   if (!b) { IZ_LOGE(t, f, __VA_ARGS__); VGOTO(IZ_FALSE, label); }

#endif  // #if !defined(__IZANAGI_STD_STD_LOG_H__)
