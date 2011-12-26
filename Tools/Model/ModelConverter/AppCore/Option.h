#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <string>
#include "izDefs.h"
#include "izToolKit.h"

// �o�̓^�C�v
enum ExportType {
	ExportTypeMdl = 1 << 0,	// MDL�Ƃ��ďo��
	ExportTypeMsh = 1 << 1,	// MSH���o��
	ExportTypeSkl = 1 << 2,	// SKL���o��
};

// �t�@�C���^�C�v
enum FileType {
	FileTypeUnknown = 0,
	FileTypeCollada,
	FileTypeXFile,
	FileTypeFBX,

	FileTypeNum,
};

struct SOption {
	izanagi::tool::CString in;		// ���̓t�@�C��
	izanagi::tool::CString out;	// �o�̓t�@�C��

	izanagi::tool::CString outMsh;
	izanagi::tool::CString outSkl;

	IZ_UINT exportType;	// �o�̓^�C�v
	FileType fileType;	// �t�@�C���^�C�v

	IZ_UINT maxJointMtxNum;
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
