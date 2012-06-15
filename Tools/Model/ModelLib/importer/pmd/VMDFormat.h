#if !defined(__MODEL_LIB_VMD_FORMAT_H__)
#define __MODEL_LIB_VMD_FORMAT_H__

#include "PMDDefs.h"

// VMDフォーマット
// +---------------------+
// | ファイルヘッダ      |
// +---------------------+
// | フレームデータ      |
// +---------------------+

#pragma pack(push,1)

// ファイルヘッダ.
struct SVmdHeader
{
	char magic[30];
	char name[20];
	DWORD frameNum;
};

// フレームデータ.
struct SVmdFrame
{
	char jointName[15];
	DWORD frameNo;

	IZ_FLOAT trans[3];
	IZ_FLOAT quat[4];

	char interpParam[64];
};

#pragma pack(pop)

#endif	// #if !defined(__MODEL_LIB_VMD_FORMAT_H__)
