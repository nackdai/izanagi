#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <vector>
#include "izToolKit.h"
#include "EnvMapConverterDefs.h"

struct SOption {
	izanagi::tool::CString in;
	izanagi::tool::CString out;

	izanagi::tool::TextureExportType typeExport;
	EnvMapType typeInEnvMap;
	EnvMapType typeOutEnvMap;
};

class COption : public SOption {
public:
	COption();
	~COption() {}

public:
	IZ_BOOL Analysis(int argc, TCHAR* argv[]);

	IZ_BOOL IsValid() const;

private:
	IZ_BOOL AfterAnalysis();
};

#endif	// #if !defined(__OPTION_H__)
