#if !defined(__FILE_DEFS_H__)
#define __FILE_DEFS_H__

#include "izToolKit.h"

struct SFile {
	izanagi::izanagi_tk::CString path;	// パス

	struct {
		UINT isCompress	: 1;	// 圧縮
		UINT isEncrypt	: 1;	// 暗号化
		UINT isIgnore	: 1;
	};

	UINT name_pos;		// 名前位置

	SFile()
	{
		isCompress = FALSE;
		isEncrypt = FALSE;
		isIgnore = FALSE;

		name_pos = 0;
	}
};

#endif	// #if !defined(__FILE_DEFS_H__)
