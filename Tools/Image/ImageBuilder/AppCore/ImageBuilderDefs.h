﻿#if !defined(__IMAGE_BUILDER_DEFS_H__)
#define __IMAGE_BUILDER_DEFS_H__

#include <vector>
#include "izSceneGraph.h"

struct SImageInfo {
	izanagi::S_IMG_TEX_HEADER info;

	CString path;
	INT tex_idx;

	SImageInfo()
	{
		memset(&info, 0, sizeof(info));
		tex_idx = -1;
	}
};

#endif	// #if !defined(__IMAGE_BUILDER_DEFS_H__)
