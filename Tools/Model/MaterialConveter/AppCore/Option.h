#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <string>
#include "izDefs.h"

// �t�@�C���^�C�v
enum FileType {
	FileTypeUnknown = 0,
	FileTypeCollada,
	FileTypeXFile,
	FileTypeFBX,

	FileTypeNum,
};

struct SOption {
	std::string in;		// ���̓t�@�C��
	std::string out;	// �o�̓t�@�C��

	FileType fileType;	// �t�@�C���^�C�v
};

class COption : public SOption {
public:
	COption();
	~COption();

public:
	// ���
	IZ_BOOL Analysis(int argc, char* argv[]);

	// �o�̓t�@�C��������g���q����菜��
	IZ_BOOL RemoveExtFromExportFileName();

	IZ_BOOL IsValid();

private:
	IZ_BOOL AnalysisInternal();
};

#endif	// #if !defined(__OPTION_H__)
