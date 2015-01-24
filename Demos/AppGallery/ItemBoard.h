#if !defined(__ITEM_BOARD_H__)
#define __ITEM_BOARD_H__

#include "izSampleKit.h"
#include "MeshVtx.h"

class ItemBoard : public izanagi::CDebugMeshRectangle {
    friend class izanagi::CDebugMeshTmpl<izanagi::SMeshVtx>;

public:
    // Create an instance.
    static ItemBoard* Create(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        IZ_UINT flag,
        IZ_COLOR color,
        IZ_FLOAT width,
        IZ_FLOAT height);

private:
    ItemBoard() {}
    virtual ~ItemBoard() {}

private:
    IZ_BOOL SetVtx(
        IZ_UINT flag,
        IZ_COLOR nColor,
        IZ_UINT nDivideX,
        IZ_UINT nDivideY,
        IZ_FLOAT fWidth,
        IZ_FLOAT fHeight);
};

#endif    // #if !defined(__ITEM_BOARD_H__)
