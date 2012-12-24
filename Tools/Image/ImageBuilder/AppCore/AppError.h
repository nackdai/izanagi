#if !defined(__APP_ERROR_H__)
#define __APP_ERROR_H__

#include "izToolKit.h"

class CErrorLog : public izanagi::tool::ILog {
public:
    CErrorLog(IZ_PCSTR format, IZ_PCSTR file_name, IZ_UINT line, ...)
    {
        static char buf[256];

        va_list vlist;
        
        va_start(vlist, format);
        vsnprintf_s(buf, sizeof(buf), sizeof(buf), format, vlist);
        va_end(vlist);

        m_ErrStr.format("%s (%s)[%d]", buf, file_name, line);
    }

    ~CErrorLog() {}

public:
    IZ_PCSTR GetString() const { return m_ErrStr; }

private:
    izanagi::tool::CString m_ErrStr;
};

#endif  // #if !defined(__APP_ERROR_H__)
