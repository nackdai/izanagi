#if !defined(__IMAGE_BUILDER_DEFS_H__)
#define __IMAGE_BUILDER_DEFS_H__

#include <vector>
#include "izSceneGraph.h"
#include "izToolKit.h"

struct SImageElement {
    izanagi::tool::CString path;
    IZ_INT texIdx;
    izanagi::graph::E_GRAPH_CUBE_TEX_FACE face;

    std::vector<std::string> mippaths;

    SImageElement()
    {
        texIdx = -1;
        face = izanagi::graph::E_GRAPH_CUBE_TEX_FACE_NUM;
    }
};

struct SImageInfo {
    izanagi::S_IMG_TEX_HEADER info;

    std::vector<SImageElement> elements;

    SImageInfo()
    {
        memset(&info, 0, sizeof(info));
        info.level = 1;
    }
};

#endif  // #if !defined(__IMAGE_BUILDER_DEFS_H__)
