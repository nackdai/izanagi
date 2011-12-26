#if !defined(__OPTION_H__)
#define __OPTION_H__

#include "izDefs.h"

struct SOption {
	CString fileIn;
	CString fileOut;

	IZ_FLOAT threshold;
	IZ_UINT blocksize;

	IZ_BOOL isEncode;
};

class COption : public SOption {
public:
	COption();
	~COption() {}

	NO_COPIABLE(COption);

public:
	BOOL Analysis(int argc, char* argv[]);
};

#endif	// #if !defined(__OPTION_H__)
