#if !defined(__OPTION_H__)
#define __OPTION_H__

#include "izToolKit.h"

struct SOption {
    izanagi::tool::CString in;
    izanagi::tool::CString out;

    izanagi::E_PLATFORM type;
};

class COption : public SOption {
public:
    COption();

    BOOL Analysis(int argc, TCHAR* argv[]);

    BOOL IsValid() const;

private:
    BOOL AfterAnalysis();
};

#endif  // #if !defined(__OPTION_H__)
