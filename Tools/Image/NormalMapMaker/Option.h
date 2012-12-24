#if !defined(__OPTION_H__)
#define __OPTION_H__

#include "izToolKit.h"

struct SOption {
    izanagi::tool::CString in;
    izanagi::tool::CString out;

    IZ_FLOAT scale;
};

class COption : public SOption {
public:
    COption()
    {
        scale = 1.0f;
    }
    ~COption() {}

public:
    IZ_BOOL Analysis(int argc, TCHAR* argv[]);

    IZ_BOOL IsValid() const;

private:
    IZ_BOOL AfterAnalysis();
};

#endif  // #if !defined(__OPTION_H__)
