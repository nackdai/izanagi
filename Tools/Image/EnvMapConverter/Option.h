#if !defined(__OPTION_H__)
#define __OPTION_H__

#include "izToolKit.h"
#include "EnvMapConverterDefs.h"

struct SOption {
	izanagi::izanagi_tk::CString in;
	izanagi::izanagi_tk::CString out;

	izanagi::izanagi_tk::TextureExportType typeExport;
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
