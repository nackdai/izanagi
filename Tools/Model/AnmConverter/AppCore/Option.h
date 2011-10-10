#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <string>
#include "izDefs.h"
#include "izToolKit.h"

// �t�@�C���^�C�v
enum FileType {
	FileTypeUnknown = 0,
	FileTypeCollada,
	FileTypeXFile,
	FileTypeFBX,

	FileTypeNum,
};

struct SOption {
	izanagi::izanagi_tk::CString in;		// ���̓t�@�C��
	izanagi::izanagi_tk::CString out;	// �o�̓t�@�C��

	std::string base;	// �x�[�X���f���t�@�C��

	FileType fileType;	// �t�@�C���^�C�v

	IZ_UINT idx;
};

class COption : public SOption {
public:
	COption();
	~COption();

public:
	// ���
	IZ_BOOL Analysis(int argc, char* argv[]);

	IZ_BOOL IsValid();

private:
	IZ_BOOL AnalysisInternal();
};

#endif	// #if !defined(__OPTION_H__)
